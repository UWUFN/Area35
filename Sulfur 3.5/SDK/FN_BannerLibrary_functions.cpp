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

// Function BannerLibrary.BannerLibrary_C.UpdateBannerIconOnMaterial
// (Static, Public, BlueprintCallable, BlueprintEvent)
// Parameters:
// class UMaterialInstanceDynamic* Material                       (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
// class UTexture*                Icon                           (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
// class UObject*                 __WorldContext                 (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

static void UBannerLibrary_C::UpdateBannerIconOnMaterial(class UMaterialInstanceDynamic* Material, class UTexture* Icon, class UObject* __WorldContext)
{
	static auto fn = UObject::FindObject<UFunction>("Function BannerLibrary.BannerLibrary_C.UpdateBannerIconOnMaterial");

	UBannerLibrary_C_UpdateBannerIconOnMaterial_Params params;
	params.Material = Material;
	params.Icon = Icon;
	params.__WorldContext = __WorldContext;

	auto flags = fn->FunctionFlags;

	static auto defaultObj = (UObject*)StaticClass();
	defaultObj->ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function BannerLibrary.BannerLibrary_C.UpdateBannerColorOnMaterial
// (Static, Public, BlueprintCallable, BlueprintEvent)
// Parameters:
// class UMaterialInstanceDynamic* Material                       (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
// struct FLinearColor            PrimaryBGColor                 (BlueprintVisible, BlueprintReadOnly, Parm, IsPlainOldData)
// struct FLinearColor            SecondaryBGColor               (BlueprintVisible, BlueprintReadOnly, Parm, IsPlainOldData)
// class UObject*                 __WorldContext                 (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

static void UBannerLibrary_C::UpdateBannerColorOnMaterial(class UMaterialInstanceDynamic* Material, const struct FLinearColor& PrimaryBGColor, const struct FLinearColor& SecondaryBGColor, class UObject* __WorldContext)
{
	static auto fn = UObject::FindObject<UFunction>("Function BannerLibrary.BannerLibrary_C.UpdateBannerColorOnMaterial");

	UBannerLibrary_C_UpdateBannerColorOnMaterial_Params params;
	params.Material = Material;
	params.PrimaryBGColor = PrimaryBGColor;
	params.SecondaryBGColor = SecondaryBGColor;
	params.__WorldContext = __WorldContext;

	auto flags = fn->FunctionFlags;

	static auto defaultObj = (UObject*)StaticClass();
	defaultObj->ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


// Function BannerLibrary.BannerLibrary_C.GenericUpdateMaterial
// (Static, Public, BlueprintCallable, BlueprintEvent)
// Parameters:
// class UMaterialInstanceDynamic* Target                         (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
// class UTexture*                BannerIcon                     (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
// struct FLinearColor            BG_PrimaryColor                (BlueprintVisible, BlueprintReadOnly, Parm, IsPlainOldData)
// struct FLinearColor            BG_SecondaryColor              (BlueprintVisible, BlueprintReadOnly, Parm, IsPlainOldData)
// class UTexture*                ShapeIcon                      (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
// class UObject*                 __WorldContext                 (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)

static void UBannerLibrary_C::GenericUpdateMaterial(class UMaterialInstanceDynamic* Target, class UTexture* BannerIcon, const struct FLinearColor& BG_PrimaryColor, const struct FLinearColor& BG_SecondaryColor, class UTexture* ShapeIcon, class UObject* __WorldContext)
{
	static auto fn = UObject::FindObject<UFunction>("Function BannerLibrary.BannerLibrary_C.GenericUpdateMaterial");

	UBannerLibrary_C_GenericUpdateMaterial_Params params;
	params.Target = Target;
	params.BannerIcon = BannerIcon;
	params.BG_PrimaryColor = BG_PrimaryColor;
	params.BG_SecondaryColor = BG_SecondaryColor;
	params.ShapeIcon = ShapeIcon;
	params.__WorldContext = __WorldContext;

	auto flags = fn->FunctionFlags;

	static auto defaultObj = (UObject*)StaticClass();
	defaultObj->ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
