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

// Function SotoMenu.SotoMenu_C.EquiptBackPack
// (Public, BlueprintCallable, BlueprintEvent)
// Parameters:
// struct FString                 Selection                      (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor)

void USotoMenu_C::EquiptBackPack(const struct FString& Selection)
{
	static auto fn = UObject::FindObject<UFunction>("Function SotoMenu.SotoMenu_C.EquiptBackPack");

	USotoMenu_C_EquiptBackPack_Params params;
	params.Selection = Selection;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function SotoMenu.SotoMenu_C.ServerChoostBackPack
// (Public, BlueprintCallable, BlueprintEvent)
// Parameters:
// class UCustomCharacterPart*    ChosenCharacterPart            (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

void USotoMenu_C::ServerChoostBackPack(class UCustomCharacterPart* ChosenCharacterPart)
{
	static auto fn = UObject::FindObject<UFunction>("Function SotoMenu.SotoMenu_C.ServerChoostBackPack");

	USotoMenu_C_ServerChoostBackPack_Params params;
	params.ChosenCharacterPart = ChosenCharacterPart;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function SotoMenu.SotoMenu_C.EquiptHat
// (Public, BlueprintCallable, BlueprintEvent)
// Parameters:
// struct FString                 Selection                      (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor)

void USotoMenu_C::EquiptHat(const struct FString& Selection)
{
	static auto fn = UObject::FindObject<UFunction>("Function SotoMenu.SotoMenu_C.EquiptHat");

	USotoMenu_C_EquiptHat_Params params;
	params.Selection = Selection;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function SotoMenu.SotoMenu_C.ServerChooseHat
// (Public, BlueprintCallable, BlueprintEvent)
// Parameters:
// class UCustomCharacterPart*    Hat                            (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

void USotoMenu_C::ServerChooseHat(class UCustomCharacterPart* Hat)
{
	static auto fn = UObject::FindObject<UFunction>("Function SotoMenu.SotoMenu_C.ServerChooseHat");

	USotoMenu_C_ServerChooseHat_Params params;
	params.Hat = Hat;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function SotoMenu.SotoMenu_C.EquiptHead
// (Public, BlueprintCallable, BlueprintEvent)
// Parameters:
// struct FString                 Selection                      (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor)

void USotoMenu_C::EquiptHead(const struct FString& Selection)
{
	static auto fn = UObject::FindObject<UFunction>("Function SotoMenu.SotoMenu_C.EquiptHead");

	USotoMenu_C_EquiptHead_Params params;
	params.Selection = Selection;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function SotoMenu.SotoMenu_C.ServerChooseHead
// (Public, BlueprintCallable, BlueprintEvent)
// Parameters:
// class UCustomCharacterPart*    HEAD                           (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

void USotoMenu_C::ServerChooseHead(class UCustomCharacterPart* HEAD)
{
	static auto fn = UObject::FindObject<UFunction>("Function SotoMenu.SotoMenu_C.ServerChooseHead");

	USotoMenu_C_ServerChooseHead_Params params;
	params.HEAD = HEAD;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function SotoMenu.SotoMenu_C.EquiptBody
// (Public, BlueprintCallable, BlueprintEvent)
// Parameters:
// struct FString                 Selection                      (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor)

void USotoMenu_C::EquiptBody(const struct FString& Selection)
{
	static auto fn = UObject::FindObject<UFunction>("Function SotoMenu.SotoMenu_C.EquiptBody");

	USotoMenu_C_EquiptBody_Params params;
	params.Selection = Selection;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function SotoMenu.SotoMenu_C.ServerChooseBody
// (Public, BlueprintCallable, BlueprintEvent)
// Parameters:
// class UCustomCharacterPart*    Body                           (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

void USotoMenu_C::ServerChooseBody(class UCustomCharacterPart* Body)
{
	static auto fn = UObject::FindObject<UFunction>("Function SotoMenu.SotoMenu_C.ServerChooseBody");

	USotoMenu_C_ServerChooseBody_Params params;
	params.Body = Body;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function SotoMenu.SotoMenu_C.BndEvt__ComboBoxString_912_K2Node_ComponentBoundEvent_6_OnSelectionChangedEvent__DelegateSignature
// (BlueprintEvent)
// Parameters:
// struct FString                 SelectedItem                   (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor)
// TEnumAsByte<ESelectInfo>       SelectionType                  (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

void USotoMenu_C::BndEvt__ComboBoxString_912_K2Node_ComponentBoundEvent_6_OnSelectionChangedEvent__DelegateSignature(const struct FString& SelectedItem, TEnumAsByte<ESelectInfo> SelectionType)
{
	static auto fn = UObject::FindObject<UFunction>("Function SotoMenu.SotoMenu_C.BndEvt__ComboBoxString_912_K2Node_ComponentBoundEvent_6_OnSelectionChangedEvent__DelegateSignature");

	USotoMenu_C_BndEvt__ComboBoxString_912_K2Node_ComponentBoundEvent_6_OnSelectionChangedEvent__DelegateSignature_Params params;
	params.SelectedItem = SelectedItem;
	params.SelectionType = SelectionType;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function SotoMenu.SotoMenu_C.BndEvt__ComboBoxString_1_K2Node_ComponentBoundEvent_0_OnSelectionChangedEvent__DelegateSignature
// (BlueprintEvent)
// Parameters:
// struct FString                 SelectedItem                   (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor)
// TEnumAsByte<ESelectInfo>       SelectionType                  (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

void USotoMenu_C::BndEvt__ComboBoxString_1_K2Node_ComponentBoundEvent_0_OnSelectionChangedEvent__DelegateSignature(const struct FString& SelectedItem, TEnumAsByte<ESelectInfo> SelectionType)
{
	static auto fn = UObject::FindObject<UFunction>("Function SotoMenu.SotoMenu_C.BndEvt__ComboBoxString_1_K2Node_ComponentBoundEvent_0_OnSelectionChangedEvent__DelegateSignature");

	USotoMenu_C_BndEvt__ComboBoxString_1_K2Node_ComponentBoundEvent_0_OnSelectionChangedEvent__DelegateSignature_Params params;
	params.SelectedItem = SelectedItem;
	params.SelectionType = SelectionType;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function SotoMenu.SotoMenu_C.BndEvt__ComboBoxString_0_K2Node_ComponentBoundEvent_3_OnSelectionChangedEvent__DelegateSignature
// (BlueprintEvent)
// Parameters:
// struct FString                 SelectedItem                   (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor)
// TEnumAsByte<ESelectInfo>       SelectionType                  (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

void USotoMenu_C::BndEvt__ComboBoxString_0_K2Node_ComponentBoundEvent_3_OnSelectionChangedEvent__DelegateSignature(const struct FString& SelectedItem, TEnumAsByte<ESelectInfo> SelectionType)
{
	static auto fn = UObject::FindObject<UFunction>("Function SotoMenu.SotoMenu_C.BndEvt__ComboBoxString_0_K2Node_ComponentBoundEvent_3_OnSelectionChangedEvent__DelegateSignature");

	USotoMenu_C_BndEvt__ComboBoxString_0_K2Node_ComponentBoundEvent_3_OnSelectionChangedEvent__DelegateSignature_Params params;
	params.SelectedItem = SelectedItem;
	params.SelectionType = SelectionType;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function SotoMenu.SotoMenu_C.BndEvt__Slider_880_K2Node_ComponentBoundEvent_1_OnFloatValueChangedEvent__DelegateSignature
// (BlueprintEvent)
// Parameters:
// float                          Value                          (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

void USotoMenu_C::BndEvt__Slider_880_K2Node_ComponentBoundEvent_1_OnFloatValueChangedEvent__DelegateSignature(float Value)
{
	static auto fn = UObject::FindObject<UFunction>("Function SotoMenu.SotoMenu_C.BndEvt__Slider_880_K2Node_ComponentBoundEvent_1_OnFloatValueChangedEvent__DelegateSignature");

	USotoMenu_C_BndEvt__Slider_880_K2Node_ComponentBoundEvent_1_OnFloatValueChangedEvent__DelegateSignature_Params params;
	params.Value = Value;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function SotoMenu.SotoMenu_C.BndEvt__ComboBoxString_1_K2Node_ComponentBoundEvent_7_OnSelectionChangedEvent__DelegateSignature
// (BlueprintEvent)
// Parameters:
// struct FString                 SelectedItem                   (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor)
// TEnumAsByte<ESelectInfo>       SelectionType                  (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

void USotoMenu_C::BndEvt__ComboBoxString_1_K2Node_ComponentBoundEvent_7_OnSelectionChangedEvent__DelegateSignature(const struct FString& SelectedItem, TEnumAsByte<ESelectInfo> SelectionType)
{
	static auto fn = UObject::FindObject<UFunction>("Function SotoMenu.SotoMenu_C.BndEvt__ComboBoxString_1_K2Node_ComponentBoundEvent_7_OnSelectionChangedEvent__DelegateSignature");

	USotoMenu_C_BndEvt__ComboBoxString_1_K2Node_ComponentBoundEvent_7_OnSelectionChangedEvent__DelegateSignature_Params params;
	params.SelectedItem = SelectedItem;
	params.SelectionType = SelectionType;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function SotoMenu.SotoMenu_C.ExecuteUbergraph_SotoMenu
// ()
// Parameters:
// int                            EntryPoint                     (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

void USotoMenu_C::ExecuteUbergraph_SotoMenu(int EntryPoint)
{
	static auto fn = UObject::FindObject<UFunction>("Function SotoMenu.SotoMenu_C.ExecuteUbergraph_SotoMenu");

	USotoMenu_C_ExecuteUbergraph_SotoMenu_Params params;
	params.EntryPoint = EntryPoint;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
