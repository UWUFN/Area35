#pragma once

// Fortnite (7.00) SDK

#ifdef _MSC_VER
	#pragma pack(push, 0x8)
#endif

namespace SDK
{
//---------------------------------------------------------------------------
//Enums
//---------------------------------------------------------------------------

// Enum Rejoin.ERejoinStatus
enum class ERejoinStatus : uint8_t
{
	NoMatchToRejoin                = 0,
	RejoinAvailable                = 1,
	UpdatingStatus                 = 2,
	NeedsRecheck                   = 3,
	NoMatchToRejoin_MatchEnded     = 4,
	ERejoinStatus_MAX              = 5
};



}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
