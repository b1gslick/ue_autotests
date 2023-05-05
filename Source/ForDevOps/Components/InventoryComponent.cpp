// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryComponent::TryToAddItem(const FInventoryData& Data)
{
    if (Data.Score < 0) return false;

    if (!Inventory.Contains(Data.Type))
    {
        Inventory.Add(Data.Type, 0);
    }
    const auto NextScore = Inventory[Data.Type] + Data.Score;
    if (NextScore > InventoryLimits[Data.Type]) return false;

    Inventory[Data.Type] = NextScore;

    return true;
}

int32 UInventoryComponent::GetInventoryAmountByType(EInventoryItemType Type) const
{
    return Inventory.Contains(Type) ? Inventory[Type] : 0;
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

#if !UE_BUILD_SHIPPING
    const UEnum* InvEnum = StaticEnum<EInventoryItemType>();
    check(InvEnum);
    for (int32 i = 0; i < InvEnum->NumEnums() - 1; ++i)
    {
        const EInventoryItemType EnumElem = static_cast<EInventoryItemType>(i);
        const FString EnumElemName = UEnum::GetValueAsString(EnumElem);
        const bool LimitCheckCond = InventoryLimits.Contains(EnumElem) && InventoryLimits[EnumElem] >= 0;
        checkf(LimitCheckCond, TEXT("Limit for %s doesn't exist or less then Zero"), *EnumElemName);
    }
#endif
}