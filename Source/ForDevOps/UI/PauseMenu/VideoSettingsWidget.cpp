// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PauseMenu/VideoSettingsWidget.h"
#include "Settings/ForDevOpsGameUserSettings.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "UI/PauseMenu/SettingOptionWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogVideoSettingsWidget, All, All);

void UVideoSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    auto* UserSettings = UForDevOpsGameUserSettings::Get();
    if (!UserSettings)
    {
        UE_LOG(LogVideoSettingsWidget, Error, TEXT("UForDevOpsGameUserSettings is nullptr"));
        return;
    }
    UserSettings->LoadSettings();

    const auto VideoSettings = UserSettings->GetVideoSettings();

    check(VideoSettingsContainer);
    VideoSettingsContainer->ClearChildren();

    for (auto* Setting : VideoSettings)
    {
        const auto SettingWidget = CreateWidget<USettingOptionWidget>(this, SettingOptionWidgetClass);
        check(SettingWidget);
        SettingWidget->Init(Setting);
        VideoSettingsContainer->AddChild(SettingWidget);
    }

    check(RunBenchmarkButton);
    RunBenchmarkButton->OnClicked.AddDynamic(this, &ThisClass::OnBenchmark);

    UserSettings->OnVideoSettingsUpdated.AddUObject(this, &ThisClass::OnVideoSettingsUpdated);
}

void UVideoSettingsWidget::OnBenchmark()
{
    if (auto* UserSettings = UForDevOpsGameUserSettings::Get())
    {
        UserSettings->RunBenchmark();
    }
}

void UVideoSettingsWidget::OnVideoSettingsUpdated()
{
    if (!VideoSettingsContainer) return;
    for (auto* Widget : VideoSettingsContainer->GetAllChildren())
    {
        if (auto* SettingOptionWidget = Cast<USettingOptionWidget>(Widget))
        {
            SettingOptionWidget->UpdateTexts();
        }
    }
}
