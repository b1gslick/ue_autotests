// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/ForDevOpsTurret.h"
#include "Weapon/ForDevOpsProjectile.h"
#include "Components/StaticMeshComponent.h"

AForDevOpsTurret::AForDevOpsTurret()
{
    PrimaryActorTick.bCanEverTick = false;
    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
    check(TurretMesh);
    SetRootComponent(TurretMesh);
}

void AForDevOpsTurret::BeginPlay()
{
    Super::BeginPlay();
    check(AmmoCount > 0);
    check(FireFrequency > 0.0f);

    const float FirstDelay = FireFrequency;
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AForDevOpsTurret::OnFire, FireFrequency, true, FirstDelay);
}

void AForDevOpsTurret::OnFire()
{
    if (--AmmoCount == 0)
    {
        GetWorldTimerManager().ClearTimer(FireTimerHandle);
    }
    if (GetWorld())
    {
        const auto SocketTransform = TurretMesh->GetSocketTransform("Muzzle");
        auto* ProjectileObj = GetWorld()->SpawnActorDeferred<AForDevOpsProjectile>(ProjectileClass, SocketTransform);
        if (ProjectileObj)
        {
            ProjectileObj->SetShotDirection(SocketTransform.GetRotation().GetForwardVector());
            ProjectileObj->FinishSpawning(SocketTransform);
        }
    }
}
