// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "ForDevOpsPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class AForDevOpsPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AForDevOpsPlayerController();

    /** Time Threshold to know if it was a short press */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    float ShortPressThreshold;

    /** FX Class that we will spawn when clicking */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UNiagaraSystem* FXCursor;

    /** MappingContext */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputMappingContext* DefaultMappingContext;

    /** Jump Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* SetDestinationClickAction;

    /** Jump Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* SetDestinationTouchAction;

    void ToggleGamePause();

protected:
    /** True if the controlled character should navigate to the mouse cursor. */
    uint32 bMoveToMouseCursor : 1;

    virtual void SetupInputComponent() override;

    // To add mapping context
    virtual void BeginPlay();

    /** Called for forwards/backward input */
    void MoveForward(float Value);

    /** Called for side to side input */
    void MoveRight(float Value);

    void MoveLeft(float Value);

    void MoveBack(float Value);
    /** Input handlers for SetDestination action. */
    void OnInputStarted();
    void OnSetDestinationTriggered();
    void OnSetDestinationReleased();
    void OnTouchTriggered();
    void OnTouchReleased();

private:
    FVector CachedDestination;

    bool bIsTouch;     // Is it a touch device
    float FollowTime;  // For how long it has been pressed
};
