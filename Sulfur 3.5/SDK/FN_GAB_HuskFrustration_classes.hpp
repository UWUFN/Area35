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

// BlueprintGeneratedClass GAB_HuskFrustration.GAB_HuskFrustration_C
// 0x0008 (0x0AF8 - 0x0AF0)
class UGAB_HuskFrustration_C : public UFortGameplayAbility
{
public:
	struct FPointerToUberGraphFrame                    UberGraphFrame;                                           // 0x0AF0(0x0008) (Transient, DuplicateTransient)

	static UClass* StaticClass()
	{
		static auto ptr = UObject::FindClass("BlueprintGeneratedClass GAB_HuskFrustration.GAB_HuskFrustration_C");
		return ptr;
	}


	void K2_ActivateAbility();
	void ExecuteUbergraph_GAB_HuskFrustration(int EntryPoint);
};


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
