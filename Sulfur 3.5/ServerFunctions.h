#pragma once
#include "framework.h"
#include <chrono>
#include <thread>
#include <map>
#include <chrono>
#include <random>

using namespace std::chrono;

struct Timer
{
    time_point<std::chrono::steady_clock> start, end;
    duration<float> dura;

    Timer()
    {
        start = high_resolution_clock::now();
    }

    ~Timer()
    {
        end = high_resolution_clock::now();
        dura = end - start;

        float ms = dura.count() * 1000.0f;
        std::cout << "Took " << ms << "ms \n";
    }

    // You would do "Timer* t = new Timer;" and then delete at the end of the function.
};

namespace Utils
{
    bool GuidComp(FGuid guidA, FGuid guidB)
    {
        if (guidA.A == guidB.A && guidA.B == guidB.B && guidA.C == guidB.C && guidA.D == guidB.D)
            return true;
        else
            return false;
    }

    std::string ActorRoletoString(ENetRole Role)
    {
        switch (Role)
        {
        case ENetRole::ROLE_Authority :
        {
            return "Authority";
        }
        case ENetRole::ROLE_AutonomousProxy :
        {
            return "AutonomousProxy";
        }
        case ENetRole::ROLE_SimulatedProxy :
        {
            return "SimulatedProxy";
        }
        }
    }

    template <typename T>
    static T* FindObjectFast(std::string ObjectName, UClass* ObjectClass = UObject::StaticClass())
    {
        auto OrigInName = std::wstring(ObjectName.begin(), ObjectName.end()).c_str();

        static auto StaticFindObjectAddr = Memory::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8B EC 48 83 EC 60 80 3D ? ? ? ? ? 45 0F B6 F1");
        auto StaticFindObject = (T * (*)(UClass*, UObject * Package, const wchar_t* OrigInName, bool ExactClass))(StaticFindObjectAddr);
        return StaticFindObject(ObjectClass, nullptr, OrigInName, false);
    }
}

namespace Logging
{
    inline std::string getCurrentDateTime(std::string s) {
        time_t now = time(0);
        struct tm  tstruct;
        char  buf[80];
        tstruct = *localtime(&now);
        if (s == "now")
            strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
        else if (s == "date")
            strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
        return std::string(buf);
    };
    inline void FileLogger(std::string logMsg) {

        std::string filePath = "D:\\ServerLogs" + getCurrentDateTime("date") + ".txt";
        std::string now = getCurrentDateTime("now");
        std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);
        ofs << now << '\t' << logMsg << '\n';
        ofs.close();
    }
}

#ifndef FORCEINLINE
#define FORCEINLINE __forceinline
#endif

template<class TArrayType>
class TArray_
{

public:
    FORCEINLINE int Num() const
    {
        return Count;
    }
    FORCEINLINE int Max() const
    {
        return MaxElements;
    }
    FORCEINLINE int Slack() const
    {
        return MaxElements - Count;
    }
    FORCEINLINE void Reserve(const int NumElements)
    {
        Data = Slack() >= NumElements ? Data : (TArrayType*)ReallocMemory(Data, (MaxElements = Count + NumElements) * sizeof(TArrayType), 0);
    }
    FORCEINLINE void Reset(int MinSizeAfterReset = 0)
    {
        MaxElements = MinSizeAfterReset;
        Count = 0;

        FreeMemory(Data);
        Data = MallocMemory(MinSizeAfterReset * sizeof(TArrayType), alignof(TArrayType));
    }
    FORCEINLINE bool RemoveSingle(const int Index)
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
    FORCEINLINE void RemoveAt(int Index, int Lenght = 1)
    {
        for (; Lenght != 0; --Lenght)
        {
            if (!RemoveSingle(Index++))
                break;
        }
    }
    FORCEINLINE void AddSingle(TArrayType InData)
    {
        Reserve(1);
        Data[Count] = InData;
        ++Count;
    }
    FORCEINLINE void Add(int n, TArrayType InData...)
    {
        va_list MyList;
        va_start(MyList, n);

        for (int i = 0; i < n; i++)
        {
            AddSingle(va_arg(MyList, TArrayType));
        }
        va_end(MyList);
    };
    FORCEINLINE void FreeArray()
    {
        FreeMemory(Data);
        Data = nullptr;
        Count = 0;
        MaxElements = 0;
    }

    FORCEINLINE explicit operator bool()
    {
        return MaxElements > 0 && Count > 0 && Data;
    }
    FORCEINLINE TArrayType& operator[](int i)
    {
        return Data[i];
    };

    FORCEINLINE const TArrayType& operator[](int i) const
    {
        return Data[i];
    };

    FORCEINLINE int MaxIndex()
    {
        return Count - 1;
    }

    class FBaseArrayIterator
    {
        int32_t Index;
        TArray<TArrayType>& ItArray;

    public:
        FBaseArrayIterator(TArray<TArrayType>& Array)
            : ItArray(Array), Index(Array.Count)
        {
        }
        FBaseArrayIterator(TArray<TArrayType>& Array, int32_t CurrentIndex)
            : ItArray(Array), Index(CurrentIndex)
        {
        }

