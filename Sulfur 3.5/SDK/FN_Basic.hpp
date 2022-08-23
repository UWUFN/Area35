#pragma once

// Fortnite (7.00) SDK

#include <string>
#include <locale>

#ifdef _MSC_VER
	#pragma pack(push, 0x8)
#endif

namespace SDK
{
template<typename Fn>
inline Fn GetVFunction(const void *instance, std::size_t index)
{
	auto vtable = *reinterpret_cast<const void***>(const_cast<void*>(instance));
	return reinterpret_cast<Fn>(vtable[index]);
}

class UObject;

struct FUObjectItem // https://github.com/EpicGames/UnrealEngine/blob/4.27/Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectArray.h#L26
{
	// Pointer to the allocated object
	UObject* Object;
	// Internal flags
	int32_t Flags;
	// UObject Owner Cluster Index
	int32_t ClusterRootIndex;
	// Weak Object Pointer Serial number associated with the object
	int32_t SerialNumber;
};

struct FFixedUObjectArray
{
	/** Static master table to chunks of pointers **/
	FUObjectItem* Objects;
	/** Number of elements we currently have **/
	int32_t MaxElements;
	/** Current number of UObject slots */
	int32_t NumElements;

	__forceinline const int32_t Num() const { return NumElements; }

	__forceinline const int32_t Capacity() const { return MaxElements; }

	__forceinline bool IsValidIndex(int32_t Index) const { return Index < Num() && Index >= 0; }

	__forceinline UObject* GetByIndex(int32_t Index) const
	{
		return Objects[Index].Object;
	}
};

struct FName;
struct FString;

inline void(*FNameToString)(FName*, FString&);
inline void(*FreeMemory)(void*);
inline void* (*ReallocMemory)(void* Memory, uint64_t NewSize, uint32_t Alignment);
inline void* (*MallocMemory)(int32_t Size, int32_t Alignment);

template<class T>
struct TArray
{
	friend struct FString;

public:
	inline TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	inline int Num() const
	{
		return Count;
	};

	inline T& operator[](int i)
	{
		return Data[i];
	};

	inline const T& operator[](int i) const
	{
		return Data[i];
	};

	inline bool IsValidIndex(int i) const
	{
		return i < Num();
	}

	inline int Slack() const
	{
		return Max - Count;
	}

	inline void Reserve(const int NumElements)
	{
		Data = Slack() >= NumElements ? Data : (T*)ReallocMemory(Data, (Max = Count + NumElements) * sizeof(T), 0);
	}

	/*inline void Add(T InputData)
	{
		if (!Data)
			Data = (T*)MallocMemory(sizeof(T), alignof(T));

		Data = (T*)ReallocMemory(Data, (Count + 1) * sizeof(T), 0);
		Data[Count + 1] = InputData;
		Count++;
		MessageBoxA(0, std::to_string((uintptr_t)Data).c_str(), "Too Goofy", 0);
	};*/

	inline bool Contains(T Other)
	{
		for (int i = 0; i < Count; i++)
		{
			if (Data[i] == Other) return true;
		}

		return false;
	};

	inline bool RemoveSingle(const int Index)
	{
		if (Index < Count)
		{
			if (Index != Count - 1)
				Data[Index] = Data[Count - 1];

			--Count;

			return true;
		}
		return false;
	}

	inline bool RemoveTen(const int Index)
	{
		if (Index < Count)
		{
			if (Index != Count - 10)
				Data[Index] = Data[Count - 10];

			--Count;

			return true;
		}
		return false;
	}

	inline void Empty()
	{
		Count = 0;
		Max = 0;
		Data = nullptr;
	}

public:
	T* Data;
	int32_t Count;
	int32_t Max;
};

struct FString : private TArray<wchar_t>
{
	inline FString()
	{
	};

	FString(const wchar_t* other)
	{
		Max = Count = *other ? std::wcslen(other) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(other);
		}
	};

	inline bool IsValid() const
	{
		return Data != nullptr;
	}

	inline const wchar_t* c_str() const
	{
		return Data;
	}

	std::string ToString() const
	{
		auto length = std::wcslen(Data);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}
};

struct FName
{
	int32_t ComparisonIndex;
	int32_t Number;

