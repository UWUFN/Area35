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

// BlueprintGeneratedClass B_EmptyBaseGlider.B_EmptyBaseGlider_C
// 0x0001 (0x0361 - 0x0360)
class AB_EmptyBaseGlider_C : public AFortPlayerParachute
{
public:
	bool                                               FX_GliderFullyDeployed;                                   // 0x0360(0x0001) (Edit, BlueprintVisible, ZeroConstructor, DisableEditOnInstance, IsPlainOldData)

	static UClass* StaticClass()
	{
		static auto ptr = UObject::FindClass("BlueprintGeneratedClass B_EmptyBaseGlider.B_EmptyBaseGlider_C");
		return ptr;
	}


	void UserConstructionScript();
};


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
