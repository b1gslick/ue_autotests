// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/ForDevOpsGameUserSettings.h"
#include "Settings/ForDevOpsGameSetting.h"

#define BIND_SETTINGS_FUNC(FUNC) \
    [&](int32 Level)             \
    {                            \
        FUNC(Level);             \
        ApplySettings(false);    \
    }

#define LOCTEXT_NAMESPACE "GameUserSettings"

UForDevOpsGameUserSettings::UForDevOpsGameUserSettings()
{
    const TArray<FSettingOption> VideoSettingOptions = {

        {LOCTEXT("VFXQualityLow_Loc", "Low"), 0},
        {LOCTEXT("VFXQualityMedium_Loc", "Medium"), 1},
        {LOCTEXT("VFXQualityHigh_Loc", "High"), 2},
        {LOCTEXT("VFXQualityEpic_Loc", "Epic"), 3},
    };
    {

        auto* Setting = NewObject<UForDevOpsGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("AntiAliasing_Loc", "Anti-Aliasing"));
        Setting->SetOption(VideoSettingOptions);
        Setting->AddGetter([&]() { return GetAntiAliasingQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetAntiAliasingQuality));
        VideoSettings.Add(Setting);
    }
    {

        auto* Setting = NewObject<UForDevOpsGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("Textures_Loc", "Textures"));
        Setting->SetOption(VideoSettingOptions);
        Setting->AddGetter([&]() { return GetTextureQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetTextureQuality));
        VideoSettings.Add(Setting);
    }
    {

        auto* Setting = NewObject<UForDevOpsGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("GlobalIllumination_Loc", "Global Illumination"));
        Setting->SetOption(VideoSettingOptions);
        Setting->AddGetter([&]() { return GetGlobalIlluminationQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetGlobalIlluminationQuality));
        VideoSettings.Add(Setting);
    }
    {

        auto* Setting = NewObject<UForDevOpsGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("Shadows_Loc", "Shadows"));
        Setting->SetOption(VideoSettingOptions);
        Setting->AddGetter([&]() { return GetShadowQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetShadowQuality));
        VideoSettings.Add(Setting);
    }
    {

        auto* Setting = NewObject<UForDevOpsGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("PostProccesing_Loc", "Post Processing"));
        Setting->SetOption(VideoSettingOptions);
        Setting->AddGetter([&]() { return GetPostProcessingQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetPostProcessingQuality));
        VideoSettings.Add(Setting);
    }
}

UForDevOpsGameUserSettings* UForDevOpsGameUserSettings::Get()
{
    return GEngine ? Cast<UForDevOpsGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

const TArray<UForDevOpsGameSetting*>& UForDevOpsGameUserSettings::GetVideoSettings() const
{
    return VideoSettings;
}

void UForDevOpsGameUserSettings::RunBenchmark()
{
    RunHardwareBenchmark();
    ApplySettings(false);
    OnVideoSettingsUpdated.Broadcast();
}

#undef LOCTEXT_NAMESPACE