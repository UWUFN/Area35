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

// Function LinearTimecode.LinearTimecodeComponent.SetDropTimecodeFrameNumber
// (Final, Native, Static, Public, HasOutParms, BlueprintCallable)
// Parameters:
// struct FDropTimecode           Timecode                       (ConstParm, Parm, OutParm, ReferenceParm, IsPlainOldData)
// int                            FrameNumber                    (Parm, ZeroConstructor, IsPlainOldData)
// struct FDropTimecode           OutTimecode                    (Parm, OutParm, IsPlainOldData)

static void ULinearTimecodeComponent::SetDropTimecodeFrameNumber(const struct FDropTimecode& Timecode, int FrameNumber, struct FDropTimecode* OutTimecode)
{
	static auto fn = UObject::FindObject<UFunction>("Function LinearTimecode.LinearTimecodeComponent.SetDropTimecodeFrameNumber");

	ULinearTimecodeComponent_SetDropTimecodeFrameNumber_Params params;
	params.Timecode = Timecode;
	params.FrameNumber = FrameNumber;

	auto flags = fn->FunctionFlags;
	fn->FunctionFlags |= 0x400;

	static auto defaultObj = (UObject*)StaticClass();
	defaultObj->ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;

	if (OutTimecode != nullptr)
		*OutTimecode = params.OutTimecode;
}


// Function LinearTimecode.LinearTimecodeComponent.GetDropTimeCodeFrameNumber
// (Final, Native, Static, Public, HasOutParms, BlueprintCallable)
// Parameters:
// struct FDropTimecode           Timecode                       (ConstParm, Parm, OutParm, ReferenceParm, IsPlainOldData)
// int                            FrameNumber                    (Parm, OutParm, ZeroConstructor, IsPlainOldData)

static void ULinearTimecodeComponent::GetDropTimeCodeFrameNumber(const struct FDropTimecode& Timecode, int* FrameNumber)
{
	static auto fn = UObject::FindObject<UFunction>("Function LinearTimecode.LinearTimecodeComponent.GetDropTimeCodeFrameNumber");

	ULinearTimecodeComponent_GetDropTimeCodeFrameNumber_Params params;
	params.Timecode = Timecode;

	auto flags = fn->FunctionFlags;
	fn->FunctionFlags |= 0x400;

	static auto defaultObj = (UObject*)StaticClass();
	defaultObj->ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;

	if (FrameNumber != nullptr)
		*FrameNumber = params.FrameNumber;
}


// Function LinearTimecode.LinearTimecodeComponent.GetDropFrameNumber
// (Final, Native, Public, BlueprintCallable, BlueprintPure, Const)
// Parameters:
// int                            ReturnValue                    (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)

int ULinearTimecodeComponent::GetDropFrameNumber()
{
	static auto fn = UObject::FindObject<UFunction>("Function LinearTimecode.LinearTimecodeComponent.GetDropFrameNumber");

	ULinearTimecodeComponent_GetDropFrameNumber_Params params;

	auto flags = fn->FunctionFlags;
	fn->FunctionFlags |= 0x400;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;

	return params.ReturnValue;
}


// Function LinearTimecode.DropTimecodeToStringConversion.Conv_DropTimecodeToString
// (Final, Native, Static, Public, HasOutParms, BlueprintCallable, BlueprintPure)
// Parameters:
// struct FDropTimecode           InTimecode                     (ConstParm, Parm, OutParm, ReferenceParm, IsPlainOldData)
// struct FString                 ReturnValue                    (Parm, OutParm, ZeroConstructor, ReturnParm)

struct FString UDropTimecodeToStringConversion::Conv_DropTimecodeToString(const struct FDropTimecode& InTimecode)
{
	static auto fn = UObject::FindObject<UFunction>("Function LinearTimecode.DropTimecodeToStringConversion.Conv_DropTimecodeToString");

	UDropTimecodeToStringConversion_Conv_DropTimecodeToString_Params params;
	params.InTimecode = InTimecode;

	auto flags = fn->FunctionFlags;
	fn->FunctionFlags |= 0x400;

	static auto defaultObj = (UObject*)StaticClass();
	defaultObj->ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;

	return params.ReturnValue;
}


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
