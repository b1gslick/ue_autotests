// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ForDevOpsTypes.h"
#include "InventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FORDEVOPS_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

    bool TryToAddItem(const FInventoryData& Data);

    UFUNCTION(BlueprintCallable)
    int32 GetInventoryAmountByType(EInventoryItemType Type) const;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TMap<EInventoryItemType, int32> InventoryLimits;

private:
    TMap<EInventoryItemType, int32> Inventory;
};