        FORCEINLINE TArrayType& operator*()
        {
            return ItArray[Index];
        }
        FORCEINLINE TArrayType& operator->()
        {
            return ItArray[Index];
        }
        FORCEINLINE FBaseArrayIterator& operator++()
        {
            ++Index;
            return *this;
        }
        FORCEINLINE bool operator==(const FBaseArrayIterator& other) const
        {
            return Index == other.Index;
        }
        FORCEINLINE bool operator!=(const FBaseArrayIterator& other) const
        {
            return Index != other.Index;
        }
    };

    FORCEINLINE FBaseArrayIterator begin()
    {
        return FBaseArrayIterator(*this, 0);
    }
    FORCEINLINE FBaseArrayIterator begin() const
    {
        return FBaseArrayIterator(*this, 0);
    }
    FORCEINLINE FBaseArrayIterator end()
    {
        return FBaseArrayIterator(*this);
    }
    FORCEINLINE FBaseArrayIterator end() const
    {
        return FBaseArrayIterator(*this);
    }

private:
    TArrayType* Data;
    int32_t Count;
    int32_t MaxElements;
};


static float MaxWalkSpeedAthena = 0.0f;

namespace ServerFunctions
{
    void (*MarkArrayDirty)(FFastArraySerializer*);
    static void* (*MarkAbilitySpecDirty)(UAbilitySystemComponent*, FGameplayAbilitySpec, bool);
    static UObject* (*StaticConstructObjectInternal)(void*, void*, void*, int, unsigned int, void*, bool, void*, bool);

    static void UpdateInventory(AFortPlayerController* TargetController)
    {
        MarkArrayDirty(&TargetController->WorldInventory->Inventory);

        TargetController->HandleWorldInventoryLocalUpdate();
        TargetController->WorldInventory->HandleInventoryLocalUpdate();
        TargetController->OnRep_QuickBar();
        TargetController->QuickBars->OnRep_PrimaryQuickBar();
        TargetController->QuickBars->OnRep_SecondaryQuickBar();

        TargetController->ForceUpdateQuickbar(EFortQuickBars::Primary);
        TargetController->ForceUpdateQuickbar(EFortQuickBars::Secondary);
    }

    /*static void SpawnAI()
    {
        auto HuskClass = (UClass*)StaticLoadObject(UClass::StaticClass(), nullptr, L"/Game/Characters/Enemies/Husk/Blueprints/HuskPawn.HuskPawn_C", nullptr, 0, nullptr, 0);
		auto AiHusk = World->SpawnActor<AHuskPawn_C>(GetSpawnLocationForAI(), { 0, 0, 0 }, HuskClass);
        auto AiController = World->SpawnActor<AFortAIController>({}, {});
        AiController->Possess(AiHusk);
    }*/

    static FVector CalculateSpectatorPawnLocation(AActor* TargetPawn)
    {
        auto FirstVect = TargetPawn->K2_GetActorLocation();
        auto SecondVect = FVector{ FirstVect.X, FirstVect.Y, FirstVect.Z + 150 };
        auto ThirdVect = FVector{ FirstVect.X - 500, FirstVect.Y, FirstVect.Z };

        return ThirdVect;
    }

    static AActor* GetRandomPlayerToSpectate(AFortPlayerController* SpectatorPC)
    {
        for (int i = 0; i < World->NetDriver->ClientConnections.Num(); i++)
        {
            auto Connection = World->NetDriver->ClientConnections[i];
            auto PlayerController = (AFortPlayerController*)Connection->PlayerController;
            auto Pawn = (AFortPlayerPawn*)PlayerController->Pawn;

            if (!PlayerController)
                continue;

            if (!Pawn)
                continue;

            if (Pawn->IsDead())
                continue;

            if (SpectatorPC == PlayerController)
                continue;

            return Pawn;

        }

        return nullptr;
    }

