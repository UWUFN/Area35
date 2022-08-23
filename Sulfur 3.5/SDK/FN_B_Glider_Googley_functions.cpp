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

// Function B_Glider_Googley.B_Glider_Googley_C.UserConstructionScript
// (Event, Public, BlueprintCallable, BlueprintEvent)

void AB_Glider_Googley_C::UserConstructionScript()
{
	static auto fn = UObject::FindObject<UFunction>("Function B_Glider_Googley.B_Glider_Googley_C.UserConstructionScript");

	AB_Glider_Googley_C_UserConstructionScript_Params params;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function B_Glider_Googley.B_Glider_Googley_C.ReceiveBeginPlay
// (Event, Protected, BlueprintEvent)

void AB_Glider_Googley_C::ReceiveBeginPlay()
{
	static auto fn = UObject::FindObject<UFunction>("Function B_Glider_Googley.B_Glider_Googley_C.ReceiveBeginPlay");

	AB_Glider_Googley_C_ReceiveBeginPlay_Params params;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function B_Glider_Googley.B_Glider_Googley_C.FireDynamics
// (BlueprintCallable, BlueprintEvent)

void AB_Glider_Googley_C::FireDynamics()
{
	static auto fn = UObject::FindObject<UFunction>("Function B_Glider_Googley.B_Glider_Googley_C.FireDynamics");

	AB_Glider_Googley_C_FireDynamics_Params params;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function B_Glider_Googley.B_Glider_Googley_C.ExecuteUbergraph_B_Glider_Googley
// ()
// Parameters:
// int                            EntryPoint                     (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

void AB_Glider_Googley_C::ExecuteUbergraph_B_Glider_Googley(int EntryPoint)
{
	static auto fn = UObject::FindObject<UFunction>("Function B_Glider_Googley.B_Glider_Googley_C.ExecuteUbergraph_B_Glider_Googley");

	AB_Glider_Googley_C_ExecuteUbergraph_B_Glider_Googley_Params params;
	params.EntryPoint = EntryPoint;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
