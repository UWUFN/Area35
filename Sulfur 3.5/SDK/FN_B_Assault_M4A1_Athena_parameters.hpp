#pragma once

// Fortnite (7.00) SDK

#ifdef _MSC_VER
	#pragma pack(push, 0x8)
#endif

#include "../SDK.hpp"

namespace SDK
{
//---------------------------------------------------------------------------
//Parameters
//---------------------------------------------------------------------------

// Function B_Assault_M4A1_Athena.B_Assault_M4A1_Athena_C.UserConstructionScript
struct AB_Assault_M4A1_Athena_C_UserConstructionScript_Params
{
};

// Function B_Assault_M4A1_Athena.B_Assault_M4A1_Athena_C.OnSetTargeting
struct AB_Assault_M4A1_Athena_C_OnSetTargeting_Params
{
	bool*                                              bNewIsTargeting;                                          // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
};

// Function B_Assault_M4A1_Athena.B_Assault_M4A1_Athena_C.ReceiveTick
struct AB_Assault_M4A1_Athena_C_ReceiveTick_Params
{
	float*                                             DeltaSeconds;                                             // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
};

// Function B_Assault_M4A1_Athena.B_Assault_M4A1_Athena_C.ExecuteUbergraph_B_Assault_M4A1_Athena
struct AB_Assault_M4A1_Athena_C_ExecuteUbergraph_B_Assault_M4A1_Athena_Params
{
	int                                                EntryPoint;                                               // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
};

}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
