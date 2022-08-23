#include "framework.h"

UWorld* World;

#include "Replication.hpp"
#include "ServerFunctions.h"
#include <fstream>
#include <thread>
#include <ostream>

static bool (*InitHost)(AOnlineBeaconHost*);
static __int64 (*NotifyControlMessage)(UWorld*, UNetConnection*, uint8_t, void*);
static void (*WelcomePlayer)(UWorld*, UNetConnection*);
static APlayerController* (*SpawnPlayActor)(UWorld*, UNetConnection* Connection, ENetRole Role, FURL& URL, void* UniqueID, FString& Error, uint8_t InNetPlayerIndex);
static void* (*TickFlush)(UNetDriver*);
static void (*ReceiveFString)(void* Bunch, FString& Str);
static void (*ReceiveUniqueIdRepl)(void* Bunch, FUniqueNetIdRepl& Str);
static void* (*ProcessEvent)(void*, void*, void*);
static char (*InternalTryActivateAbility)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle, FPredictionKey, UGameplayAbility** OutInstancedAbility, void* Delegate, FGameplayEventData* TriggerEventData);
static UObject* (*StaticLoadObject)(UClass* ObjectClass, SDK::UObject* InOuter, const wchar_t* InName, const wchar_t* Filename, uint32_t LoadFlags, SDK::UPackageMap* Sandbox, bool bAllowObjectReconciliation);
static __int64 (*DestroyActor)(AActor* Actor, unsigned __int8 a2, unsigned __int8 a3);
static bool (*DestroySwappedPC)(UWorld* World, UNetConnection* Connection);
static void* (*SeamlessTravelHandlerForWorld)(UEngine* Engine, UWorld* World);
static FGameplayAbilitySpec*(*FindAbilitySpecFromHandle)(UAbilitySystemComponent* a1, int a2);
static void* (*CallActivateAbility)(UGameplayAbility*, FGameplayAbilitySpecHandle, FGameplayAbilityActorInfo*, FGameplayAbilityActivationInfo, void*, FGameplayEventData*);
static void* (*OnGamepadAbilityRelease)(AFortPlayerControllerAthena* PlayerController, int a2);
static void* (*EquipWeaponDefinition)(AFortPlayerPawnAthena*, UFortItemDefinition*, FGuid);
static bool* (*TryActivateAbility)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle, bool bAllowRemoteActivation);
static void* (*OnReload)(AFortWeapon* a1, int a2);
static void* (*WeaponInit)(AFortWeapon* a1, AFortWeapon* a2);

uint8_t CurrentTeamIndex = 3;
std::map<int, EFortTeam> Teams;

UNetDriver* CurrentNetDriver;
bool bHasInitializedPlayer = false;
static bool hasAnnounced = false;

static FVector GetSpawnLocation()
{
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(Globals::GEngine->GameViewport->World, AFortPlayerStartWarmup::StaticClass(), &PlayerStarts);

    auto Idx = rand() % (PlayerStarts.Num() - 1);
    auto PlayerStart = PlayerStarts[Idx];
    return PlayerStart->K2_GetActorLocation();
}

static FGameplayAbilitySpecHandle* GiveAbilityAndActivateOnce(UAbilitySystemComponent* Comp, FGameplayAbilitySpec& Spec)
{
    FGameplayAbilitySpecHandle* AddedAbilityHandle = ServerFunctions::GiveAbility(Comp, &Spec);

    FGameplayAbilitySpec* FoundSpec = ServerFunctions::FindAbilitySpecFromHandle(Comp, *AddedAbilityHandle);

    if (FoundSpec)
    {
        FoundSpec->RemoveAfterActivation = true;

        auto eventdata = new FGameplayEventData();

        Comp->ClientTryActivateAbility(*AddedAbilityHandle);
    }

    return AddedAbilityHandle;
}

static AFortPickupAthena* SpawnPickup(FVector Location, UFortItemDefinition* ItemDef = ServerFunctions::GetRandomItemDefinition(), int Count = 1)
{
    if (Count == 1 && ItemDef->IsA(UFortAmmoItemDefinition::StaticClass()))
        Count = ((UFortAmmoItemDefinition*)(ItemDef))->DropCount;

    auto Pickup = World->SpawnActor<AFortPickupAthena>(Location, {});

    Pickup->PrimaryPickupItemEntry.ItemDefinition = ItemDef;
    Pickup->PrimaryPickupItemEntry.Count = Count;

    Pickup->TossPickup(Location, nullptr, 6, true);

    return Pickup;
}

static void InitGameplay()
{
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), &Actors);

    for (int i = 0; i < Actors.Num(); i++)
    {
        auto Actor = Actors[i];
        bool bShouldSpawn = UKismetMathLibrary::RandomBoolWithWeight(0.6f);
        bool bIsConsumable = !UKismetMathLibrary::RandomBoolWithWeight(0.2f);

        if (bShouldSpawn && Actor->Class->GetName().contains("Tiered_Athena_FloorLoot"))
        {
            if (bIsConsumable)
            {
                auto WeaponPickup = SpawnPickup(Actor->K2_GetActorLocation());
                auto WeaponDef = (UFortWeaponItemDefinition*)WeaponPickup->PrimaryPickupItemEntry.ItemDefinition;

                auto Ammo = World->SpawnActor<AFortPickupAthena>(Actor->K2_GetActorLocation(), {});

                auto AmmoDef = WeaponDef->GetAmmoWorldItemDefinition_BP();

                Ammo->PrimaryPickupItemEntry.ItemDefinition = AmmoDef;

                Ammo->PrimaryPickupItemEntry.Count = AmmoDef->DropCount;

                Ammo->TossPickup(Actor->K2_GetActorLocation(), nullptr, 6, true);
            }
            else
            {
                SpawnPickup(Actor->K2_GetActorLocation(), ServerFunctions::GetRandomConsumableItemDefinition());
            }
        }
    }
}

