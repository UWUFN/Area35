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

// BlueprintGeneratedClass B_PlayerShieldDamage_LensEffect_Direction.B_PlayerShieldDamage_LensEffect_Direction_C
// 0x0000 (0x0400 - 0x0400)
class AB_PlayerShieldDamage_LensEffect_Direction_C : public AFortEmitterCameraLensEffectDirectional
{
public:

	static UClass* StaticClass()
	{
		static auto ptr = UObject::FindClass("BlueprintGeneratedClass B_PlayerShieldDamage_LensEffect_Direction.B_PlayerShieldDamage_LensEffect_Direction_C");
		return ptr;
	}

};


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif