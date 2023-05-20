// This is My project

#if WITH_AUTOMATION_TESTS

#include "ForDevOps/Tests/UITests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "ForDevOps/Tests/TestUtils.h"
#include "UI/PauseMenu/PauseMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "UI/PauseMenu/VideoSettingsWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Settings/ForDevOpsGameUserSettings.h"
#include "Settings/ForDevOpsGameSetting.h"
#include "UI/PauseMenu/SettingOptionWidget.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldBeVisibleOnGamePaused, "DevOpsGame.UI.PauseMenuShouldBeVisibleOnGamePaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldBeCollapsedOnGameUnPaused, "DevOpsGame.UI.PauseMenuShouldBeCollapsedOnGameUnPaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingCanBeApplied, "DevOpsGame.UI.SettingCanBeApplied",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllVidioSettingsExist, "DevOpsGame.UI.AllVidioSettingsExist",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAutoBenchmarkShouldWork, "DevOpsGame.UI.AutoBenchmarkShouldWork",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::MediumPriority);

using namespace ForDevOps::Test;

namespace
{

void NextSettingClick(int32 SettingIndex)
{
    const auto* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    const auto* VerticalBox = Cast<UVerticalBox>(FindWidgetByName(VideoSettingsWidget, "VideoSettingsContainer"));
    const auto* SettingOptionWidget = Cast<USettingOptionWidget>(VerticalBox->GetChildAt(SettingIndex));
    const auto* NextSettingButton = Cast<UButton>(FindWidgetByName(SettingOptionWidget, "NextSettingButton"));
    NextSettingButton->OnClicked.Broadcast();
}

void DoBenchmarkClick()
{
    const auto* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    const auto* BenchmarkButton = Cast<UButton>(FindWidgetByName(VideoSettingsWidget, "RunBenchmarkButton"));
    BenchmarkButton->OnClicked.Broadcast();
}

}  // namespace

bool FPauseMenuShouldBeVisibleOnGamePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/TopDownMap");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    const UPauseMenuWidget* PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();
    TestTrueExpr(PauseMenuWidget != nullptr);

    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);
    PausePressed(PC->InputComponent);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Visible);
    return true;
}

bool FPauseMenuShouldBeCollapsedOnGameUnPaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/TopDownMap");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    const UPauseMenuWidget* PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();
    TestTrueExpr(PauseMenuWidget != nullptr);

    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);
    PausePressed(PC->InputComponent);
    PausePressed(PC->InputComponent);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);
    return true;
}

bool FAllVidioSettingsExist::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/TopDownMap");

    const UVideoSettingsWidget* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    TestTrueExpr(VideoSettingsWidget != nullptr);

    const UButton* BencmarkButton = Cast<UButton>(FindWidgetByName(VideoSettingsWidget, "RunBenchmarkButton"));
    TestTrueExpr(BencmarkButton != nullptr);

    const UVerticalBox* VerticalBox = Cast<UVerticalBox>(FindWidgetByName(VideoSettingsWidget, "VideoSettingsContainer"));
    TestTrueExpr(VerticalBox != nullptr);

    const auto& VideoSettings = UForDevOpsGameUserSettings::Get()->GetVideoSettings();
    TestTrueExpr(VerticalBox->GetChildrenCount() == VideoSettings.Num());

    return true;
}

bool FSettingCanBeApplied::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/TopDownMap");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);
    PausePressed(PC->InputComponent);

    const int32 IndexGlobalElemenationInSettings = 2;

    const auto& VideoSettings = UForDevOpsGameUserSettings::Get()->GetVideoSettings();

    const int32 GIQualityBefore = UForDevOpsGameUserSettings::Get()->GetGlobalIlluminationQuality();
    TestTrueExpr(GIQualityBefore == VideoSettings[IndexGlobalElemenationInSettings]->GetCurrentOption().Value);

    NextSettingClick(IndexGlobalElemenationInSettings);
    const int32 GIQualityAfter = UForDevOpsGameUserSettings::Get()->GetGlobalIlluminationQuality();
    TestTrueExpr(GIQualityBefore != GIQualityAfter);
    TestTrueExpr(GIQualityAfter == VideoSettings[IndexGlobalElemenationInSettings]->GetCurrentOption().Value);

    UForDevOpsGameUserSettings::Get()->SetGlobalIlluminationQuality(GIQualityBefore);
    UForDevOpsGameUserSettings::Get()->ApplySettings(false);
    return true;
}

bool FAutoBenchmarkShouldWork::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/TopDownMap");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);
    PausePressed(PC->InputComponent);

    DoBenchmarkClick();

    TArray<int32> SavedSettingValues;
    const auto& VideoSettings = UForDevOpsGameUserSettings::Get()->GetVideoSettings();
    for (const auto& Setting : VideoSettings)
    {
        SavedSettingValues.Add(Setting->GetCurrentOption().Value);
    }

    // random clicks

    for (int32 i = 0; i < VideoSettings.Num(); ++i)
    {
        NextSettingClick(i);
    }

    for (int32 i = 0; i < VideoSettings.Num(); ++i)
    {
        TestTrueExpr(SavedSettingValues[i] != VideoSettings[i]->GetCurrentOption().Value);
    }

    DoBenchmarkClick();

    for (int32 i = 0; i < VideoSettings.Num(); ++i)
    {
        TestTrueExpr(SavedSettingValues[i] == VideoSettings[i]->GetCurrentOption().Value);
    }

    return true;
}
#endif
