// This is My project

#if WITH_AUTOMATION_TESTS

#include "ForDevOps/Tests/ForDevOpsScreenshots.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "ForDevOps/Tests/TestUtils.h"
#include "AutomationBlueprintFunctionLibrary.h"
#include "Camera/CameraActor.h"
#include "BufferVisualizationData.h"
#include "Engine/DamageEvents.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRenderingShouldBeCorrect, "DevOpsGame.Screenshots.RenderingShouldBeCorrect",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMainPlayerHUDShouldBeRendered, "DevOpsGame.Screenshots.FMainPlayerHUDShouldBeRendered",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthWidgetShouldBeRenderedCorrectlyAfterDamage,
    "DevOpsGame.Screenshots.FHealthWidgetShouldBeRenderedCorrectlyAfterDamage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingWidgetBeRenderedWhenGamePaused, "DevOpsGame.Screenshots.FSettingWidgetBeRenderedWhenGamePaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

using namespace ForDevOps::Test;

bool FRenderingShouldBeCorrect::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/TopDownMap");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exixts", World)) return false;

    const FTransform Transform{FVector{740.f, 2270.0f, 60.0f}};
    ACameraActor* Camera = World->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), Transform);
    if (!TestNotNull("Camera exists", Camera)) return false;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller exusts", PC)) return false;

    PC->SetViewTarget(Camera);
    ADD_LATENT_AUTOMATION_COMMAND(FTakeGameScreenshotLatentCommand("rendering_check_screenshot"));
    return true;
}

bool FMainPlayerHUDShouldBeRendered::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/TopDownMap");
    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("main_player_hud_screenshot"));
    return true;
}

bool FHealthWidgetShouldBeRenderedCorrectlyAfterDamage::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/TopDownMap");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exixts", World)) return false;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller exusts", PC)) return false;
    if (!TestNotNull("Pawn controller exusts", PC->GetPawn())) return false;

    const float DamageAmount = 40.0f;

    PC->GetPawn()->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("health_widget_screenshot"));
    return true;
}
bool FSettingWidgetBeRenderedWhenGamePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/TopDownMap");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exixts", World)) return false;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller exusts", PC)) return false;

    PausePressed(PC->InputComponent);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("setting_widget_screenshot"));
    return true;
}

#endif