static APlayerController* SpawnPlayActorHook(UWorld*, UNetConnection* NewPlayer, ENetRole Role, FURL& URL, void* UniqueID, FString& Error, uint8_t InNetPlayerIndex)
{
    auto PlayerController = (AFortPlayerControllerAthena*)SpawnPlayActor(World, NewPlayer, Role, URL, UniqueID, Error, InNetPlayerIndex);

    NewPlayer->PlayerController = PlayerController;

    if (NewPlayer->IsA(ULocalPlayer::StaticClass()))
        return PlayerController;

    static bool bReady = false;

    if (!bReady)
    {
        bReady = true;
        // CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&InitGameplay, 0, 0, 0);
    }

    auto Gamestate = (AFortGameStateAthena*)World->GameState;

    AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)PlayerController->PlayerState;
    PlayerState->SetOwner(PlayerController);

    auto Pawn = Globals::GEngine->GameViewport->World->SpawnActor<APlayerPawn_Athena_C>(GetSpawnLocation(), {});

    if (!Pawn)
        return PlayerController;

    Pawn->SetOwner(PlayerController);

    PlayerController->Pawn = Pawn;
    Pawn->Owner = PlayerController;
    Pawn->OnRep_Owner();
    PlayerController->OnRep_Pawn();
    PlayerController->Possess(Pawn);

    std::string SprintAbility;
    std::string JumpAbility;
    std::string SearchAbility;
    std::string DeathAbility;
    std::string ReloadAbility;
	
    SprintAbility = "/Script/FortniteGame.FortGameplayAbility_Sprint";
    JumpAbility = "/Script/FortniteGame.FortGameplayAbility_Jump";
    SearchAbility = "BlueprintGeneratedClass GA_DefaultPlayer_InteractSearch.GA_DefaultPlayer_InteractSearch_C";
    DeathAbility = "BlueprintGeneratedClass GA_DefaultPlayer_Death.GA_DefaultPlayer_Death_C";
    ReloadAbility = "/Script/FortniteGame.FortGameplayAbility_Reload";


    auto SprintAbilityVariable = Utils::FindObjectFast<UClass>(SprintAbility);
    auto JumpAbilityVariable = Utils::FindObjectFast<UClass>(JumpAbility);
    auto ReloadAbilityVariable = Utils::FindObjectFast<UClass>(ReloadAbility);	
	
    auto SearchAbilityVariable = UObject::FindClass(SearchAbility);
    auto DeathAbilityVariable = UObject::FindClass(DeathAbility);

    if (SprintAbilityVariable) {
        ServerFunctions::GrantGameplayAbility(Pawn, SprintAbilityVariable);
    }

    if (JumpAbilityVariable) {
        ServerFunctions::GrantGameplayAbility(Pawn, JumpAbilityVariable);
    }

    if (SearchAbilityVariable) {
        ServerFunctions::GrantGameplayAbility(Pawn, SearchAbilityVariable);
    }
	
    if (DeathAbilityVariable) {
        ServerFunctions::GrantGameplayAbility(Pawn, DeathAbilityVariable);
    }

    if (ReloadAbilityVariable) {
        ServerFunctions::GrantGameplayAbility(Pawn, ReloadAbilityVariable);
    }

    Pawn->bCanBeDamaged = false;

    Pawn->bReplicateMovement = true;
    Pawn->OnRep_ReplicateMovement();

    auto QuickBars = Globals::GEngine->GameViewport->World->SpawnActor<AFortQuickBars>({ 0, 0, 0 }, {});

    // if (!QuickBars)
        // return PlayerController;

    PlayerController->QuickBars = QuickBars;
    PlayerController->QuickBars->SetOwner(PlayerController);
    PlayerController->QuickBars->OnRep_Owner();
    PlayerController->OnRep_QuickBar();

    PlayerController->CheatManager = (UCheatManager*)UGameplayStatics::SpawnObject(UFortCheatManager::StaticClass(), PlayerController);

    PlayerController->WorldInventory->SetOwner(PlayerController);

    ServerFunctions::SetupInventory(PlayerController);

    PlayerController->bHasClientFinishedLoading = true;
    PlayerController->bHasServerFinishedLoading = true;
    PlayerController->OnRep_bHasServerFinishedLoading();

    PlayerState->bHasFinishedLoading = true;
    PlayerState->bHasStartedPlaying = true;
    PlayerState->OnRep_bHasStartedPlaying();

    static auto HeroType = Utils::FindObjectFast<UFortHeroType>("/Game/Athena/Heroes/HID_042_Athena_Commando_M_Cyberpunk.HID_042_Athena_Commando_M_Cyberpunk");
	
    if (HeroType)
        PlayerState->HeroType = HeroType;
	
    PlayerState->OnRep_HeroType();

    static auto HeadCharacterPart = Utils::FindObjectFast<UCustomCharacterPart>("/Game/Characters/CharacterParts/Female/Medium/Heads/F_Med_Head1.F_Med_Head1");
    static auto BodyCharacterPart = Utils::FindObjectFast<UCustomCharacterPart>("/Game/Characters/CharacterParts/Female/Medium/Bodies/F_Med_Soldier_01.F_Med_Soldier_01");

    if (HeadCharacterPart)
         PlayerState->CharacterParts[0] = HeadCharacterPart;
	if (BodyCharacterPart)
        PlayerState->CharacterParts[1] = BodyCharacterPart;
	
    if (HeadCharacterPart || BodyCharacterPart)
        PlayerState->OnRep_CharacterParts();
   
    PlayerState->OnRep_HeroType();
    
    //  PlayerController->K2_AttachToActor(Pawn, FName(), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

    Pawn->SetMaxHealth(100);
    Pawn->SetHealth(100);
    Pawn->SetMaxShield(100);

	// Does not work for some reason

     auto PlayerName = ServerFunctions::GetPlayerName(NewPlayer);
    auto TeamId = ServerFunctions::GetTeamId(PlayerName);

    if ((uint8_t)Teams[TeamId] == 0)
        Teams[TeamId] = (EFortTeam)CurrentTeamIndex++;

    PlayerState->TeamIndex = Teams[TeamId];
    PlayerState->SquadId = (uint8_t)Teams[TeamId];
    PlayerState->OnRep_PlayerTeam();
    PlayerState->OnRep_SquadId(); 

    return PlayerController;
}

static __int64 NotifyControlMessageHook(AOnlineBeaconHost*, UNetConnection* Connection, uint8_t MessageType, __int64* Bunch)
{
    if (MessageType == 5) //PreLogin isnt really needed so we can just use this
    {
        Bunch[7] += (16 * 1024 * 1024);

        FString OnlinePlatformName = FString(L"");

        ReceiveFString(Bunch, Connection->ClientResponse);
        ReceiveFString(Bunch, Connection->RequestURL);
        ReceiveUniqueIdRepl(Bunch, Connection->PlayerID);
        ReceiveFString(Bunch, OnlinePlatformName);

        Bunch[7] -= (16 * 1024 * 1024);

        WelcomePlayer(World, Connection);

        return 0;
    }

    if (MessageType == 4)     // 20000 original Tick Rate
    {
        Connection->CurrentNetSpeed = 20000;
        return 0;
    }

    return NotifyControlMessage(World, Connection, MessageType, Bunch);
}

static void* TickFlushHook(UNetDriver* NetDriver)
{
    if (NetDriver->ClientConnections.Num() != 0 && NetDriver->ClientConnections[0]->InternalAck == false)
    {
        Tick(NetDriver);
    }

    return TickFlush(NetDriver);
}

static void* KickPatchHook(__int64 a1, __int64 a2, __int64 a3, __int64 a4)
{
    return 0;
}

static void* KickPatch2Hook(__int64 a1, __int64 a2)
{
    return 0;
}

static char KickPatch3Hook(__int64 a1, __int64 a2, __int64 a3) // AGameSession::KIckPlayer
{
    return 0;
}

static void ShutdownThread()
{
    /* Sleep(15000);

    for (int i = 0; i < World->NetDriver->ClientConnections.Num(); i++)
    {
        auto Connection = World->NetDriver->ClientConnections[i];

        if (Connection->PlayerController)
        {
            ((AFortPlayerControllerAthena*)Connection->PlayerController)->ServerReturnToMainMenu();
        }
    }

    UKismetSystemLibrary::QuitGame(World, Globals::GEngine->GameInstance->LocalPlayers[0]->PlayerController, EQuitPreference::Quit);  */
}

static void* ProcessEventHook(UObject* Object, UFunction* Function, void* Params)
{
    if (!Object || !Function)
        return ProcessEvent(Object, Function, Params);

    auto FuncName = Function->GetName();

    //if ((Function->FunctionFlags & 0x00200000 || Function->FunctionFlags & 0x01000000) && !FuncName.contains("Move") && !FuncName.contains("Camera") && !Object->GetName().contains("CharacterMovement"))
    //{
        //printf("RPC Called %s\n", FuncName.c_str());
    //}

    if (FuncName.contains("ServerPlayEmoteItem"))
    {
        auto PlayerController = (AFortPlayerControllerAthena*)Object;
        auto Pawn = (APlayerPawn_Athena_C*)PlayerController->Pawn;
        auto CurrentParams = (AFortPlayerController_ServerPlayEmoteItem_Params*)Params;

        if (Pawn)
        {
            auto EmoteItem = CurrentParams->EmoteAsset;
            auto Animation = EmoteItem->GetAnimationHardReference(EFortCustomBodyType::All, EFortCustomGender::Both);

            if (Pawn->RepAnimMontageInfo.AnimMontage != Animation)
            {
                Pawn->RepAnimMontageInfo.AnimMontage = Animation;
                Pawn->RepAnimMontageInfo.PlayRate = 1;
                Pawn->RepAnimMontageInfo.IsStopped = false;
                Pawn->RepAnimMontageInfo.SkipPositionCorrection = true;

                Pawn->PlayAnimMontage(Animation, 1, FName());
                Pawn->OnRep_AttachmentMesh();
                Pawn->OnRep_AttachmentReplication();
                Pawn->OnRep_ReplicatedAnimMontage();
            }
        }
    }

    if (FuncName.contains("ServerCheat"))
    {
        return NULL;
    }
		
    if (FuncName.contains("ServerAbilityRPCBatch"))
    {
        std::cout << "ServerAbilityRPCBatch\n";
        auto AbilityComp = (UAbilitySystemComponent*)Object;
        auto CurrentParams = (UAbilitySystemComponent_ServerAbilityRPCBatch_Params*)Params;
        auto BatchInfo = CurrentParams->BatchInfo;

        AbilityComp->ServerTryActivateAbility(BatchInfo.AbilitySpecHandle, BatchInfo.InputPressed, BatchInfo.PredictionKey);
        AbilityComp->ServerSetReplicatedTargetData(BatchInfo.AbilitySpecHandle, BatchInfo.PredictionKey, BatchInfo.TargetData, FGameplayTag(), BatchInfo.PredictionKey);

        if (BatchInfo.Ended) 
        {
            FGameplayAbilityActivationInfo FakeInfo;
            FakeInfo.PredictionKeyWhenActivated = BatchInfo.PredictionKey;
            AbilityComp->ServerEndAbility(BatchInfo.AbilitySpecHandle, FakeInfo, BatchInfo.PredictionKey);
        }

        return 0;
    }

    if (FuncName == "ServerAttemptInteract")
    {
        auto PlayerController = (AFortPlayerControllerAthena*)Object;
        auto CurrentParams = (AFortPlayerController_ServerAttemptInteract_Params*)Params;

        auto ReceivingActor = CurrentParams->ReceivingActor;

        if (ReceivingActor && ReceivingActor->Class->GetName().contains("Tiered_Short_Ammo"))
        {
            auto Ammo = (ABuildingContainer*)ReceivingActor;
            Ammo->bAlreadySearched = true;
            Ammo->OnRep_bAlreadySearched();

            auto Location = ReceivingActor->K2_GetActorLocation();

            for (int i = 0; i < 2; i++)
            {
                SpawnPickup(Location, ServerFunctions::GetRandomAmmoItemDefinition());
            }
        }

        if (ReceivingActor && ReceivingActor->Class->GetName().contains("Tiered_Chest"))
        {
            auto Chest = (ABuildingContainer*)ReceivingActor;
            Chest->bAlreadySearched = true;
            Chest->OnRep_bAlreadySearched();

            auto Location = ReceivingActor->K2_GetActorLocation();

            auto WeaponDef = ServerFunctions::GetRandomItemDefinition();
            auto Pickup = SpawnPickup(Location, WeaponDef);

            SpawnPickup(Location, ServerFunctions::GetRandomConsumableItemDefinition());
            SpawnPickup(Location, ((UFortWeaponItemDefinition*)WeaponDef)->GetAmmoWorldItemDefinition_BP());
            SpawnPickup(Location, ServerFunctions::GetRandomResourceItemDefinition(), 30);
			
            return NULL; 
        }
    }

    if (FuncName == "ClientOnPawnDied")
    {
        auto CurrentParams = (AFortPlayerControllerZone_ClientOnPawnDied_Params*)Params;
        auto PlayerController = (AFortPlayerControllerAthena*)Object;


        if (CurrentParams->DeathReport.KillerPlayerState)
        {
            ((AFortPlayerStateAthena*)CurrentParams->DeathReport.KillerPlayerState)->ClientReportKill((AFortPlayerStateAthena*)PlayerController->PlayerState);
            ((AFortPlayerStateAthena*)CurrentParams->DeathReport.KillerPlayerState)->KillScore++;
            ((AFortPlayerStateAthena*)CurrentParams->DeathReport.KillerPlayerState)->OnRep_Kills();
            ((AFortPlayerStateAthena*)CurrentParams->DeathReport.KillerPlayerState)->OnRep_Score();
        }

        TArray<AActor*> Actors;
        UGameplayStatics::GetAllActorsOfClass(World, APlayerPawn_Athena_C::StaticClass(), &Actors);
		
        if (PlayerController->WorldInventory)
        {
            for (int i = 0; i < PlayerController->WorldInventory->Inventory.ItemInstances.Num(); i++)
            {
                auto ItemInstance = PlayerController->WorldInventory->Inventory.ItemInstances[i];
                auto ItemDef = ItemInstance->GetItemDefinitionBP();

                if (ItemDef->IsA(UFortBuildingItemDefinition::StaticClass()))
                    continue;

                if (ItemDef->IsA(UFortWeaponMeleeItemDefinition::StaticClass()))
                    continue;

                SpawnPickup(PlayerController->K2_GetActorLocation(), ItemDef, ItemInstance->ItemEntry.Count);
            }

            if (Actors.Num() == 1 || Actors.Num() == 2 && Actors.Contains(PlayerController->Pawn))
            {
                for (int ConnIdx = 0; ConnIdx < World->NetDriver->ClientConnections.Num(); ConnIdx++)
                {
                    auto PlayerController = (AFortPlayerControllerAthena*)World->NetDriver->ClientConnections[ConnIdx]->PlayerController;
                    if (PlayerController->Pawn)
                        PlayerController->ClientNotifyWon();
                }
                // CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&ShutdownThread, 0, 0, 0);
            }
        }

        PlayerController->ClientReturnToMainMenu(L"You died, please wait for match to end! Spectating was removed to fix and prevent server lag.");

        /* for (int i = 0; i < Actors.Num(); i++)
        {
            auto Pawn = (APlayerPawn_Athena_C*)Actors[i];

            if (Pawn && !Pawn->IsDead() && Pawn != PlayerController->Pawn)
            {
                if (!PlayerController->SpectatorPawn)
                {
                    PlayerController->SpectatorPawn = World->SpawnActor<ASpectatorPawn>(FVector{ 0,0, 10000 }, FRotator{ 0,0,0 });
                }

                if (PlayerController->SpectatorPawn && Pawn)
                {
                    //PlayerController->SpectateOnDeath();
                 //   PC->SetViewTargetWithBlend(Pawntofollow, 1.0, EViewTargetBlendFunction::VTBlend_EaseInOut, 1, true);
                    PlayerController->SpectatorPawn->K2_SetActorLocation(ServerFunctions::CalculateSpectatorPawnLocation(Pawn), false, true, new FHitResult());
                    PlayerController->SpectatorPawn->K2_SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PlayerController->SpectatorPawn->K2_GetActorLocation(), Pawn->K2_GetActorLocation()), false);
                    PlayerController->SpectatorPawn->SetActorEnableCollision(false);
                    PlayerController->SpectatorPawn->SetActorHiddenInGame(true);
                    PlayerController->Possess(PlayerController->SpectatorPawn);
                    PlayerController->SpectatorPawn->K2_AttachToActor(Pawn, FName(), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
                }

                break;
            }
        } */
    }

    if (FuncName.contains("ServerLoadingScreenDropped"))
    {
        auto Paramss = (AFortPlayerController_ServerLoadingScreenDropped_Params*)Params;

		auto PC = (AFortPlayerControllerAthena*)Object;

        if (PC->Pawn)
            ServerFunctions::GiveGameplayAbilities((APlayerPawn_Athena_C*)PC->Pawn);
    }

    if (FuncName == "ServerHandlePickup")
    {
        auto Pawn = (AFortPlayerPawnAthena*)Object;
        auto PlayerController = (AFortPlayerControllerAthena*)Pawn->Controller;
        auto CurrentParams = (AFortPlayerPawn_ServerHandlePickup_Params*)Params;

        auto Pickup = CurrentParams->Pickup;

        if (Pickup && !Pickup->bPickedUp)
        {
            Pickup->bPickedUp = true;

            auto ItemDef = Pickup->PrimaryPickupItemEntry.ItemDefinition;
            auto ItemInstances = PlayerController->WorldInventory->Inventory.ItemInstances;
            auto QuickbarSlots = PlayerController->QuickBars->PrimaryQuickBar.Slots;
            
            Pickup->K2_DestroyActor();
			
            if (!ItemDef->IsA(UFortWeaponItemDefinition::StaticClass()))
            {
                ServerFunctions::AddItemToInventory(ItemDef, EFortQuickBars::Secondary, PlayerController, Pickup->PrimaryPickupItemEntry.Count, 0);
                return ProcessEvent(Object, Function, Params);
            }
		
            for (int i = 0; i < QuickbarSlots.Num(); i++)
            {
                if (QuickbarSlots[i].Items.Data == 0)
                {
                    if (i >= 6)
                    {
                        int CurrentFocusedSlot = PlayerController->QuickBars->PrimaryQuickBar.CurrentFocusedSlot;

                        if (i == 0)
                            continue;

                        i = CurrentFocusedSlot;

                        FGuid CurrentFocusedGUID = QuickbarSlots[CurrentFocusedSlot].Items[0];

                        for (int j = 0; j < ItemInstances.Num(); j++)
                        {
                            auto ItemInstance = ItemInstances[j];

                            auto CurrentItemDef = ItemInstance->GetItemDefinitionBP();
                            auto CurrentItemGuid = ItemInstance->GetItemGuid();

                            if (Utils::GuidComp(CurrentItemGuid, CurrentFocusedGUID))
                            {
                                SpawnPickup(Pawn->K2_GetActorLocation(), CurrentItemDef);
                            }
                        }

                        PlayerController->QuickBars->EmptySlot(EFortQuickBars::Primary, CurrentFocusedSlot);
                    }

                    ServerFunctions::AddItemToInventory(ItemDef, EFortQuickBars::Primary, PlayerController, 1, i);
                    break;
                }
            }

        }
    }

    if (FuncName.contains("ServerDBNOReviveStarted"))
    {
        auto CurrentParams = (AFortPlayerControllerAthena_ServerDBNOReviveStarted_Params*)Params;
        if (CurrentParams->DBNOPawn)
        {
            CurrentParams->DBNOPawn->bIsDBNO = false;
            CurrentParams->DBNOPawn->OnRep_IsDBNO();

            CurrentParams->DBNOPawn->SetHealth(30.0f);
        }
    }

    if (FuncName.contains("ServerBeginEditingBuildingActor"))
    {
        auto PlayerController = (AFortPlayerControllerAthena*)Object;
        auto Pawn = (AFortPlayerPawn*)PlayerController->Pawn;

        if (!Pawn)
            return ProcessEvent(Object, Function, Params);

        auto CurrentParams = (AFortPlayerController_ServerBeginEditingBuildingActor_Params*)Params;
        auto EditActor = CurrentParams->BuildingActorToEdit;

        if (!EditActor)
            return ProcessEvent(Object, Function, Params);

        auto EditToolDef = Utils::FindObjectFast<UFortWeaponItemDefinition>("/Game/Items/Weapons/BuildingTools/EditTool.EditTool");
        auto EditTool = (AFortWeap_EditingTool*)Pawn->EquipWeaponDefinition(EditToolDef, FGuid{ rand() % 1000, rand() % 1000, rand() % 1000, rand() % 1000 });
        EditTool->EditActor = EditActor;
        EditTool->OnRep_EditActor();
    }

    if (FuncName.contains("ServerAttemptAircraftJump"))
    {
        auto CurrentParams = (AFortPlayerControllerAthena_ServerAttemptAircraftJump_Params*)Params;
        auto PlayerController = (AFortPlayerControllerAthena*)Object;

        if (PlayerController->Pawn)
        {
            printf("Pawn is still valid on playercontroller");
            return ProcessEvent(Object, Function, Params);
        }
           

        if (((AFortGameStateAthena*)Globals::GEngine->GameViewport->World->GameState)->Aircrafts.Num() == 0)
            return ProcessEvent(Object, Function, Params);

        auto Pawn = Globals::GEngine->GameViewport->World->SpawnActor<APlayerPawn_Athena_C>(((AFortGameStateAthena*)Globals::GEngine->GameViewport->World->GameState)->Aircrafts[0]->K2_GetActorLocation(), {});

        if (!Pawn)
        {
            printf("Pawn is not valid");
            return ProcessEvent(Object, Function, Params);
        }
            
		Pawn->K2_SetActorLocation(((AFortGameStateAthena*)Globals::GEngine->GameViewport->World->GameState)->Aircrafts[0]->K2_GetActorLocation(), false, true, new FHitResult());

        Pawn->SetActorEnableCollision(false);

        Pawn->SetOwner(PlayerController);
        PlayerController->ClientRestart(Pawn);
        PlayerController->Possess(Pawn);

        Pawn->bReplicateMovement = true;
        Pawn->OnRep_ReplicateMovement();

        PlayerController->SetControlRotation(CurrentParams->ClientRotation);

        Pawn->SetMaxHealth(100);
        Pawn->SetHealth(100);
        Pawn->SetMaxShield(100);

        PlayerController->Pawn->SetActorEnableCollision(true);
        PlayerController->K2_AttachToActor(Pawn, FName(), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

        PlayerController->ClientRestart(Pawn);

        Pawn->bCanBeDamaged = true;

        ServerFunctions::GiveWeapons(PlayerController);
        ServerFunctions::GiveBackMats(PlayerController);
    }

    if (FuncName.contains("ServerReturnToMainMenu"))
    {
        auto Controller = (AFortPlayerControllerAthena*)Object;

        Controller->ClientReturnToMainMenu(FString());
    }

    if (FuncName.contains("ServerCreateBuilding"))
    {
        auto PlayerController = (AFortPlayerControllerAthena*)Object;
        auto CurrentParams = (AFortPlayerController_ServerCreateBuildingActor_Params*)Params;
        auto PlayerState = (AFortPlayerStateAthena*)PlayerController->PlayerState;

        if (PlayerController->LastBuildPreviewGridSnapCentroid.X != 69)
        {
            auto Building = World->SpawnActor<ABuildingSMActor>(CurrentParams->BuildLoc, CurrentParams->BuildRot, CurrentParams->BuildingClassData.BuildingClass);
            Building->PlacedByPlacementTool();
            Building->bPlayerPlaced = true;
            Building->InitializeKismetSpawnedBuildingActor(Building, PlayerController);

            Building->Team = PlayerState->TeamIndex;
            Building->OnRep_Team();

            PlayerController->LastBuildPreviewGridSnapCentroid.X = 69;

            ServerFunctions::RemoveBuildingAmount(CurrentParams->BuildingClassData.BuildingClass, PlayerController);
        }
        else
            PlayerController->LastBuildPreviewGridSnapCentroid.X = 420;
    }

    if (FuncName.contains("ServerSetInAircraft"))
    {
        static bool bReady = false;

        auto CurrentPlayerState = (AFortPlayerStateAthena*)Object;
        auto PC = (AFortPlayerController*)CurrentPlayerState->GetOwner();

        if (!bReady)
        {
            bReady = true;

		    // if (World->NetDriver->ClientConnections.Num() == 0)
                // UKismetSystemLibrary::QuitGame(World, Globals::GEngine->GameInstance->LocalPlayers[0]->PlayerController, EQuitPreference::Quit);

            if (Currentgm == EGamemode::EndGame)
            {
                ((AFortGameStateAthena*)Globals::GEngine->GameViewport->World->GameState)->Aircrafts[0]->FlightInfo.FlightSpeed = 0.001;
                auto Loc = ServerFunctions::GetRandomBattleBusLocation();
                ((AFortGameStateAthena*)Globals::GEngine->GameViewport->World->GameState)->Aircrafts[0]->FlightInfo.FlightStartLocation = *(FVector_NetQuantize100*)&Loc;
                ((AFortGameStateAthena*)Globals::GEngine->GameViewport->World->GameState)->Aircrafts[0]->K2_SetActorLocation(ServerFunctions::GetRandomBattleBusLocation(), false, true, new FHitResult());

                auto GameState = (AFortGameStateAthena*)World->GameState;
                GameState->bAircraftIsLocked = false;
                GameState->GamePhaseStep = EAthenaGamePhaseStep::BusFlying;
				
                UFortKismetLibrary::SetTimeOfDay(World, 0);
            }
            if (Currentgm == EGamemode::Assassins)
            {
                auto soundeffect = Utils::FindObjectFast<USoundWave>("FortniteGame/Content/Sounds/Fort_GamePlay_Sounds/Portal/EnemyPortal_Lightning06.EnemyPortal_Lightning06");

                ServerFunctions::ClientPlaySoundMulticast(soundeffect);

                ServerFunctions::SetupAssassins();
            }
			
            Globals::Beacon->BeaconState = EBeaconState::DenyRequests;

            return NULL;
        }
    }

    if (FuncName.contains("ServerEditBuildingActor"))
    {
        auto PlayerController = (AFortPlayerControllerAthena*)Object;
        auto CurrentParams = (AFortPlayerController_ServerEditBuildingActor_Params*)Params;

        /*if (CurrentParams->BuildingActorToEdit.IsBe)*/
        if (CurrentParams->BuildingActorToEdit && CurrentParams->NewBuildingClass)
        {
            auto Location = CurrentParams->BuildingActorToEdit->K2_GetActorLocation();
            auto Rotation = CurrentParams->BuildingActorToEdit->K2_GetActorRotation();


           /* UKismetMathLibrary::GridSnap_Float()

            auto ForwardVector = CurrentParams->BuildingActorToEdit->GetActorForwardVector();
            */

            Rotation.Yaw = Rotation.Yaw + (CurrentParams->RotationIterations * 90);
            //printf("Yaw %f Loc X %f Loc Y %f RIterations %i", Rotation.Yaw, ForwardVector.X, ForwardVector.Y, CurrentParams->RotationIterations);


            CurrentParams->BuildingActorToEdit->SetActorHiddenInGame(true);

            CurrentParams->BuildingActorToEdit->K2_DestroyActor();
            auto Building = World->SpawnActor<ABuildingSMActor>(Location, Rotation, CurrentParams->NewBuildingClass);
            Building->bMirrored = CurrentParams->bMirrored;
            Building->InitializeKismetSpawnedBuildingActor(Building, PlayerController);
            Building->bMirrored = CurrentParams->bMirrored;
            Building->OnRep_CustomState();
            Building->Team = ((AFortPlayerStateAthena*)PlayerController->PlayerState)->TeamIndex;
            Building->OnRep_Team();
        }
    }

    if (FuncName.contains("ServerSpawnDeco"))
    {
		auto FortDeco = (AFortDecoTool*)Object;
        auto CurrentParams = (AFortDecoTool_ServerSpawnDeco_Params*)Params; 
        auto Owner = (APlayerPawn_Athena_C*)FortDeco->GetOwner();
        auto PC = (AFortPlayerControllerAthena*)Owner->Controller;


        auto Trap = (UFortTrapItemDefinition*)FortDeco->ItemDefinition;
		auto Deco = World->SpawnActor<ABuildingTrap>(CurrentParams->Location, CurrentParams->Rotation, Trap->GetBlueprintClass());
        Deco->AttachedTo = CurrentParams->AttachedActor;
        Deco->OnRep_AttachedTo();
        Deco->Team = ((AFortPlayerStateAthena*)Owner->PlayerState)->TeamIndex;
        Deco->OnRep_Team();
        Deco->OnInitTeam();
        Deco->OnPlaced();
        Deco->OnFinishedBuilding();
        Deco->InitializeKismetSpawnedBuildingActor(Deco, PC);
    }

    if (FuncName.contains("ServerExecuteInventoryItem"))
    {
        auto PlayerController = (AFortPlayerControllerAthena*)Object;
        auto CurrentParams = (AFortPlayerController_ServerExecuteInventoryItem_Params*)Params;

        auto Inventory = PlayerController->WorldInventory;
        auto Pawn = (AFortPlayerPawnAthena*)PlayerController->Pawn;

        if (!Inventory || !Pawn)
            return ProcessEvent(Object, Function, Params);

        for (int i = 0; i < Inventory->Inventory.ItemInstances.Num(); i++)
        {
            auto ItemInstance = Inventory->Inventory.ItemInstances[i];
            auto ItemGuid = ItemInstance->GetItemGuid();

            if (Utils::GuidComp(ItemGuid, CurrentParams->ItemGuid))
            {
                auto ItemDef = (UFortWeaponItemDefinition*)ItemInstance->GetItemDefinitionBP();

                if (ItemInstance->ItemEntry.ItemDefinition->GetName().contains("TID"))
                {
                    auto TrapCasted = (UFortTrapItemDefinition*)ItemDef;
					auto Trap = World->SpawnActor<ABuildingTrap>(Pawn->K2_GetActorLocation(), Pawn->K2_GetActorRotation());
                    Pawn->PickUpActor(Trap, TrapCasted);
                }
                else
                {
                    auto Weapon = Pawn->EquipWeaponDefinition(ItemDef, ItemGuid);
                    if (Weapon)
                    {
                        Weapon->SetOwner(Pawn);
                        Weapon->OnRep_Owner();
                        Weapon->ClientGivenTo(Pawn);
                        Pawn->ClientInternalEquipWeapon(Weapon);
                        Pawn->ServerInternalEquipWeapon(Weapon);
                        Pawn->CurrentWeapon = Weapon;
                        Pawn->OnRep_CurrentWeapon();
                    }
                }
            }
        }

    }

    if (FuncName == "ServerTryActivateAbility")
    {
        auto ServerParams = (UAbilitySystemComponent_ServerTryActivateAbility_Params*)Params;
        auto AbilityComp = (UAbilitySystemComponent*)Object;

        UGameplayAbility* InstancedAbility = nullptr;

        auto FoundSpec = ServerFunctions::FindAbilitySpecFromHandle(AbilityComp, ServerParams->AbilityToActivate);

        if (FoundSpec)
        {
            printf("Trying to Activate Ability %s\n", FoundSpec->Ability->GetName().c_str());
			
            if (!InternalTryActivateAbility(AbilityComp, ServerParams->AbilityToActivate, ServerParams->PredictionKey, &InstancedAbility, nullptr, nullptr))
            {
               AbilityComp->ClientActivateAbilityFailed(ServerParams->AbilityToActivate, ServerParams->PredictionKey.Current);
            }
        }

        return NULL;
    }
	
    if (FuncName == "ServerChoosePart")
    {
        auto CurrentParams = (AFortPlayerPawn_ServerChoosePart_Params*)Params;

		if (CurrentParams->ChosenCharacterPart == nullptr)
        {
            return NULL;
        }
    }


    if (FuncName == "ServerTryActivateAbilityWithEventData")
    {
        auto ServerParams = (UAbilitySystemComponent_ServerTryActivateAbilityWithEventData_Params*)Params;
        auto AbilityComp = (UFortAbilitySystemComponent*)Object;

        UGameplayAbility* InstancedAbility = nullptr;

        auto FoundSpec = ServerFunctions::FindAbilitySpecFromHandle(AbilityComp, ServerParams->AbilityToActivate);

        if (FoundSpec)
        {
            printf("Trying to Activate Ability %s\n", FoundSpec->Ability->GetName().c_str());

            if (!InternalTryActivateAbility(AbilityComp, ServerParams->AbilityToActivate, ServerParams->PredictionKey, &InstancedAbility, nullptr, nullptr))
            {
                AbilityComp->ClientActivateAbilityFailed(ServerParams->AbilityToActivate, ServerParams->PredictionKey.Current);
            }
        }

        return NULL;
    }

    if (FuncName.contains("ReceiveDestroyed"))
    {
        auto Actor = (AActor*)Object;

        if (World && World->NetDriver)
        {
            for (int i = 0; i < World->NetDriver->ClientConnections.Num(); i++)
            {
                auto Connection = World->NetDriver->ClientConnections[i];
                auto Channel = FindChannel(Actor, Connection);

                if (Channel)
                    CloseActorChannel(Channel);
            }
        }
    }

    if (FuncName.contains("BP_PlayButton"))
    {
        Globals::GEngine->GameInstance->LocalPlayers[0]->PlayerController->SwitchLevel(L"Athena_Terrain?game=athena");
        std::cout << "Switched!\n";
    }

    if (FuncName == "ReadyToStartMatch")
    {
        if (!Globals::GEngine->GameViewport->World->GetName().contains("Frontend"))
        {
            static bool bReady = false;

            if (!bReady)
            {
                bReady = true;
                World = Globals::GEngine->GameViewport->World;
                Globals::Beacon = World->SpawnActor<AFortOnlineBeaconHost>({}, {});
                Globals::Beacon->ListenPort = 7777;
                if (!InitHost(Globals::Beacon))
                {
                    MessageBoxA(0, "Failed to initialize Beacon!", "SkiddedGS", MB_ICONERROR);
                    return ProcessEvent(Object, Function, Params);
                }

                CurrentNetDriver = Globals::Beacon->NetDriver;
                CurrentNetDriver->NetDriverName = FName{ 282, 0 };
                Globals::Beacon->NetDriver->World = World;
                World->LevelCollections[0].NetDriver = Globals::Beacon->NetDriver;
                World->LevelCollections[1].NetDriver = Globals::Beacon->NetDriver;
                auto BaseAddr = (uintptr_t)GetModuleHandle(0);

                auto NCMWorld = (PVOID)Memory::FindPattern("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 45 33 F6 49 8B D9 44 89 74 24");
                NotifyControlMessage = decltype(NotifyControlMessage)(NCMWorld);

                auto TickFlushAddr = (PVOID)Memory::FindPattern("4C 8B DC 55 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 89 5B 18 48 8D 05 ? ? ? ? 49 89 7B E8 48 8B F9 4D 89 63 E0 45 33 E4 44 38 25 ? ? ? ? 41 0F 29 73 ? 0F 28 F1 49 0F 45 C4 41 0F 29 7B ? 48 89 45 18 48 C7 45 ? ? ? ? ? 48 8B 45 80 48 39 81 ? ? ? ? ");

                MH_CreateHook(TickFlushAddr, TickFlushHook, (void**)&TickFlush);
                MH_EnableHook(TickFlushAddr);
                auto NCMBeaconAddr = (PVOID)Memory::FindPattern("40 55 53 56 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 33 FF 48 89 4C 24 ? 89 7C 24 60 49 8B F1 48 8B 41 10 45 0F B6 E0 4C 8B F2");
                MH_CreateHook(NCMBeaconAddr, NotifyControlMessageHook, nullptr);
                MH_EnableHook(NCMBeaconAddr);
                auto SpawnPlayActorAddr = (PVOID)Memory::FindPattern("44 89 44 24 ? 48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 33 F6 48 8D 05 ? ? ? ? 89 75 67");
                
                MH_CreateHook(SpawnPlayActorAddr, SpawnPlayActorHook, (void**)&SpawnPlayActor);
                MH_EnableHook(SpawnPlayActorAddr);
				
                auto KickPatch1Addr = (PVOID)Memory::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 57 41 56 41 57 48 81 EC ? ? ? ? 48 8B 01 49 8B E9 45 0F B6 F8 4C 8B F2 48 8B F9 B3 01 FF 90 ? ? ? ?");

                if (KickPatch1Addr)
                {
                    MH_CreateHook(KickPatch1Addr, KickPatchHook, nullptr);
                    MH_EnableHook(KickPatch1Addr);
                }

                auto KickPatch2Addr = (PVOID)Memory::FindPattern("");

                if (KickPatch2Addr)
                {
                    MH_CreateHook(KickPatch2Addr, KickPatch2Hook, nullptr);
                    MH_EnableHook(KickPatch2Addr);
                }

                auto KickPatch3Addr = (PVOID)Memory::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 49 8B F0 48 8B DA 48 85 D2 74 78 48 8B BA ? ? ? ? 48 85 FF 74 6C E8 ? ? ? ? 48 8B 57 10 4C 8D 88 ? ? ? ? 49 63 41 08");

                if (KickPatch3Addr)
                {
                    MH_CreateHook(KickPatch3Addr, KickPatch3Hook, nullptr);
                    MH_EnableHook(KickPatch3Addr);
                }

                auto GameState = (AFortGameStateAthena*)World->GameState;

                std::cout << "AAAQA\n\n\n" << '\n';
                 
                std::this_thread::sleep_for(std::chrono::seconds(2));

                static auto Playlist = Utils::FindObjectFast<UFortPlaylistAthena>("/Game/Athena/Playlists/Blitz/Playlist_Blitz_Solo.Playlist_Blitz_Solo");
                if (Playlist)
                {
                    GameState->CurrentPlaylistData = Playlist;
                    GameState->OnRep_CurrentPlaylistData();
                }

                GameState->GamePhase = EAthenaGamePhase::None;
                GameState->OnRep_GamePhase(EAthenaGamePhase::Setup);
                /* GameState->bAircraftIsLocked = false;
                GameState->bGameModeWillSkipAircraft = true;
                GameState->PlayersLeft--;
                GameState->TotalPlayers--;
                GameState->OnRep_PlayersLeft(); */

                World->AuthorityGameMode->GameSession->MaxPlayers = 100;

                // UKismetSystemLibrary::ExecuteConsoleCommand(World, L"log LogAbilitySystem Log", Globals::GEngine->GameInstance->LocalPlayers[0]->PlayerController);

                Globals::Beacon->BeaconState = EBeaconState::AllowRequests;
				
                // CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ServerFunctions::SetIsReady, 0, 0, 0);

                World->NetworkManager->NetCullDistanceSquared += (World->NetworkManager->NetCullDistanceSquared * 2.5);
            }
        }
    }

    return ProcessEvent(Object, Function, Params);
}

static char CalculatePingHook(__int64 a1)
{
    return 1;
}


static __int64 DestroyActorHook(AActor* Actor, unsigned __int8 a2, unsigned __int8 a3)
{
    static auto GameState = (AFortGameStateAthena*)World->GameState;

    if (World && World->NetDriver)
    {
        for (int i = 0; i < World->NetDriver->ClientConnections.Num(); i++)
        {
            auto Connection = World->NetDriver->ClientConnections[i];
            auto Channel = FindChannel(Actor, Connection);

            if (Channel)
                CloseActorChannel(Channel);
        }
    }

    return DestroyActor(Actor, a2, a3);
}

bool DestroySwappedPCHook(UWorld*, UNetConnection* Connection)
{
    return DestroySwappedPC(World, Connection);
}

void* SeamlessTravelHandlerForWorldHook(UEngine* Engine, UWorld*)
{
    return SeamlessTravelHandlerForWorld(Engine, World);
}

void* OnReloadHook(AFortWeapon* a1, int a2)
{
    auto Pawn = (AFortPlayerPawnAthena*)a1->GetOwner();
	auto PlayerController = (AFortPlayerControllerAthena*)Pawn->Controller;

    bool Successful = true;
	
    if (a1 && Successful)
    {
        auto AmmoDef = a1->WeaponData->GetAmmoWorldItemDefinition_BP();

        if (!AmmoDef || a1->WeaponData->GetName().contains("TID"))
            AmmoDef = a1->WeaponData;

        auto Inventory = PlayerController->WorldInventory;

        auto ReplicatedEntries = Inventory->Inventory.ReplicatedEntries;
        auto ItemInstances = Inventory->Inventory.ItemInstances;

        for (int i = 0; i < Inventory->Inventory.ReplicatedEntries.Num(); i++)
        {
            if (Inventory->Inventory.ReplicatedEntries[i].ItemDefinition == AmmoDef)
            {
                Inventory->Inventory.ReplicatedEntries[i].Count -= a2;
                Inventory->Inventory.ReplicatedEntries[i].ReplicationKey++;

                if (Inventory->Inventory.ReplicatedEntries[i].Count <= 0)
                {
                    Inventory->Inventory.ReplicatedEntries.RemoveSingle(i);

                    for (int j = 0; j < ItemInstances.Num(); j++)
                    {
                        auto ItemInstance = ItemInstances[j];

                        if (ItemInstance && ItemInstance->GetItemDefinitionBP() == AmmoDef)
                        {
                            ItemInstances.RemoveSingle(i);
                        }
                    }
                }

                ServerFunctions::UpdateInventory(PlayerController);
            }
        }
		
		return OnReload(a1, a2);
    }
}

static void Main()
{
    AllocConsole();
    FILE* Handle;
    freopen_s(&Handle, "CONOUT$", "w", stdout);

    auto BaseAddr = (uintptr_t)GetModuleHandle(0);
    auto GObjectsAddress = Memory::FindPattern("48 8B 05 ? ? ? ? 48 8D 1C C8 81 4B ? ? ? ? ? 49 63 76 30", true, 3);
    auto FNameToStringAddress = Memory::FindPattern("48 89 5C 24 ? 57 48 83 EC 40 83 79 04 00 48 8B DA 48 8B F9");
    auto FreeMemoryAddress = Memory::FindPattern("48 85 C9 74 1D 4C 8B 05 ? ? ? ? 4D 85 C0");
    auto AddNetworkActorAddress = Memory::FindPattern("48 85 D2 0F 84 ? ? ? ? 48 89 5C 24 ? 56 48 83 EC 20 48 63 42 0C 48 8B F2 3B 05 ? ? ? ?");
    auto ActorChannelCloseAddress = Memory::FindPattern("48 89 5C 24 ? 55 57 41 57 48 83 EC 50 33 FF 48 8B D9 89 7C 24 70 80 3D ? ? ? ? ? 44 8D 7F 01 0F 82 ? ? ? ? 48 8B 49 68 48 85 C9 74 26 45 33 C0 48 8D 54 24 ?");
    auto NotifyActorDestroyedAddress = Memory::FindPattern("E8 ? ? ? ? 48 8B 5D 20 48 85 DB 74 55");
    auto DestroySwappedPCAddress = Memory::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 30 C7 44 24");
    auto SeamlessTravelHandlerForWorldAddress = Memory::FindPattern("48 83 EC 28 48 8B 81 ? ? ? ? 48 63 89 ? ? ? ? 48 8D 0C C8 48 3B C1 74 1A 0F 1F 44 00 ? 4C 8B 00 49 39 90 ? ? ? ? 74 17 48 83 C0 08 48 3B C1 75 EB E8 ? ? ? ?");
    auto ReAllocAddress = Memory::FindPattern("4C 8B D1 48 8B 0D ? ? ? ? 48 85 C9 75 08 49 8B CA E9 ? ? ? ? 48 8B 01 45 8B C8 4C 8B C2 49 8B D2 48 FF 60 18");
    auto MallocAddress = Memory::FindPattern("4C 8B C9 48 8B 0D ? ? ? ? 48 85 C9 75 08 49 8B C9 E9 ? ? ? ?");
    auto FindAbilitySpecFromHandleAddress = Memory::FindPattern("40 53 48 83 EC 20 48 8B 99 ? ? ? ? 48 8D 05 ? ? ? ? 44 0F B6 0D ? ? ? ? 45 33 D2 48 63 89 ? ? ? ? 45 84 C9 49 0F 45 C2 4C 69 C1 ? ? ? ? 48 89 44 24 ? 4C 03 C3 49 3B D8");
    auto OnGamepadAbilityReleaseAddress = Memory::FindPattern("40 55 53 56 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 30 48 8B D9 48 63 FA 48 8B 89 ? ? ? ?");
    auto OnReloadAddr = Memory::FindPattern("89 54 24 10 55 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 80 B9 ? ? ? ? ? 4C 8B F1 0F 85 ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 84 C0 0F 84 ? ? ? ?");
    auto TestReload = Memory::FindPattern("48 89 5C 24 ? 57 48 83 EC 20 48 8B F9 E8 ? ? ? ? 48 8B D8 48 85 C0 74 3A E8");
    auto WeaponInitlized = Memory::FindPattern("48 89 5C 24 ? 57 48 83 EC 30 80 B9 ? ? ? ? ? 48 8B FA 48 8B D9 75 43 48 85 D2 74 3E 48 8B CA E8 ? ? ? ? 48 85 C0");
    auto InitHostAddr = Memory::FindPattern("48 8B C4 48 81 EC ? ? ? ? 48 89 58 18 4C 8D 05 ? ? ? ? 48 8B D9 48 89 78 F8 48 8D 48 88");
    auto SetWorldAddr = Memory::FindPattern("");
    auto WelcomePlayerAddr = Memory::FindPattern("48 8B C4 55 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 48 89 70 20 48 8B F1 48 89 78 F0 48 8B CA 4C 89 70 E8 ");
    auto CreateChannelAddr = Memory::FindPattern("40 56 57 41 54 41 55 41 57 48 83 EC 60 48 8B 01 41 8B F9 45 0F B6 E0 4C 63 FA 48 8B F1 FF 90 ? ? ? ? 45 33 ED 83 FF FF 0F 85 ? ? ? ?");
    auto SetChannelActorAddr = Memory::FindPattern("48 8B C4 55 53 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 48 89 70 10 48 8B D9 48 89 78 18");
    auto ReplicateActorAddr = Memory::FindPattern("48 8B C4 48 89 48 08 55 53 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 A8 0F 29 78 98 48 89 70 E8 4C 89 60 D8");
    auto CallPreReplicationAddr = Memory::FindPattern("48 85 D2 0F 84 ? ? ? ? 56 41 56 48 83 EC 38 4C 8B F2 48 89 5C 24 ? 48 89 6C 24 ? 48 8D 54 24 ? 48 8B F1");
    auto ReceiveFStringAddr = Memory::FindPattern("40 55 53 56 57 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 27 0F B6 41 28 48 8B F2 48 8B F9 A8 01 ");
    auto ReceiveUniqueIdAddr = Memory::FindPattern("48 89 5C 24 ? 55 56 57 48 8B EC 48 83 EC 40 F6 41 28 40 48 8B FA 48 8B D9 0F 84 ? ? ? ? F6 41 2B 02");
    auto SendClientAdjustmentAddr = Memory::FindPattern("40 53 48 83 EC 20 48 8B 99 ? ? ? ? 48 39 99 ? ? ? ? 74 0A 48 83 B9 ? ? ? ? ? 74 78 48 85 DB 75 0C 48 8B 99 ? ? ? ?");
    auto IsNetRelevantForAddr = Memory::FindPattern("48 89 5C 24 ? 56 57 41 56 48 83 EC 70 48 8B FA 4D 8B F1 0F B6 91 ? ? ? ? 49 8B D8 48 8B F1 F6 C2 10 0F 85 ? ? ? ? 48 8B C1 0F 1F 40 00");
    auto InternalTryActivateAbilityAddr = Memory::FindPattern("4C 89 4C 24 ? 4C 89 44 24 ? 89 54 24 10 55 53 56 57 41 54 41 57");

    srand(time(0));

    UObject::GObjects = decltype(SDK::UObject::GObjects)(GObjectsAddress);
    FNameToString = decltype(SDK::FNameToString)(FNameToStringAddress);
    FreeMemory = decltype(SDK::FreeMemory)(FreeMemoryAddress);
    ReallocMemory = decltype(ReallocMemory)(ReAllocAddress);
    MallocMemory = decltype(MallocMemory)(MallocAddress);
    InitHost = decltype(InitHost)(InitHostAddr);
    WelcomePlayer = decltype(WelcomePlayer)(WelcomePlayerAddr);
    CreateChannel = decltype(CreateChannel)(CreateChannelAddr);
    SetChannelActor = decltype(SetChannelActor)(SetChannelActorAddr);
    ReplicateActor = decltype(ReplicateActor)(ReplicateActorAddr);
    CallPreReplication = decltype(CallPreReplication)(CallPreReplicationAddr);
    ReceiveFString = decltype(ReceiveFString)(ReceiveFStringAddr);
    ReceiveUniqueIdRepl = decltype(ReceiveUniqueIdRepl)(ReceiveUniqueIdAddr);
    SendClientAdjustment = decltype(SendClientAdjustment)(SendClientAdjustmentAddr);
    ServerFunctions::MarkArrayDirty = decltype(ServerFunctions::MarkArrayDirty)(Memory::FindPattern("40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 8B 43 48 45 33 DB 85 C0 7E 4A 4C 8D 4B 38 45 8B C3 44 8B D0 0F 1F 40 00 66 66 0F 1F 84 00 ? ? ? ? 49 8B 41 08 48 F7 D8 8B 43 48 48 1B D2 FF C8 48 F7 DA 48 63 C8 48 FF CA 49 23 C8 49 23 D1 49 FF C0 49 0B 51 08 C7 04 8A ? ? ? ? 49 83 EA 01 75 CE FF 43 54 8B 43 54 83 F8 FF 48 C7 83 ? ? ? ? ? ? ? ? 41 0F 44 C3 89 43 54 48 83 C4 20 5B C3"));
    IsNetRelevantFor = decltype(IsNetRelevantFor)(IsNetRelevantForAddr);
    CloseActorChannel = decltype(CloseActorChannel)(ActorChannelCloseAddress);
    SupportsObject = decltype(SupportsObject)(Memory::FindPattern("48 89 5C 24 ? 57 48 83 EC 40 48 8B DA 48 8B F9 48 85 D2 75 0D B0 01 48 8B 5C 24 ? 48 83 C4 40 5F C3 48 8D 4C 24 ? 4D 85 C0 74"));
    InternalTryActivateAbility = decltype(InternalTryActivateAbility)(InternalTryActivateAbilityAddr);
    StaticLoadObject = decltype(StaticLoadObject)(Memory::FindPattern("4C 89 4C 24 ? 48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 33 D2 48 8D 05 ? ? ? ? 38 15 ? ? ? ? 49 8B F0 8B CA 48 89 55 FF 48 0F 45 C2 48 89 55 07 48 83 CF FF 48 89 45 E7 4D 85 C0 74 39 66 41 39 10"));
    ServerFunctions::MarkAbilitySpecDirty = decltype(ServerFunctions::MarkAbilitySpecDirty)(Memory::FindPattern("48 89 5C 24 ? 57 48 83 EC 20 80 B9 ? ? ? ? ? 48 8B FA 48 8B D9 75 4A C6 81 ? ? ? ? ? 48 81 C1 ? ? ? ? 83 3A FF 75 16 FF 41 50 8B 41 50 89 02 33 D2 8B 41 50 83 F8 FF 0F 44 C2 89 41 50"));
    FindAbilitySpecFromHandle = decltype(FindAbilitySpecFromHandle)(FindAbilitySpecFromHandleAddress);
    ServerFunctions::StaticConstructObjectInternal = decltype(ServerFunctions::StaticConstructObjectInternal)(Memory::FindPattern("4C 89 44 24 ? 53 55 56 57 41 54 41 56 41 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 44 8B A4 24 ? ? ? ? 48 8D 05 ? ? ? ? 48 8B BC 24 ? ? ? ? 48 8B F1 44 0F B6 B4 24 ? ? ? ? 33 C9"));
    EquipWeaponDefinition = decltype(EquipWeaponDefinition)(Memory::FindPattern("40 55 56 57 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 07 80 B9 ? ? ? ? ?"));
    TryActivateAbility = decltype(TryActivateAbility)(Memory::FindPattern("48 8B C4 89 50 10 55 53 48 8D 68 A1 48 81 EC ? ? ? ? 48 89 70 08 48 8B D9 48 89 78 E8"));
    WeaponInit = decltype(WeaponInit)(WeaponInitlized);

    Globals::GEngine = UObject::FindObject<UGameEngine>("FortEngine_");
    Globals::GEngine->GameViewport->ViewportConsole = (UConsole*)UGameplayStatics::SpawnObject(UConsole::StaticClass(), Globals::GEngine->GameViewport);

    World = Globals::GEngine->GameViewport->World;

    if (MH_Initialize() != MH_OK)
    {
        MessageBoxA(0, "Failed to Initialize MinHook!\n", "SkiddedGS", MB_ICONERROR);
        return;
    }
	
    MH_CreateHook(Globals::GEngine->Vtable[64], ProcessEventHook, (void**)&ProcessEvent);
    MH_EnableHook(Globals::GEngine->Vtable[64]);
    // MH_CreateHook((void*)(BaseAddr + 0x5CBE60), CalculatePingHook, nullptr);
    // MH_EnableHook((void*)(BaseAddr + 0x5CBE60));
    // MH_CreateHook((void*)DestroySwappedPCAddress, DestroySwappedPCHook, (void**)&DestroySwappedPC);
    // MH_EnableHook((void*)DestroySwappedPCAddress);
    // MH_CreateHook((void*)SeamlessTravelHandlerForWorldAddress, SeamlessTravelHandlerForWorldHook, (void**)&SeamlessTravelHandlerForWorld);
    // MH_EnableHook((void*)SeamlessTravelHandlerForWorldAddress);
    MH_CreateHook((void*)(OnReloadAddr), OnReloadHook, (void**)&OnReload);
    MH_EnableHook((void*)(OnReloadAddr));
	
    auto AddNetworkActorAddr = (PVOID)Memory::FindPattern("48 85 D2 0F 84 ? ? ? ? 48 89 5C 24 ? 56 48 83 EC 20 48 63 42 0C 48 8B F2 3B 05 ? ? ? ? 48 8B D9 7D 11 4C 8D 04 40 48 8B 05");

    MH_CreateHook(AddNetworkActorAddr, AddNetworkActorHook, (void**)&AddNetworkActor);
    MH_EnableHook(AddNetworkActorAddr);

    std::cout << "Setup!\n";

    return;
}

BOOL APIENTRY DllMain(HMODULE, DWORD dwReason, LPVOID)
{
    if (dwReason == 1)
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&Main, 0, 0, 0);

    return 1;
}
    