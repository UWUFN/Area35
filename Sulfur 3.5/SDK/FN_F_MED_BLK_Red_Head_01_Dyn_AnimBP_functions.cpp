// Fortnite (7.00) SDK

#ifdef _MSC_VER
	#pragma pack(push, 0x8)
#endif

#include "../SDK.hpp"

namespace SDK
{
//---------------------------------------------------------------------------
//Functions
//---------------------------------------------------------------------------

// Function F_MED_BLK_Red_Head_01_Dyn_AnimBP.F_MED_BLK_Red_Head_01_Dyn_AnimBP_C.BlueprintInitializeAnimation
// (Event, Public, BlueprintEvent)

void UF_MED_BLK_Red_Head_01_Dyn_AnimBP_C::BlueprintInitializeAnimation()
{
	static auto fn = UObject::FindObject<UFunction>("Function F_MED_BLK_Red_Head_01_Dyn_AnimBP.F_MED_BLK_Red_Head_01_Dyn_AnimBP_C.BlueprintInitializeAnimation");

	UF_MED_BLK_Red_Head_01_Dyn_AnimBP_C_BlueprintInitializeAnimation_Params params;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function F_MED_BLK_Red_Head_01_Dyn_AnimBP.F_MED_BLK_Red_Head_01_Dyn_AnimBP_C.ExecuteUbergraph_F_MED_BLK_Red_Head_01_Dyn_AnimBP
// ()
// Parameters:
// int                            EntryPoint                     (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

void UF_MED_BLK_Red_Head_01_Dyn_AnimBP_C::ExecuteUbergraph_F_MED_BLK_Red_Head_01_Dyn_AnimBP(int EntryPoint)
{
	static auto fn = UObject::FindObject<UFunction>("Function F_MED_BLK_Red_Head_01_Dyn_AnimBP.F_MED_BLK_Red_Head_01_Dyn_AnimBP_C.ExecuteUbergraph_F_MED_BLK_Red_Head_01_Dyn_AnimBP");

	UF_MED_BLK_Red_Head_01_Dyn_AnimBP_C_ExecuteUbergraph_F_MED_BLK_Red_Head_01_Dyn_AnimBP_Params params;
	params.EntryPoint = EntryPoint;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
