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

// BlueprintGeneratedClass GCNL_GM_OnDmgReceived_Slow.GCNL_GM_OnDmgReceived_Slow_C
// 0x0000 (0x0440 - 0x0440)
class AGCNL_GM_OnDmgReceived_Slow_C : public AFortGameplayCueNotify_Looping
{
public:

	static UClass* StaticClass()
	{
		static auto ptr = UObject::FindClass("BlueprintGeneratedClass GCNL_GM_OnDmgReceived_Slow.GCNL_GM_OnDmgReceived_Slow_C");
		return ptr;
	}


	void OnWhileActiveParticleSystemActivate(class UParticleSystemComponent** WhileActiveParticleSysComponent, struct FGameplayCueParameters* Parameters);
	void UserConstructionScript();
};


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
