// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ForDevOps/ForDevOpsTypes.h"
#include "ForDevOpsCharacter.generated.h"

class UInventoryComponent;
class UDamageType;
class AController;
class USpringArmComponent;
class UCameraComponent;
class UInputComponent;

UCLASS(Blueprintable)
class AForDevOpsCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AForDevOpsCharacter();

    // Called every frame.
    virtual void Tick(float DeltaSeconds) override;

    /** Returns TopDownCameraComponent subobject **/
    FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

    UFUNCTION(BlueprintCallable, Category = "Health" /*, meta = (BlueprintProtected = "true")*/)
    float GetHealthPercent() const;

private:
    /** Top down camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* TopDownCameraComponent;

    /** Camera boom positioning the camera above the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UInventoryComponent* InventoryComponent;

    /** Called for forwards/backward input */
    void MoveForward(float Value);

    /** Called for side to side input */
    void MoveRight(float Value);

    void MoveLeft(float Value);

    void MoveBack(float Value);
    // APawn interface
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    // End of APawn interface

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    FHealthData HealthData;

    virtual void BeginPlay() override;

private:
    float Health{0.0f};
    FTimerHandle HealTimerHandle;

    UFUNCTION()
    void OnAnyDamageReceived(
        AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

    void OnHealing();
    void OnDeath();
};
