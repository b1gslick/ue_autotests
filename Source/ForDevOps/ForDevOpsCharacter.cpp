// Copyright Epic Games, Inc. All Rights Reserved.

#include "ForDevOpsCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "ForDevOps/Components/InventoryComponent.h"
#include "TimerManager.h"

AForDevOpsCharacter::AForDevOpsCharacter()
{
    // Set size for player capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate character to camera direction
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;  // Rotate character to moving direction
    GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    // Create a camera boom...
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->SetUsingAbsoluteRotation(true);  // Don't want arm to rotate when character does
    CameraBoom->TargetArmLength = 800.f;
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    CameraBoom->bDoCollisionTest = false;  // Don't want to pull camera in when it collides with level

    // Create a camera...
    TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TopDownCameraComponent->bUsePawnControlRotation = false;  // Camera does not rotate relative to arm

    // Activate ticking in order to update the cursor every frame.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
}

void AForDevOpsCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AForDevOpsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAxis("MoveForward", this, &AForDevOpsCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AForDevOpsCharacter::MoveRight);

    PlayerInputComponent->BindAxis("MoveBack", this, &AForDevOpsCharacter::MoveBack);
    PlayerInputComponent->BindAxis("MoveLeft", this, &AForDevOpsCharacter::MoveLeft);
}

void AForDevOpsCharacter::MoveForward(float Value)
{
    if (Controller && Value != 0.0f)
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void AForDevOpsCharacter::MoveRight(float Value)
{
    if (Controller && Value != 0.0f)
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get right vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

void AForDevOpsCharacter::MoveBack(float Value)
{
    if (Controller && Value != 0.0f)
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, -1);

        // get right vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

void AForDevOpsCharacter::MoveLeft(float Value)
{
    if (Controller && Value != 0.0f)
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, -1);

        // get right vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

void AForDevOpsCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthData.MaxHealth > 0.0f);
    Health = HealthData.MaxHealth;

    OnTakeAnyDamage.AddDynamic(this, &AForDevOpsCharacter::OnAnyDamageReceived);
}

float AForDevOpsCharacter::GetHealthPercent() const
{
    return Health / HealthData.MaxHealth;
}

void AForDevOpsCharacter::OnAnyDamageReceived(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    const auto IsAlive = [&]() { return Health > 0.0f; };

    if (Damage <= 0.0f || !IsAlive()) return;

    Health = FMath::Clamp(Health - Damage, 0.0f, HealthData.MaxHealth);

    if (IsAlive())
    {
        GetWorldTimerManager().SetTimer(HealTimerHandle, this, &AForDevOpsCharacter::OnHealing, HealthData.HealRate, true, -1.0f);
    }
    else
    {
        OnDeath();
    }
}

void AForDevOpsCharacter::OnHealing()
{
    Health = FMath::Clamp(Health + HealthData.HealModifier, 0.0f, HealthData.MaxHealth);
    if (FMath::IsNearlyEqual(Health, HealthData.MaxHealth))
    {
        Health = HealthData.MaxHealth;
        GetWorldTimerManager().ClearTimer(HealTimerHandle);
    }
}

void AForDevOpsCharacter::OnDeath()
{
    GetWorldTimerManager().ClearTimer(HealTimerHandle);

    check(GetCharacterMovement());
    check(GetCapsuleComponent());
    check(GetMesh());

    GetCharacterMovement()->DisableMovement();
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    SetLifeSpan(HealthData.LifeSpan);
}