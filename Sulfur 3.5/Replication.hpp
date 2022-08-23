#pragma once
#include "ServerFunctions.h"
#include <future>

UChannel* (*CreateChannel)(UNetConnection*, int, bool, int32_t);
__int64(*SetChannelActor)(UActorChannel*, AActor*);
void(*CallPreReplication)(AActor*, UNetDriver*);
bool(*ReplicateActor)(UActorChannel*);
void(*SendClientAdjustment)(AActor*);
static void* (*AddNetworkActor)(UWorld*, AActor*);
static char (*IsNetRelevantFor)(AActor*, AActor*, AActor*, FVector&);
static char (*SupportsObject)(void* NetGUIDCache, UObject* Object, void**);
static void (*CloseActorChannel)(UActorChannel*);
static void (*NotifyActorDestroyed)(UNetDriver* NetDriver, AActor* Actor, char IsSeamlessTravel);
static UActorChannel* (*FindActorChannelRef)(UNetConnection*, TWeakObjectPtr<AActor>&);

std::vector<AActor*> PrivateObjectList;

inline UActorChannel* ReplicateToClient(AActor* InActor, UNetConnection* InConnection)
{
	if (InActor->IsA(APlayerController::StaticClass()) && InActor != InConnection->PlayerController)
		return nullptr;

	auto Channel = (UActorChannel*)(CreateChannel(InConnection, 2, true, -1));

	if (Channel) {
		SetChannelActor(Channel, InActor);
		Channel->Connection = InConnection;
	}

	return Channel;
}

inline int PrepConnections(UNetDriver* NetDriver)
{
	int ReadyConnections = 0;

	for (int ConnIdx = 0; ConnIdx < NetDriver->ClientConnections.Num(); ConnIdx++)
	{
		UNetConnection* Connection = NetDriver->ClientConnections[ConnIdx];
		if (!Connection) continue;

		AActor* OwningActor = Connection->OwningActor;

		if (OwningActor)
		{
			ReadyConnections++;
			AActor* DesiredViewTarget = OwningActor;

			if (Connection->PlayerController)
			{
				if (AActor* ViewTarget = Connection->PlayerController->GetViewTarget())
				{
					DesiredViewTarget = ViewTarget;
				}
			}

			Connection->ViewTarget = DesiredViewTarget;

			for (int ChildIdx = 0; ChildIdx < Connection->Children.Num(); ++ChildIdx)
			{
				UNetConnection* ChildConnection = Connection->Children[ChildIdx];
				if (ChildConnection && ChildConnection->PlayerController && ChildConnection->ViewTarget)
				{
					ChildConnection->ViewTarget = DesiredViewTarget;
				}
			}
		}
		else
		{
			Connection->ViewTarget = nullptr;

			for (int ChildIdx = 0; ChildIdx < Connection->Children.Num(); ++ChildIdx)
			{
				UNetConnection* ChildConnection = Connection->Children[ChildIdx];
				if (ChildConnection && ChildConnection->PlayerController && ChildConnection->ViewTarget)
				{
					ChildConnection->ViewTarget = nullptr;
				}
			}
		}
	}

	return ReadyConnections;
}

inline UActorChannel* FindChannel(AActor* Actor, UNetConnection* Connection)
{
	static auto ActorChannelClass = UActorChannel::StaticClass();

	for (int i = 0; i < Connection->OpenChannels.Num(); i++)
	{
		auto Channel = Connection->OpenChannels[i];

		if (Channel && Channel->Class)
		{
			if (Channel->Class == ActorChannelClass)
			{
				if (((UActorChannel*)Channel)->Actor == Actor)
					return ((UActorChannel*)Channel);
			}
		}
	}

	return NULL;
}

UNetConnection* GetOwningConnection(AActor* Actor)
{
	for (auto Owner = Actor; Actor; Actor = Actor->GetOwner())
	{
		if (Actor->IsA(APlayerController::StaticClass()))
		{
			return ((APlayerController*)Actor)->NetConnection;
		}
	}
}

std::vector<AActor*>& GetNetworkObjectList()
{
	return PrivateObjectList;
}

