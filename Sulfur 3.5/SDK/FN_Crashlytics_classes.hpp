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

// Class Crashlytics.CrashlyticsBlueprintLibrary
// 0x0000 (0x0028 - 0x0028)
class UCrashlyticsBlueprintLibrary : public UBlueprintFunctionLibrary
{
public:

	static UClass* StaticClass()
	{
		static auto ptr = UObject::FindClass("Class Crashlytics.CrashlyticsBlueprintLibrary");
		return ptr;
	}


	static void SetUserName(const struct FString& Name);
	static void SetUserIdentifier(const struct FString& ID);
	static void SetUserEmail(const struct FString& Email);
	static void SetStringKey(const struct FString& Key, const struct FString& Value);
	static void SetIntegerKey(const struct FString& Key, int Value);
	static void SetFloatKey(const struct FString& Key, float Value);
	static void SetBooleanKey(const struct FString& Key, bool Value);
	static void LogMessage(const struct FString& MESSAGE);
	static void LogException(const struct FString& MESSAGE);
};


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
