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

// BlueprintGeneratedClass AnimNotify_ToggleOwnerVisibility.AnimNotify_ToggleOwnerVisibility_C
// 0x0001 (0x0039 - 0x0038)
class UAnimNotify_ToggleOwnerVisibility_C : public UAnimNotify
{
public:
	bool                                               bHideOwner;                                               // 0x0038(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)

	static UClass* StaticClass()
	{
		static auto ptr = UObject::FindClass("BlueprintGeneratedClass AnimNotify_ToggleOwnerVisibility.AnimNotify_ToggleOwnerVisibility_C");
		return ptr;
	}


	bool Received_Notify(class USkeletalMeshComponent** MeshComp, class UAnimSequenceBase** Animation);
};


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
