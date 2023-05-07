// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ForDevOpsGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePauseDelegate, bool, IsPaused);

UCLASS(minimalapi)
class AForDevOpsGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AForDevOpsGameMode();
    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

    UPROPERTY(BlueprintAssignable)
    FOnGamePauseDelegate OnGamePause;
};
