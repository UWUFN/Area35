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

// BlueprintGeneratedClass StyleLibrary.StyleLibrary_C
// 0x0000 (0x0028 - 0x0028)
class UStyleLibrary_C : public UBlueprintFunctionLibrary
{
public:

	static UClass* StaticClass()
	{
		static auto ptr = UObject::FindClass("BlueprintGeneratedClass StyleLibrary.StyleLibrary_C");
		return ptr;
	}


	static void Get_Conning_Color_For_Specific_Difficulty(int Difficulty_Value, class UObject* __WorldContext, struct FLinearColor* Difficulty_Linear_Color, bool* Invalid);
	static void Get_Conning_Color_From_Difficulty_Value(int Player_Skill_Level, float Content_Difficulty_Level, const struct FGameDifficultyInfo& Game_Difficulty_Info, int Content_Skill_Points, class UObject* __WorldContext, struct FLinearColor* Difficulty_Linear_Color, bool* Success, struct FText* ToolTipText, int* DifficultyValue);
	static void Get_HarvestWeakPoint_Color(const struct FFortUIStylesheet& Stylesheet, class UObject* __WorldContext, struct FLinearColor* StyleSheetOut);
	static void ResizeBrush(TEnumAsByte<EFortBrushSize> BrushSize, const struct FSlateBrush& BaseBrush, class UObject* __WorldContext, struct FSlateBrush* NewParam);
	static void Get_Base___Buff_Colors(EFortStatValueDisplayType Display_Type, EFortBuffState Buff_State, const struct FFortUIStylesheet& Stylesheet, class UObject* __WorldContext, struct FLinearColor* Base, struct FLinearColor* Buff);
	static void Get_Bolt_Elemental_Color(const struct FFortUIStylesheet& Stylesheet, class UObject* __WorldContext, struct FLinearColor* Color);
	static void Get_Ice_Elemental_Color(const struct FFortUIStylesheet& Stylesheet, class UObject* __WorldContext, struct FLinearColor* Color);
	static void Get_Fire_Elemental_Color(const struct FFortUIStylesheet& Stylesheet, class UObject* __WorldContext, struct FLinearColor* Color);
	static void Get_Unique_Color(const struct FFortUIStylesheet& Stylesheet, class UObject* __WorldContext, struct FLinearColor* Color);
	static void Get_Item_Size(const struct FFortUIStylesheet& Stylesheet, TEnumAsByte<EFortBrushSize> Brush_Size, class UObject* __WorldContext, struct FVector2D* Item_Size);
	static void Get_Debuff_Color(const struct FFortUIStylesheet& Stylesheet, class UObject* __WorldContext, struct FLinearColor* Color);
	static void Get_Buff_Color(const struct FFortUIStylesheet& Stylesheet, class UObject* __WorldContext, struct FLinearColor* Color);
	static void Get_Item_Icon_Size(const struct FFortUIStylesheet& Stylesheet, TEnumAsByte<EFortBrushSize> Brush_Size, class UObject* __WorldContext, struct FVector2D* Icon_Size);
	static void Get_Standard_Icon_Size(const struct FFortUIStylesheet& Stylesheet, TEnumAsByte<EFortBrushSize> Brush_Size, class UObject* __WorldContext, struct FVector2D* Icon_Size);
	static void Get_Theme(const struct FFortUIStylesheet& Stylesheet, TEnumAsByte<EFortUITheme> Theme_Type, class UObject* __WorldContext, struct FFortUITheme* Theme);
	static void Get_Theme_Color(const struct FFortUIStylesheet& Stylesheet, TEnumAsByte<EFortUITheme> Theme_Type, TEnumAsByte<EFortUIThemeColor> Color_Type, class UObject* __WorldContext, struct FLinearColor* Theme_Color);
};


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
