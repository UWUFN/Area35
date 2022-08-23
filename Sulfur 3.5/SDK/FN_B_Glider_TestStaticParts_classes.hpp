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

// BlueprintGeneratedClass B_Glider_TestStaticParts.B_Glider_TestStaticParts_C
// 0x0008 (0x0368 - 0x0360)
class AB_Glider_TestStaticParts_C : public AFortPlayerParachute
{
public:
	class UStaticMeshComponent*                        GliderStaticParts;                                        // 0x0360(0x0008) (BlueprintVisible, ZeroConstructor, InstancedReference, IsPlainOldData)

	static UClass* StaticClass()
	{
		static auto ptr = UObject::FindClass("BlueprintGeneratedClass B_Glider_TestStaticParts.B_Glider_TestStaticParts_C");
		return ptr;
	}


	void UserConstructionScript();
};


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
