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

// AnimBlueprintGeneratedClass F_MED_BLK_Red_Head_01_Dyn_AnimBP.F_MED_BLK_Red_Head_01_Dyn_AnimBP_C
// 0x2318 (0x2748 - 0x0430)
class UF_MED_BLK_Red_Head_01_Dyn_AnimBP_C : public UCustomCharacterPartAnimInstance
{
public:
	struct FPointerToUberGraphFrame                    UberGraphFrame;                                           // 0x0430(0x0008) (Transient, DuplicateTransient)
	struct FAnimNode_SubInput                          AnimGraphNode_SubInput_105AE3D84E770FFB047C23A6846B2248;  // 0x0438(0x0068)
	struct FAnimNode_ConvertLocalToComponentSpace      AnimGraphNode_LocalToComponentSpace_CDCE9EC14E6EC7B9241FD2B3A3177508;// 0x04A0(0x0048)
	struct FAnimNode_Root                              AnimGraphNode_Root_6A7437884A430A55C809AC89486D8BB6;      // 0x04E8(0x0048)
	struct FAnimNode_ConvertComponentToLocalSpace      AnimGraphNode_ComponentToLocalSpace_9D7914B445705F63BD6E75B42EB514D9;// 0x0530(0x0048)
	unsigned char                                      UnknownData00[0x8];                                       // 0x0578(0x0008) MISSED OFFSET
	struct FAnimNode_AnimDynamics                      AnimGraphNode_AnimDynamics_7FE3E5BC49E159EFE83DB1B3F4D870EA;// 0x0580(0x0360)
	struct FAnimNode_AnimDynamics                      AnimGraphNode_AnimDynamics_81C70A20464E57C4CE7098B3F482B33E;// 0x08E0(0x0360)
	struct FAnimNode_AnimDynamics                      AnimGraphNode_AnimDynamics_B2D6D8EF4E38002542F705A2474DBD8F;// 0x0C40(0x0360)
	struct FAnimNode_AnimDynamics                      AnimGraphNode_AnimDynamics_1F5B5CFA479F9E280FF0F8A2DB030C2A;// 0x0FA0(0x0360)
	struct FAnimNode_AnimDynamics                      AnimGraphNode_AnimDynamics_0559BE1D4A2FC8460697968C3974EEFA;// 0x1300(0x0360)
	struct FAnimNode_AnimDynamics                      AnimGraphNode_AnimDynamics_F6A1231D401E85CE451BEFB61FE1DBB9;// 0x1660(0x0360)
	struct FAnimNode_AnimDynamics                      AnimGraphNode_AnimDynamics_45D664C64E3D960A1AC8C89C9D086677;// 0x19C0(0x0360)
	struct FAnimNode_AnimDynamics                      AnimGraphNode_AnimDynamics_ED808006428A300EF4B8B39642F8AAAA;// 0x1D20(0x0360)
	struct FAnimNode_AnimDynamics                      AnimGraphNode_AnimDynamics_A32DC5B4430CACD6121E80A70990E75B;// 0x2080(0x0360)
	struct FAnimNode_AnimDynamics                      AnimGraphNode_AnimDynamics_6E4E7A0F46BBEF23900039AC5B0441D8;// 0x23E0(0x0360)
	class USkeletalMeshComponent*                      MeshToCopy;                                               // 0x2740(0x0008) (Edit, BlueprintVisible, ZeroConstructor, DisableEditOnInstance, InstancedReference, IsPlainOldData)

	static UClass* StaticClass()
	{
		static auto ptr = UObject::FindClass("AnimBlueprintGeneratedClass F_MED_BLK_Red_Head_01_Dyn_AnimBP.F_MED_BLK_Red_Head_01_Dyn_AnimBP_C");
		return ptr;
	}


	void BlueprintInitializeAnimation();
	void ExecuteUbergraph_F_MED_BLK_Red_Head_01_Dyn_AnimBP(int EntryPoint);
};


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
