#pragma once

// Fortnite (7.00) SDK

#ifdef _MSC_VER
	#pragma pack(push, 0x8)
#endif

namespace SDK
{
//---------------------------------------------------------------------------
//Classes
//---------------------------------------------------------------------------

// BlueprintGeneratedClass SurvivorAnchorAvoidEnemyContext.SurvivorAnchorAvoidEnemyContext_C
// 0x0000 (0x0030 - 0x0030)
class USurvivorAnchorAvoidEnemyContext_C : public UEnvQueryContext_BlueprintBase
{
public:

	static UClass* StaticClass()
	{
		static auto ptr = UObject::FindClass("BlueprintGeneratedClass SurvivorAnchorAvoidEnemyContext.SurvivorAnchorAvoidEnemyContext_C");
		return ptr;
	}


	void ProvideSingleLocation(class UObject** QuerierObject, class AActor** QuerierActor, struct FVector* ResultingLocation);
};


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
