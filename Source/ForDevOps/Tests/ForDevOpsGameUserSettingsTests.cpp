// This is My project

#if WITH_AUTOMATION_TESTS

#include "ForDevOps/Tests/ForDevOpsGameUserSettingsTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "ForDevOps/Tests/TestUtils.h"
#include "Settings/ForDevOpsGameUserSettings.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingsShouldExist, "DevOpsGame.GameUserSettings.SettingsShouldExist",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace ForDevOps::Test;

bool FSettingsShouldExist::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/TopDownMap");

    TestTrueExpr(UForDevOpsGameUserSettings::Get() != nullptr);
    TestTrueExpr(UForDevOpsGameUserSettings::Get()->GetVideoSettings().Num() == 4);

    return true;
}

#endif
