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

// BlueprintGeneratedClass FrontendCamera_Main.FrontendCamera_Main_C
// 0x0010 (0x08C0 - 0x08B0)
class AFrontendCamera_Main_C : public AFortCameraBase
{
public:
	struct FPointerToUberGraphFrame                    UberGraphFrame;                                           // 0x08B0(0x0008) (Transient, DuplicateTransient)
	class UCameraComponent*                            LoginCamera_0_1;                                          // 0x08B8(0x0008) (BlueprintVisible, ZeroConstructor, InstancedReference, IsPlainOldData)

	static UClass* StaticClass()
	{
		static auto ptr = UObject::FindClass("BlueprintGeneratedClass FrontendCamera_Main.FrontendCamera_Main_C");
		return ptr;
	}


	void UserConstructionScript();
	void OnActivated();
	void ExecuteUbergraph_FrontendCamera_Main(int EntryPoint);
};


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
