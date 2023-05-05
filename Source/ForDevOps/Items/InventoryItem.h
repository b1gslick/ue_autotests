// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForDevOpsTypes.h"
#include "InventoryItem.generated.h"

class USphereComponent;

UCLASS(Abstract)  // forbiden to spawn class on scene
class FORDEVOPS_API AInventoryItem : public AActor
{
    GENERATED_BODY()

public:
    AInventoryItem();

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FInventoryData InventoryData;
};