    static void EnableDamageDelayed(APlayerPawn_Athena_C* Target)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));

        Target->bCanBeDamaged = true;
    }

    static void ClientPlaySoundMulticast(USoundWave* Sound)
    {
        for (int i = 0; i < World->NetDriver->ClientConnections.Num(); i++)
        {
            auto Connection = World->NetDriver->ClientConnections[i];
            auto PlayerController = Connection->PlayerController;

            if (!PlayerController)
                continue;

            PlayerController->ClientPlaySound(Sound, 0.1, 1);
        }
    }

    static FGameplayAbilitySpec* FindAbilitySpecFromHandle(UAbilitySystemComponent* AbilityComp, FGameplayAbilitySpecHandle Handle)
    {
        for (int ConnIdx = 0; ConnIdx < AbilityComp->ActivatableAbilities.Items.Num(); ConnIdx++)
        {
            auto Spec = AbilityComp->ActivatableAbilities.Items[ConnIdx];

            if (Spec.Handle.Handle == Handle.Handle)
            {
                return &Spec;
            }
        }

        return nullptr;
    }

    static FGameplayAbilitySpec* FindAbilitySpecFromClass(UAbilitySystemComponent* AbilityComp, UClass* Class)
    {
        for (int ConnIdx = 0; ConnIdx < AbilityComp->ActivatableAbilities.Items.Num(); ConnIdx++)
        {
            auto Spec = AbilityComp->ActivatableAbilities.Items[ConnIdx];

            if (Spec.Ability && Spec.Ability->IsA(Class))
            {
                return &Spec;
            }
        }

        return nullptr;
    }

    static void GrantGameplayAbility(APlayerPawn_Athena_C* TargetPawn, UClass* GameplayAbilityClass) // Bro this scuffed way crashes 9/10 times
    {
        auto AbilitySystemComponent = TargetPawn->AbilitySystemComponent;

        static UGameplayEffect* DefaultGameplayEffect = Utils::FindObjectFast<UGameplayEffect>("/Game/Abilities/Player/Constructor/Perks/ContainmentUnit/GE_Constructor_ContainmentUnit_Applied.Default__GE_Constructor_ContainmentUnit_Applied_C");

        if (!DefaultGameplayEffect)
            return;

        TArray<FGameplayAbilitySpecDef> GrantedAbilities = DefaultGameplayEffect->GrantedAbilities;

        // overwrite current gameplay ability with the one we want to activate
        GrantedAbilities[0].Ability = GameplayAbilityClass;

        // give this gameplay effect an infinite duration
        DefaultGameplayEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;

        static auto GameplayEffectClass = Utils::FindObjectFast<UClass>("/Game/Abilities/Player/Constructor/Perks/ContainmentUnit/GE_Constructor_ContainmentUnit_Applied.GE_Constructor_ContainmentUnit_Applied_C");

        if (!GameplayEffectClass)
            return;

        auto handle = FGameplayEffectContextHandle();

		if (GameplayEffectClass && AbilitySystemComponent)
            AbilitySystemComponent->BP_ApplyGameplayEffectToTarget(GameplayEffectClass, AbilitySystemComponent, 1, handle);
    }

    static void GiveGameplayAbilities(APlayerPawn_Athena_C* Pawn)
    {
        static auto a1 = UObject::FindClass("Class FortniteGame.FortGameplayAbility_Sprint");
        static auto a4 = UObject::FindClass("Class FortniteGame.FortGameplayAbility_Jump");
        static auto a7 = UObject::FindClass("BlueprintGeneratedClass GA_DefaultPlayer_InteractSearch.GA_DefaultPlayer_InteractSearch_C");
        static auto a5 = UObject::FindClass("BlueprintGeneratedClass GA_DefaultPlayer_Death.GA_DefaultPlayer_Death_C");
        static auto a3 = UObject::FindClass("Class FortniteGame.FortGameplayAbility_RangedWeapon");

		// GrantGameplayAbility(Pawn, a1);
		// GrantGameplayAbility(Pawn, a4);
		// GrantGameplayAbility(Pawn, a7);
		// GrantGameplayAbility(Pawn, a5);
        // GrantGameplayAbility(Pawn, a3);

        /* static auto AbilitySet = Utils::FindObjectFast<UFortAbilitySet>("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_DefaultPlayer.GAS_DefaultPlayer");

        if (AbilitySet)
        {
            for (int i = 0; i < AbilitySet->GameplayAbilities.Num(); i++)
            {
                auto Ability = AbilitySet->GameplayAbilities[i];

                if (Ability)
                    GrantGameplayAbility(Pawn, Ability);
            }
        } */

        // ServerFunctions::GrantGameplayAbility((APlayerPawn_Athena_C*)Pawn, UGA_Ranged_GenericDamage_C::StaticClass());

		
    }

    /*static void SetCharacterParts(AFortPlayerStateAthena* PlayerState)
    {
		auto CID = Utils::FindObjectFast<UAthenaCharacterItemDefinition>("/Game/Athena/Items/Cosmetics/Characters/CID_033_Athena_Commando_F_Medieval.CID_033_Athena_Commando_F_Medieval");  

        for (int i = 0; i < HeroDefinition->CharacterParts.Num(); i++)
        {
			auto Part = HeroDefinition->CharacterParts[i];
            PlayerState->CharacterParts[i] = Part;
        }
		
        PlayerState->OnRep_CharacterParts();
    }*/

    static void ShootDelayed(int Delay, FGameplayAbilityActivationInfo Actinfo, FGameplayAbilitySpecHandle Handle, UFortAbilitySystemComponent* AbilityComp)
    {
        AbilityComp->ClientTryActivateAbility(Handle);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        AbilityComp->ClientEndAbility(Handle, Actinfo);
    }

    static UFortItem* CreateItem(AFortPlayerController* TargetController, UFortItemDefinition* ItemDefinition, int count)
    {
        auto TempItemInstance = ItemDefinition->CreateTemporaryItemInstanceBP(count, 1);

        if (TempItemInstance)
        {
            TempItemInstance->SetOwningControllerForTemporaryItem(TargetController);
        }

        return TempItemInstance;
    }

    static void SetIsReady()
    {
        std::this_thread::sleep_for(std::chrono::seconds(9));
        Globals::Beacon->BeaconState = EBeaconState::AllowRequests;
    }

    static void AddItemToInventory(UFortItemDefinition* ItemDefinition, EFortQuickBars Quickbar, AFortPlayerController* TargetController, int count, int Slot)
    {
        if (!ItemDefinition)
        {
			printf("Invalid Item Definition\n");
            return;
        }
           

        auto Inventory = TargetController->WorldInventory;

        auto QuickBars = TargetController->QuickBars;

        if (Quickbar == EFortQuickBars::Secondary)
        {
            UFortWorldItem* ItemInstance = nullptr;

            for (int i = 0; i < Inventory->Inventory.ItemInstances.Num(); i++)
            {
                if (Inventory->Inventory.ItemInstances[i]->GetItemDefinitionBP() == ItemDefinition)
                    ItemInstance = Inventory->Inventory.ItemInstances[i];
            }

            if (ItemInstance)
            {
                auto NewCount = ItemInstance->ItemEntry.Count + count;
                for (int i = 0; i < Inventory->Inventory.ReplicatedEntries.Num(); i++)
                {
					if (Inventory->Inventory.ReplicatedEntries[i].ItemDefinition == ItemDefinition)
					{
						Inventory->Inventory.ReplicatedEntries[i].Count = NewCount;
                        Inventory->Inventory.ReplicatedEntries[i].ReplicationKey++;
					}
                }
                ItemInstance->ItemEntry.Count = NewCount;
                ItemInstance->ItemEntry.ReplicationKey++;
                return;
            }
        }

		
        auto ItemInstance = (UFortWorldItem*)CreateItem(TargetController, ItemDefinition, count);
        ItemInstance->OwnerInventory = Inventory;
        ItemInstance->bTemporaryItemOwningController = true;

        if (Inventory && QuickBars)
        {
            auto ReplicatedEntries = (TArray_<FFortItemEntry>*) & Inventory->Inventory.ReplicatedEntries;
            auto ItemInstances = (TArray_<UFortWorldItem*>*) & Inventory->Inventory.ItemInstances;

            ReplicatedEntries->AddSingle(ItemInstance->ItemEntry);
            ItemInstances->AddSingle(ItemInstance);


            Logging::FileLogger("Added Item To Inventory");

            TargetController->AddItemToQuickBars(ItemDefinition, Quickbar, Slot);
            TargetController->QuickBars->ServerAddItemInternal(ItemInstance->GetItemGuid(), Quickbar, Slot);

            Logging::FileLogger("Added Item To QuickBars");
        }

        UpdateInventory(TargetController);
    }

    static UFortTrapItemDefinition* GetRandomTrapDefinition()
    {
        while (true)
        {
            auto Idx = UKismetMathLibrary::RandomInteger(TrapsPool.size());
            auto Item = TrapsPool[Idx];
            auto Def = Utils::FindObjectFast<UFortTrapItemDefinition>(Item);
            if (!Def)
                continue;

            auto rng = std::default_random_engine{};
            std::shuffle(TrapsPool.begin(), TrapsPool.end(), rng);
            return Def;
        }
    }

    static UFortItemDefinition* GetRandomPickaxeDefinition()
    {
        while (true)
        {
            auto Idx = UKismetMathLibrary::RandomInteger(PickaxePool.size());
            auto Item = PickaxePool[Idx];
            auto Def = Utils::FindObjectFast<UFortItemDefinition>(Item);
            if (!Def)
                continue;

            auto rng = std::default_random_engine{};
            std::shuffle(PickaxePool.begin(), PickaxePool.end(), rng);
            return Def;
        }
    }

    static void SetupInventory(AFortPlayerController* TargetController)
    {
        auto Pawn = (APlayerPawn_Athena_C*)TargetController->Pawn;
        //Building Peaces	
        auto pickaxe = GetRandomPickaxeDefinition();
        static auto m_pWallBuildDef = Utils::FindObjectFast<UFortBuildingItemDefinition>("/Game/Items/Weapons/BuildingTools/BuildingItemData_Wall.BuildingItemData_Wall");
        static auto m_pFloorBuildDef = Utils::FindObjectFast<UFortBuildingItemDefinition>("/Game/Items/Weapons/BuildingTools/BuildingItemData_Floor.BuildingItemData_Floor");
        static auto m_pStairBuildDef = Utils::FindObjectFast<UFortBuildingItemDefinition>("/Game/Items/Weapons/BuildingTools/BuildingItemData_Stair_W.BuildingItemData_Stair_W");
        static auto m_pRoofBuildDef = Utils::FindObjectFast<UFortBuildingItemDefinition>("/Game/Items/Weapons/BuildingTools/BuildingItemData_RoofS.BuildingItemData_RoofS");
        //Building Mats	
        static auto WoodItemDef = Utils::FindObjectFast<UFortBuildingItemDefinition>("/Game/Items/ResourcePickups/WoodItemData.WoodItemData");
        static auto BrickItemDef = Utils::FindObjectFast<UFortBuildingItemDefinition>("/Game/Items/ResourcePickups/StoneItemData.StoneItemData");
        static auto MetalItemDef = Utils::FindObjectFast<UFortBuildingItemDefinition>("/Game/Items/ResourcePickups/MetalItemData.MetalItemData");
        //Weapons	
        static auto AssualtRifle = Utils::FindObjectFast<UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_AMR_Athena_SR_Ore_T03");
        static auto Smg = Utils::FindObjectFast<UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_UC_Ore_T03.WID_Pistol_Scavenger_Athena_UC_Ore_T03");
        static auto PumpShotgun = Utils::FindObjectFast<UFortBuildingItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03");
        static auto Sniper = Utils::FindObjectFast<UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_NoScope_Athena_UC_Ore_T03.WID_Sniper_NoScope_Athena_UC_Ore_T03");
        static auto slurp = Utils::FindObjectFast<UFortWeaponItemDefinition>("/Game/Athena/Items/Consumables/KnockGrenade/Athena_KnockGrenade.Athena_KnockGrenade");
        static auto JumpPad = Utils::FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Traps/TID_Floor_Player_Launch_Pad_Athena.TID_Floor_Player_Launch_Pad_Athena");
        /*static auto SpikeTrap = Utils::FindObjectFast<UFortItemDefinition>("/Game/Items/Traps/Floor/TID_Floor_Spikes_UC_T03.TID_Floor_Spikes_UC_T03");*/
        static auto Rockets = Utils::FindObjectFast<UFortAmmoItemDefinition>("/Game/Athena/Items/Ammo/AmmoDataRockets.AmmoDataRockets");
        static auto Shells = Utils::FindObjectFast<UFortAmmoItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataShells.AthenaAmmoDataShells");
        static auto Medium = Utils::FindObjectFast<UFortAmmoItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium");
        static auto Light = Utils::FindObjectFast<UFortAmmoItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight");
        static auto Heavy = Utils::FindObjectFast<UFortAmmoItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy");
        static auto Trap = Utils::FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Traps/TID_Floor_Spikes_Athena_R_T03.TID_Floor_Spikes_Athena_R_T03");

        TargetController->QuickBars->ServerEnableSlot(SDK::EFortQuickBars::Secondary, 0);
        TargetController->QuickBars->ServerEnableSlot(SDK::EFortQuickBars::Secondary, 1);
        TargetController->QuickBars->ServerEnableSlot(SDK::EFortQuickBars::Secondary, 2);
        TargetController->QuickBars->ServerEnableSlot(SDK::EFortQuickBars::Secondary, 3);
        TargetController->QuickBars->ServerEnableSlot(SDK::EFortQuickBars::Secondary, 4);
        TargetController->QuickBars->ServerEnableSlot(SDK::EFortQuickBars::Secondary, 5);
        TargetController->QuickBars->ServerEnableSlot(SDK::EFortQuickBars::Primary, 0);
        TargetController->QuickBars->ServerEnableSlot(SDK::EFortQuickBars::Primary, 1);
        TargetController->QuickBars->ServerEnableSlot(SDK::EFortQuickBars::Primary, 2);
        TargetController->QuickBars->ServerEnableSlot(SDK::EFortQuickBars::Primary, 3);
        TargetController->QuickBars->ServerEnableSlot(SDK::EFortQuickBars::Primary, 4);
        TargetController->QuickBars->ServerEnableSlot(SDK::EFortQuickBars::Primary, 5);
        //Add Weapons	
		if (pickaxe)
            AddItemToInventory(pickaxe, EFortQuickBars::Primary, TargetController, 1, 0);
        //Add Building Peaces
        while (!m_pWallBuildDef) // Checks if it's null
        {
            m_pWallBuildDef = Utils::FindObjectFast<UFortBuildingItemDefinition>("/Game/Items/Weapons/BuildingTools/BuildingItemData_Wall.BuildingItemData_Wall");
            Sleep(500);
        }
            AddItemToInventory(m_pWallBuildDef, EFortQuickBars::Secondary, TargetController, 1, 0);
        if (m_pFloorBuildDef)
            AddItemToInventory(m_pFloorBuildDef, EFortQuickBars::Secondary, TargetController, 1, 1);
		if (m_pStairBuildDef)
            AddItemToInventory(m_pStairBuildDef, EFortQuickBars::Secondary, TargetController, 1, 2);
        if (m_pRoofBuildDef)
            AddItemToInventory(m_pRoofBuildDef, EFortQuickBars::Secondary, TargetController, 1, 3);
        //Add Mats	
        if (WoodItemDef)
            AddItemToInventory(WoodItemDef, EFortQuickBars::Secondary, TargetController, 500, 0);
        if (BrickItemDef)
            AddItemToInventory(BrickItemDef, EFortQuickBars::Secondary, TargetController, 300, 0);
        if (MetalItemDef)
            AddItemToInventory(MetalItemDef, EFortQuickBars::Secondary, TargetController, 100, 0);
        // Add Ammo
		
        if (Rockets)
            AddItemToInventory(Rockets, EFortQuickBars::Secondary, TargetController, 999, 0);
        if (Shells)
            AddItemToInventory(Shells, EFortQuickBars::Secondary, TargetController, 999, 0);
        if (Medium)
            AddItemToInventory(Medium, EFortQuickBars::Secondary, TargetController, 999, 0);
        if (Light)
            AddItemToInventory(Light, EFortQuickBars::Secondary, TargetController, 999, 0);
        if (Heavy)
            AddItemToInventory(Heavy, EFortQuickBars::Secondary, TargetController, 999, 0);

        //traps 
        // AddItemToInventory(GetRandomTrapDefinition(), EFortQuickBars::Secondary, TargetController, 3, 0);
         AddItemToInventory(JumpPad, EFortQuickBars::Secondary, TargetController, 420, 0); 

    }

    static void GiveBackMats(AFortPlayerControllerAthena* TargetController)
    {
        static auto WoodItemDef = Utils::FindObjectFast<UFortBuildingItemDefinition>("/Game/Items/ResourcePickups/WoodItemData.WoodItemData");
        static auto BrickItemDef = Utils::FindObjectFast<UFortBuildingItemDefinition>("/Game/Items/ResourcePickups/StoneItemData.StoneItemData");
        static auto MetalItemDef = Utils::FindObjectFast<UFortBuildingItemDefinition>("/Game/Items/ResourcePickups/MetalItemData.MetalItemData");

        if (WoodItemDef)
            AddItemToInventory(WoodItemDef, EFortQuickBars::Secondary, TargetController, 999, 0);
        if (BrickItemDef)
            AddItemToInventory(BrickItemDef, EFortQuickBars::Secondary, TargetController, 999, 0);
        if (MetalItemDef)
            AddItemToInventory(MetalItemDef, EFortQuickBars::Secondary, TargetController, 999, 0);
    }

    static void GiveWeapons(AFortPlayerControllerAthena* TargetController)
    {
        static auto AssualtRifle = Utils::FindObjectFast<UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_C_Ore_T02.WID_Assault_Auto_Athena_C_Ore_T02");
        static auto Smg = Utils::FindObjectFast<UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_UC_Ore_T03.WID_Shotgun_SemiAuto_Athena_UC_Ore_T03");
        static auto PumpShotgun = Utils::FindObjectFast<UFortBuildingItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_C_Ore_T02.WID_Pistol_AutoHeavySuppressed_Athena_C_Ore_T02");
        static auto Sniper = Utils::FindObjectFast<UFortWeaponItemDefinition>("/Game/Athena/Items/Consumables/Medkit/Athena_Medkit.Athena_Medkit");
        static auto slurp = Utils::FindObjectFast<UFortWeaponItemDefinition>("/Game/Athena/Items/Consumables/KnockGrenade/Athena_KnockGrenade.Athena_KnockGrenade");
        static auto Zap = Utils::FindObjectFast<UFortAmmoItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataEnergyCell.AthenaAmmoDataEnergyCell");

        if (AssualtRifle)
            AddItemToInventory(AssualtRifle, EFortQuickBars::Primary, TargetController, 1, 1);
        AddItemToInventory(Smg, EFortQuickBars::Primary, TargetController, 1, 2);
        AddItemToInventory(PumpShotgun, EFortQuickBars::Primary, TargetController, 1, 3);
        AddItemToInventory(Sniper, EFortQuickBars::Primary, TargetController, 2, 4);
        AddItemToInventory(slurp, EFortQuickBars::Primary, TargetController, 3, 5);
        AddItemToInventory(Zap, EFortQuickBars::Secondary, TargetController, 999, 0);
    }

    std::wstring s2ws(const std::string& str)
    {
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
        std::wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
        return wstrTo;
    }

    static void RemoveBuildingAmount(UClass* BuildingClass, AFortPlayerControllerAthena* TargetController)
    {
        auto Inventory = TargetController->WorldInventory;

        auto ReplicatedEntries = Inventory->Inventory.ReplicatedEntries;
        auto ItemInstances = Inventory->Inventory.ItemInstances;

        for (int i = 0; i < Inventory->Inventory.ReplicatedEntries.Num(); i++)
        {
            if (Inventory->Inventory.ReplicatedEntries[i].ItemDefinition->GetName().contains("ItemData"))
            {
                int NewCount = Inventory->Inventory.ReplicatedEntries[i].Count - 10;
				
                if (BuildingClass->GetName().contains("W1") && Inventory->Inventory.ReplicatedEntries[i].ItemDefinition->GetName().contains("Wood"))
                {
                    Inventory->Inventory.ReplicatedEntries[i].Count -= 10;
                    Inventory->Inventory.ReplicatedEntries[i].ReplicationKey++;

                    for (int j = 0; j < Inventory->Inventory.ItemInstances.Num(); j++)
                    {
						if (Inventory->Inventory.ItemInstances[j]->GetName().contains("Wood"))
						{
							Inventory->Inventory.ItemInstances[j]->ItemEntry.Count -= 10;
                            Inventory->Inventory.ItemInstances[j]->ItemEntry.ReplicationKey++;
                        }
                    }
					
                    //ItemInstances.RemoveSingle(i);
				    //AddItemToInventory(Inventory->Inventory.ReplicatedEntries[i].ItemDefinition, EFortQuickBars::Secondary, TargetController, Inventory->Inventory.ReplicatedEntries[i].Count - 10, 0);
                    UpdateInventory(TargetController);
                    return;
                }
                if (BuildingClass->GetName().contains("S1") && Inventory->Inventory.ReplicatedEntries[i].ItemDefinition->GetName().contains("Stone"))
                {
                    Inventory->Inventory.ReplicatedEntries[i].Count -= 10;
                    Inventory->Inventory.ReplicatedEntries[i].ReplicationKey++;
					
                    for (int j = 0; j < Inventory->Inventory.ItemInstances.Num(); j++)
                    {
                        if (Inventory->Inventory.ItemInstances[j]->GetName().contains("Stone"))
                        {
                            Inventory->Inventory.ItemInstances[j]->ItemEntry.Count -= 10;
                            Inventory->Inventory.ItemInstances[j]->ItemEntry.ReplicationKey++;
                        }
                    }

                    //ItemInstances.RemoveSingle(i);
                    //AddItemToInventory(Inventory->Inventory.ReplicatedEntries[i].ItemDefinition, EFortQuickBars::Secondary, TargetController, Inventory->Inventory.ReplicatedEntries[i].Count - 10, 1);
                    UpdateInventory(TargetController);
                    return;
                }
                if (BuildingClass->GetName().contains("M1") && Inventory->Inventory.ReplicatedEntries[i].ItemDefinition->GetName().contains("Metal"))
                {
                    Inventory->Inventory.ReplicatedEntries[i].Count -= 10;
                    Inventory->Inventory.ReplicatedEntries[i].ReplicationKey++;
					
                    for (int j = 0; j < Inventory->Inventory.ItemInstances.Num(); j++)
                    {
                        if (Inventory->Inventory.ItemInstances[j]->GetName().contains("Metal"))
                        {
                            Inventory->Inventory.ItemInstances[j]->ItemEntry.Count -= 10;
                            Inventory->Inventory.ItemInstances[j]->ItemEntry.ReplicationKey++;
                        }
                    }

                    //ItemInstances.RemoveSingle(i);
                    //AddItemToInventory(Inventory->Inventory.ReplicatedEntries[i].ItemDefinition, EFortQuickBars::Secondary, TargetController, Inventory->Inventory.ReplicatedEntries[i].Count - 10, 2);
                    UpdateInventory(TargetController);
                    return; 
                }
            }
        }
    }

    static void EmptyInventorys()
    {
        for (int i = 0; i < World->NetDriver->ClientConnections.Num(); i++)
        {
            auto Connection = World->NetDriver->ClientConnections[i];
            auto PlayerController = (AFortPlayerController*)Connection->PlayerController;
            auto Inventory = PlayerController->WorldInventory;

            if (!PlayerController)
                continue;

            if (!Inventory)
                continue;

            for (int i = 0; i < Inventory->Inventory.ItemInstances.Num(); i++)
            {

                if (i == 0)
                    continue;

                if (Inventory->Inventory.ReplicatedEntries[i].ItemDefinition->GetName().contains("Building"))
                    continue;

                if (Inventory->Inventory.ReplicatedEntries[i].ItemDefinition->GetName().contains("Harvest"))
                    continue;

                if (Inventory->Inventory.ReplicatedEntries[i].ItemDefinition->GetName().contains("Wood"))
                    continue;

                PlayerController->QuickBars->EmptySlot(EFortQuickBars::Primary, i);
                Inventory->Inventory.ItemInstances.RemoveSingle(i);
                Inventory->Inventory.ReplicatedEntries.RemoveSingle(i);
            }

            UpdateInventory(PlayerController);
            SetupInventory(PlayerController);
        }
    }

    static std::string GetPlayerName(UNetConnection* Connection)
    {
        try
        {
            std::string PlayerName = std::string();

            for (auto Char : Connection->RequestURL.ToString())
            {
                if (Char == '=' && PlayerName.size() != 0)
                    break;

                if (Char == '?' && PlayerName.size() != 0)
                    break;

                if (Char == '=' && PlayerName.size() == 0 || PlayerName.size() != 0)
                    PlayerName += Char;
            }

            PlayerName = PlayerName.erase(0, 1);
            return PlayerName;
        }
        catch (...)
        {
            return "";
        }

        return "";
    }

    static int GetTeamId(std::string PlayerName)
    {
        try
        {
            return std::stoi(PlayerName.substr(0, PlayerName.find('_')));
        }
        catch (...)
        {
            return rand() % 1000;
        }

        return rand() % 1000;
    }

    static int GetNextSlot(AFortPlayerController* PC)
    {
        return PC->QuickBars->PrimaryQuickBar.Slots.Num() + 1;
    }

    static UFortItemDefinition* GetRandomItemDefinition()
    {
        while (true)
        {
            auto Idx = UKismetMathLibrary::RandomInteger(AthenaLootPool.size());
            auto Item = AthenaLootPool[Idx];
            auto Def = Utils::FindObjectFast<UFortItemDefinition>(Item);
            if (!Def)
                continue;
			
            auto rng = std::default_random_engine{};
            std::shuffle(AthenaLootPool.begin(), AthenaLootPool.end(), rng);
            return Def;
        }
    }

    static UFortItemDefinition* GetRandomConsumableItemDefinition()
    {
        while (true)
        {
            auto Idx = UKismetMathLibrary::RandomInteger(AthenaConsumables.size());
            auto Item = AthenaConsumables[Idx];
            auto Def = Utils::FindObjectFast<UFortItemDefinition>(Item);
            if (!Def)
                continue;

            auto rng = std::default_random_engine{};
            std::shuffle(AthenaConsumables.begin(), AthenaConsumables.end(), rng);
            return Def;
        }
    }
	
    static UFortItemDefinition* GetRandomResourceItemDefinition()
    {
        while (true)
        {
            auto Idx = UKismetMathLibrary::RandomInteger(ResourcePool.size());
            auto Item = ResourcePool[Idx];
            auto Def = Utils::FindObjectFast<UFortItemDefinition>(Item);
            if (!Def)
                continue;

            auto rng = std::default_random_engine{};
            std::shuffle(ResourcePool.begin(), ResourcePool.end(), rng);
            return Def;
        }
    }

    static UFortAmmoItemDefinition* GetRandomAmmoItemDefinition()
    {
        while (true)
        {
            auto Idx = UKismetMathLibrary::RandomInteger(AmmoPool.size());
            auto Item = AmmoPool[Idx];
            auto Def = Utils::FindObjectFast<UFortAmmoItemDefinition>(Item);
            if (!Def)
                continue;

            auto rng = std::default_random_engine{};
            std::shuffle(AmmoPool.begin(), AmmoPool.end(), rng);
            return Def;
        }
    }

    static UGameplayAbility* CreateNewInstanceOfAbility(UAbilitySystemComponent* Comp, FGameplayAbilitySpec& Spec, UGameplayAbility* Ability)
    {
        AActor* Owner = Comp->GetOwner();

        UGameplayAbility* AbilityInstance = (UGameplayAbility*)StaticConstructObjectInternal(Ability->StaticClass(), Owner, 0, 0, 0, 0, 0, 0, 0);

        auto ReplicatedInstances = (TArray_<UGameplayAbility*>*) & Spec.ReplicatedInstances;
        auto ReplicatedInstancedAbilities = (TArray_<UGameplayAbility*>*) & Comp->AllReplicatedInstancedAbilities;

        if (AbilityInstance)
        {
            /*ReplicatedInstances->AddSingle(AbilityInstance);*/
            ReplicatedInstancedAbilities->AddSingle(AbilityInstance);
            return AbilityInstance;
        }
        printf("Spawned Ability Object is Not Valid");
        return nullptr;
    }

    FGameplayAbilitySpecHandle* GiveAbility(UAbilitySystemComponent* Comp, FGameplayAbilitySpec* const Spec)
    {
        auto ActivatableItems = (TArray_<FGameplayAbilitySpec>*) & Comp->ActivatableAbilities.Items;

        FGameplayAbilitySpec* AbSpec = Spec;

        ActivatableItems->AddSingle(*AbSpec);

        CreateNewInstanceOfAbility(Comp, *AbSpec, Spec->Ability);

        MarkAbilitySpecDirty(Comp, *AbSpec, true);

        return &AbSpec->Handle;
    }

    static FVector GetRandomBattleBusLocation()
    {
        static std::vector<FVector> Locations = 
        {
            { 24426, 37710, 25025 },
            { 34278, 867, 17525 },
            { 79710, 15677, 15038 },
            { 103901, -20203, 18416, },
            { 86766, -83071, 17013 },
            { 2399, -96255, 19748 },
            { -35037, -463, 13242 },
            { 50018, 73844, 23021 },
            { 34575.64, -59968.65, 11548.21 },
            { 83375, 50856, 25341 }
        };

        static auto Location =  Locations[rand() % Locations.size()];
        return Location;
    }

    static void RemoveGameplayEffects(AFortPlayerStateAthena* PlayerState)
    {
        auto AbilityComp = PlayerState->AbilitySystemComponent;

        for (int ConnIdx = 0; ConnIdx < AbilityComp->ActivatableAbilities.Items.Num(); ConnIdx++)
        {
			auto Spec = AbilityComp->ActivatableAbilities.Items[ConnIdx];
			if (Spec.Ability)
			{
                auto Instance = Spec.GameplayEffectHandle;

                AbilityComp->RemoveActiveGameplayEffect(Instance, 1);
			}
        }
    }

    static void OnStartGame()
    {
        for (int i = 0; i < World->NetDriver->ClientConnections.Num(); i++)
        {
            auto Connection = World->NetDriver->ClientConnections[i];
            auto PlayerController = (AFortPlayerControllerAthena*)Connection->PlayerController;

            auto Pawn = (APlayerPawn_Athena_C*)PlayerController->Pawn;

            Pawn->bCanBeDamaged = false;

            Pawn->K2_SetActorLocation(ServerFunctions::GetRandomBattleBusLocation(), false, true, new FHitResult());
            ServerFunctions::GiveWeapons(PlayerController);
            ServerFunctions::GiveBackMats(PlayerController);

            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ServerFunctions::EnableDamageDelayed, (void*)Pawn, 0, 0);
        }
    }

    static void SetupDuos()
    {
        EmptyInventorys();
    }
	
    static void SetupAssassins()
    {
		
        EmptyInventorys();
		
        for (int i = 0; i < World->NetDriver->ClientConnections.Num(); i++)
        {
            auto Connection = World->NetDriver->ClientConnections[i];
            auto PlayerController = (AFortPlayerController*)Connection->PlayerController;
            auto Inventory = PlayerController->WorldInventory;
            auto Pawn = (AFortPlayerPawnAthena*)PlayerController->Pawn;
            static auto lpistol = Utils::FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_SR_Ore_T03.WID_Pistol_SemiAuto_Athena_SR_Ore_T03");

            if (!PlayerController)
                continue;

            if (!Inventory)
                continue;

            AddItemToInventory(lpistol, EFortQuickBars::Primary, PlayerController, 1, 1);
        }
        UKismetSystemLibrary::ExecuteConsoleCommand(World, FString(L"slomo 1.5"), nullptr);
    }
}
