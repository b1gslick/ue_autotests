// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForDevOpsTurret.generated.h"

class UStaticMeshComponent;
class AForDevOpsProjectile;

UCLASS(Abstract)
class FORDEVOPS_API AForDevOpsTurret : public AActor
{
    GENERATED_BODY()

public:
    AForDevOpsTurret();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* TurretMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    int32 AmmoCount{10};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Units = s))
    float FireFrequency{1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<AForDevOpsProjectile> ProjectileClass;

private:
    FTimerHandle FireTimerHandle;

    void OnFire();
};