	std::string ToString()
	{
		FString Temp;

		FNameToString(this, Temp);

		auto OutWSTR = std::wstring(Temp.c_str());

		FreeMemory((void*)(Temp.c_str()));

		return std::string(OutWSTR.begin(), OutWSTR.end());
	}
};

template<class TEnum>
class TEnumAsByte
{
public:
	inline TEnumAsByte()
	{
	}

	inline TEnumAsByte(TEnum _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(int32_t _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(uint8_t _value)
		: value(_value)
	{
	}

	inline operator TEnum() const
	{
		return (TEnum)value;
	}

	inline TEnum GetValue() const
	{
		return (TEnum)value;
	}

private:
	uint8_t value;
};

class FScriptInterface
{
private:
	UObject* ObjectPointer;
	void* InterfacePointer;

public:
	inline UObject* GetObject() const
	{
		return ObjectPointer;
	}

	inline UObject*& GetObjectRef()
	{
		return ObjectPointer;
	}

	inline void* GetInterface() const
	{
		return ObjectPointer != nullptr ? InterfacePointer : nullptr;
	}
};

template<class InterfaceType>
class TScriptInterface : public FScriptInterface
{
public:
	inline InterfaceType* operator->() const
	{
		return (InterfaceType*)GetInterface();
	}

	inline InterfaceType& operator*() const
	{
		return *((InterfaceType*)GetInterface());
	}

	inline operator bool() const
	{
		return GetInterface() != nullptr;
	}
};

struct FText
{
	char UnknownData[0x18];
};

struct FScriptDelegate
{
	char UnknownData[16];
};

struct FScriptMulticastDelegate
{
	char UnknownData[16];
};

template<typename Key, typename Value>
class TMap
{
	char UnknownData[0x50];
};

struct FWeakObjectPtr
{
public:
	inline bool SerialNumbersMatch(FUObjectItem* ObjectItem) const
	{
		return ObjectItem->SerialNumber == ObjectSerialNumber;
	}

	bool IsValid() const;

	UObject* Get() const;

	int32_t ObjectIndex;
	int32_t ObjectSerialNumber;
};

template<class T, class TWeakObjectPtrBase = FWeakObjectPtr>
struct TWeakObjectPtr : TWeakObjectPtrBase
{
public:
	inline T* Get() const
	{
		return (T*)TWeakObjectPtrBase::Get();
	}

	inline T& operator*() const
	{
		return *Get();
	}

	inline T* operator->() const
	{
		return Get();
	}

	inline bool IsValid() const
	{
		return TWeakObjectPtrBase::IsValid();
	}
};

template<class T, class TBASE>
class TAutoPointer : public TBASE
{
public:
	inline operator T*() const
	{
		return TBASE::Get();
	}

	inline operator const T*() const
	{
		return (const T*)TBASE::Get();
	}

	explicit inline operator bool() const
	{
		return TBASE::Get() != nullptr;
	}
};

template<typename TObjectID>
class TPersistentObjectPtr_
{
public:
	class FWeakObjectPtr WeakPtr;
	int32_t TagAtLastTest;
	TObjectID ObjectID;
};

struct FSoftObjectPath_
{
	FName AssetPathName;
	FString SubPathString;
};

class FSoftObjectPtr_ : public TPersistentObjectPtr_<FSoftObjectPath_>
{

};

template<typename ObjectType>
class TSoftObjectPtr_ : FSoftObjectPtr_
{

};

template<class T>
class TAutoWeakObjectPtr : public TAutoPointer<T, TWeakObjectPtr<T>>
{
public:
};

template<typename TObjectID>
class TPersistentObjectPtr
{
public:
	FWeakObjectPtr WeakPtr;
	int32_t TagAtLastTest;
	TObjectID ObjectID;
};

struct FStringAssetReference_
{
	char UnknownData[0x10];
};

class FAssetPtr : public TPersistentObjectPtr<FStringAssetReference_>
{

};

template<typename ObjectType>
class TAssetPtr : FAssetPtr
{

};

struct FUniqueObjectGuid_
{
	char UnknownData[0x10];
};

class FLazyObjectPtr : public TPersistentObjectPtr<FUniqueObjectGuid_>
{

};

template<typename ObjectType>
class TLazyObjectPtr : FLazyObjectPtr
{

};
}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
