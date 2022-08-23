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

// Function SotoMenu.SotoMenu_C.EquiptBackPack
struct USotoMenu_C_EquiptBackPack_Params
{
	struct FString                                     Selection;                                                // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor)
};

// Function SotoMenu.SotoMenu_C.ServerChoostBackPack
struct USotoMenu_C_ServerChoostBackPack_Params
{
	class UCustomCharacterPart*                        ChosenCharacterPart;                                      // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
};

// Function SotoMenu.SotoMenu_C.EquiptHat
struct USotoMenu_C_EquiptHat_Params
{
	struct FString                                     Selection;                                                // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor)
};

// Function SotoMenu.SotoMenu_C.ServerChooseHat
struct USotoMenu_C_ServerChooseHat_Params
{
	class UCustomCharacterPart*                        Hat;                                                      // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
};

// Function SotoMenu.SotoMenu_C.EquiptHead
struct USotoMenu_C_EquiptHead_Params
{
	struct FString                                     Selection;                                                // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor)
};

// Function SotoMenu.SotoMenu_C.ServerChooseHead
struct USotoMenu_C_ServerChooseHead_Params
{
	class UCustomCharacterPart*                        HEAD;                                                     // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
};

// Function SotoMenu.SotoMenu_C.EquiptBody
struct USotoMenu_C_EquiptBody_Params
{
	struct FString                                     Selection;                                                // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor)
};

// Function SotoMenu.SotoMenu_C.ServerChooseBody
struct USotoMenu_C_ServerChooseBody_Params
{
	class UCustomCharacterPart*                        Body;                                                     // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
};

// Function SotoMenu.SotoMenu_C.BndEvt__ComboBoxString_912_K2Node_ComponentBoundEvent_6_OnSelectionChangedEvent__DelegateSignature
struct USotoMenu_C_BndEvt__ComboBoxString_912_K2Node_ComponentBoundEvent_6_OnSelectionChangedEvent__DelegateSignature_Params
{
	struct FString                                     SelectedItem;                                             // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor)
	TEnumAsByte<ESelectInfo>                           SelectionType;                                            // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
};

// Function SotoMenu.SotoMenu_C.BndEvt__ComboBoxString_1_K2Node_ComponentBoundEvent_0_OnSelectionChangedEvent__DelegateSignature
struct USotoMenu_C_BndEvt__ComboBoxString_1_K2Node_ComponentBoundEvent_0_OnSelectionChangedEvent__DelegateSignature_Params
{
	struct FString                                     SelectedItem;                                             // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor)
	TEnumAsByte<ESelectInfo>                           SelectionType;                                            // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
};

// Function SotoMenu.SotoMenu_C.BndEvt__ComboBoxString_0_K2Node_ComponentBoundEvent_3_OnSelectionChangedEvent__DelegateSignature
struct USotoMenu_C_BndEvt__ComboBoxString_0_K2Node_ComponentBoundEvent_3_OnSelectionChangedEvent__DelegateSignature_Params
{
	struct FString                                     SelectedItem;                                             // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor)
	TEnumAsByte<ESelectInfo>                           SelectionType;                                            // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
};

// Function SotoMenu.SotoMenu_C.BndEvt__Slider_880_K2Node_ComponentBoundEvent_1_OnFloatValueChangedEvent__DelegateSignature
struct USotoMenu_C_BndEvt__Slider_880_K2Node_ComponentBoundEvent_1_OnFloatValueChangedEvent__DelegateSignature_Params
{
	float                                              Value;                                                    // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
};

// Function SotoMenu.SotoMenu_C.BndEvt__ComboBoxString_1_K2Node_ComponentBoundEvent_7_OnSelectionChangedEvent__DelegateSignature
struct USotoMenu_C_BndEvt__ComboBoxString_1_K2Node_ComponentBoundEvent_7_OnSelectionChangedEvent__DelegateSignature_Params
{
	struct FString                                     SelectedItem;                                             // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor)
	TEnumAsByte<ESelectInfo>                           SelectionType;                                            // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
};

// Function SotoMenu.SotoMenu_C.ExecuteUbergraph_SotoMenu
struct USotoMenu_C_ExecuteUbergraph_SotoMenu_Params
{
	int                                                EntryPoint;                                               // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
};

}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
