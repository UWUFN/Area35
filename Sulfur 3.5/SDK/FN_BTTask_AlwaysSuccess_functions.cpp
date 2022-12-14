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

// Function BTTask_AlwaysSuccess.BTTask_AlwaysSuccess_C.ReceiveExecute
// (Event, Protected, BlueprintEvent)
// Parameters:
// class AActor**                 OwnerActor                     (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

void UBTTask_AlwaysSuccess_C::ReceiveExecute(class AActor** OwnerActor)
{
	static auto fn = UObject::FindObject<UFunction>("Function BTTask_AlwaysSuccess.BTTask_AlwaysSuccess_C.ReceiveExecute");

	UBTTask_AlwaysSuccess_C_ReceiveExecute_Params params;
	params.OwnerActor = OwnerActor;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function BTTask_AlwaysSuccess.BTTask_AlwaysSuccess_C.ExecuteUbergraph_BTTask_AlwaysSuccess
// ()
// Parameters:
// int                            EntryPoint                     (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

void UBTTask_AlwaysSuccess_C::ExecuteUbergraph_BTTask_AlwaysSuccess(int EntryPoint)
{
	static auto fn = UObject::FindObject<UFunction>("Function BTTask_AlwaysSuccess.BTTask_AlwaysSuccess_C.ExecuteUbergraph_BTTask_AlwaysSuccess");

	UBTTask_AlwaysSuccess_C_ExecuteUbergraph_BTTask_AlwaysSuccess_Params params;
	params.EntryPoint = EntryPoint;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
