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

// Function NetDebugUI.NetDebugUI_C.Construct
// (BlueprintCosmetic, Event, Public, BlueprintEvent)

void UNetDebugUI_C::Construct()
{
	static auto fn = UObject::FindObject<UFunction>("Function NetDebugUI.NetDebugUI_C.Construct");

	UNetDebugUI_C_Construct_Params params;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function NetDebugUI.NetDebugUI_C.ExecuteUbergraph_NetDebugUI
// ()
// Parameters:
// int                            EntryPoint                     (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

void UNetDebugUI_C::ExecuteUbergraph_NetDebugUI(int EntryPoint)
{
	static auto fn = UObject::FindObject<UFunction>("Function NetDebugUI.NetDebugUI_C.ExecuteUbergraph_NetDebugUI");

	UNetDebugUI_C_ExecuteUbergraph_NetDebugUI_Params params;
	params.EntryPoint = EntryPoint;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
