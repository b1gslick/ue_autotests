// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "ForDevOpsGameUserSettings.generated.h"

class UForDevOpsGameSetting;

DECLARE_MULTICAST_DELEGATE(FOnSettingsUpdatedDelegate);

UCLASS() class FORDEVOPS_API UForDevOpsGameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:
    UForDevOpsGameUserSettings();
    static UForDevOpsGameUserSettings* Get();

    const TArray<UForDevOpsGameSetting*>& GetVideoSettings() const;

    void RunBenchmark();

    FOnSettingsUpdatedDelegate OnVideoSettingsUpdated;

private:
    UPROPERTY()
    TArray<UForDevOpsGameSetting*> VideoSettings;
};
