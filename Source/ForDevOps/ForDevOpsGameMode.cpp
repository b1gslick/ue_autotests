// Copyright Epic Games, Inc. All Rights Reserved.

#include "ForDevOpsGameMode.h"
#include "ForDevOpsPlayerController.h"
#include "ForDevOpsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AForDevOpsGameMode::AForDevOpsGameMode()
{
    // use our custom PlayerController class
    PlayerControllerClass = AForDevOpsPlayerController::StaticClass();

    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
    if (PlayerPawnBPClass.Class != nullptr)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }

    // set default controller to our Blueprinted controller
    static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(
        TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
    if (PlayerControllerBPClass.Class != NULL)
    {
        PlayerControllerClass = PlayerControllerBPClass.Class;
    }
}

bool AForDevOpsGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const bool Succeed = Super::SetPause(PC, CanUnpauseDelegate);
    if (Succeed)
    {
        OnGamePause.Broadcast(true);
    }
    return false;
}

bool AForDevOpsGameMode::ClearPause()
{
    const bool Succeed = Super::ClearPause();
    if (Succeed)
    {
        OnGamePause.Broadcast(false);
    }
    return false;
}