void BuildConsiderList(UNetDriver* NetDriver, std::vector<AActor*>& OutConsiderList)
{
	std::vector<AActor*> ActorsToRemove = std::vector<AActor*>();

	for (auto Actor : GetNetworkObjectList())
	{
		if (!Actor)
			continue;

		if (Actor->bActorIsBeingDestroyed)
		{
			ActorsToRemove.push_back(Actor);
			continue;
		}

		if (Actor->RemoteRole == ENetRole::ROLE_None)
		{
			ActorsToRemove.push_back(Actor);
			continue;
		}

		if (Actor->NetDriverName.ComparisonIndex != NetDriver->NetDriverName.ComparisonIndex)
			continue;

		if (Actor->NetDormancy == ENetDormancy::DORM_Initial && Actor->bNetStartup)
		{
			ActorsToRemove.push_back(Actor);
			continue;
		}

		if (!Actor->bReplicates)
			continue;			

		if (Actor->Name.ComparisonIndex != 0)
		{
			CallPreReplication(Actor, NetDriver);
			OutConsiderList.push_back(Actor);
		}
	}

	for (auto Actor : ActorsToRemove)
		GetNetworkObjectList().erase(std::find(GetNetworkObjectList().begin(), GetNetworkObjectList().end(), Actor));

	ActorsToRemove.clear();
	ActorsToRemove.empty();
}

struct ProcessConnectionParams
{
	int* Connections;
	UNetConnection* Connection;
	std::vector<AActor*> ConsiderList;
};

static bool IsActorRelevant(AActor* Actor, AActor* Viewer)
{
	auto Loc = Viewer->K2_GetActorLocation();

	if (IsNetRelevantFor(Actor, Viewer, Viewer, Loc))
	{
		return true;
	}

	return false;
}

void ProcessConnection(ProcessConnectionParams* Params)
{
	auto Connection = Params->Connection;
	auto ConsiderList = Params->ConsiderList;
	
	if (!Connection->ViewTarget)
	{
		*Params->Connections++;
		return;
	}

	if (Connection->PlayerController)
		SendClientAdjustment(Connection->PlayerController);

	for (auto Actor : ConsiderList)
	{
		auto Channel = FindChannel(Actor, Connection);

		if (!Actor->bAlwaysRelevant && !Actor->bNetUseOwnerRelevancy && !Actor->bOnlyRelevantToOwner)
		{
			if (!IsActorRelevant(Actor, Connection->ViewTarget))
			{
				if (Channel)
					CloseActorChannel(Channel);
				continue;
			}
		}

		if (!Channel)
			Channel = ReplicateToClient(Actor, Connection);

		if (Channel)
			ReplicateActor(Channel);
	}

	*Params->Connections++;
}

void Tick(UNetDriver* NetDriver)
{
	++* (DWORD*)(__int64(NetDriver) + 816);

	auto NumClientsToTick = PrepConnections(NetDriver);

	if (NumClientsToTick == 0)
		return;

	std::vector<AActor*> ConsiderList = std::vector<AActor*>();
	BuildConsiderList(NetDriver, ConsiderList);

	int ProcessedConnections = 0;
	int ConnectionsToProcess = NetDriver->ClientConnections.Num();

	for (int i = 0; i < NetDriver->ClientConnections.Num(); i++)
	{
		auto Connection = NetDriver->ClientConnections[i];

		if (!Connection->ViewTarget)
			continue;

		if ((*(__int64(__fastcall**)(__int64))(*(__int64*)Connection + 664))(*(__int64*)((__int64)Connection)))
		{
			ProcessedConnections++;
			continue;
		}

		auto fut = std::async(std::launch::async, ProcessConnection, new ProcessConnectionParams{ &ProcessedConnections, Connection, ConsiderList });
		fut.get();
	}

	while (ConnectionsToProcess != ProcessedConnections)
	{
		continue;
	}

	ConsiderList.empty();
	ConsiderList.clear();
}

static void* AddNetworkActorHook(UWorld* World, AActor* Actor)
{
	if (Actor)
	{
		GetNetworkObjectList().push_back(Actor);
	}

	return AddNetworkActor(World, Actor);
}