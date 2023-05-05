// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/InventoryItem.h"
#include "Gameframework/Pawn.h"
#include "Components/SphereComponent.h"
#include "ForDevOps/Components/InventoryComponent.h"

AInventoryItem::AInventoryItem()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    check(CollisionComponent);
    CollisionComponent->InitSphereRadius(30.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    CollisionComponent->SetGenerateOverlapEvents(true);
    SetRootComponent(CollisionComponent);
}

void AInventoryItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (const auto Pawn = Cast<APawn>(OtherActor))
    {
        if (const auto InvComp = Pawn->FindComponentByClass<UInventoryComponent>())
        {
            if (InvComp->TryToAddItem(InventoryData))
            {
                Destroy();
            }
        }
    }
}
