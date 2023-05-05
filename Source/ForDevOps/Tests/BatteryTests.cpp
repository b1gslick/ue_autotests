// My game copyright

#if WITH_AUTOMATION_TESTS

#include "ForDevOps/Tests/BatteryTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Items/Battery.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatteryTests, "DevOpsGame.Items.Battery",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FBatteryTests::RunTest(const FString& Parameters)
{
    using namespace ForDevOps;

    AddInfo("Battery with default ctor");

    const Battery SUT_BatteryDefault;
    TestTrueExpr(FMath::IsNearlyEqual(SUT_BatteryDefault.GetPercent(), 1.0f));
    TestTrueExpr(SUT_BatteryDefault.GetColor() == FColor::Green);
    TestTrueExpr(SUT_BatteryDefault.ToString().Equals("100%"));

    AddInfo("Battery with custom ctor");

    const Battery SUT_BatteryParametres{1.0f};
    TestTrueExpr(FMath::IsNearlyEqual(SUT_BatteryParametres.GetPercent(), 1.0f));
    TestTrueExpr(SUT_BatteryParametres.GetColor() == FColor::Green);
    TestTrueExpr(SUT_BatteryParametres.ToString().Equals("100%"));

    AddInfo("Battery with custom ctor with no 0.1%");

    const Battery SUT_BatteryTenPercent{0.1f};
    TestTrueExpr(FMath::IsNearlyEqual(SUT_BatteryTenPercent.GetPercent(), 0.1f));
    TestTrueExpr(SUT_BatteryTenPercent.GetColor() == FColor::Red);
    TestTrueExpr(SUT_BatteryTenPercent.ToString().Equals("10%"));

    const auto BatteryTestFunc = [this](float Percent, const FColor& Color, const FString& PercentStirng)
    {
        const Battery SUT_BatteryObject{Percent};
        TestTrueExpr(FMath::IsNearlyEqual(SUT_BatteryObject.GetPercent(), FMath::Clamp(Percent, 0.0f, 1.0f)));
        TestTrueExpr(SUT_BatteryObject.GetColor() == Color);
        TestTrueExpr(SUT_BatteryObject.ToString().Equals(PercentStirng));
    };

    BatteryTestFunc(1.0f, FColor::Green, "100%");
    BatteryTestFunc(0.46f, FColor::Yellow, "46%");
    BatteryTestFunc(0.2f, FColor::Red, "20%");
    BatteryTestFunc(3000.0f, FColor::Green, "100%");
    BatteryTestFunc(-3000.0f, FColor::Red, "0%");

    AddInfo("Battery charge / uncharge");
    Battery SUT_ForCharge{0.6f};
    TestTrueExpr(FMath::IsNearlyEqual(SUT_ForCharge.GetPercent(), 0.6f));
    SUT_ForCharge.UnCharge();
    TestTrueExpr(FMath::IsNearlyEqual(SUT_ForCharge.GetPercent(), 0.5f));
    SUT_ForCharge.Charge();
    TestTrueExpr(FMath::IsNearlyEqual(SUT_ForCharge.GetPercent(), 0.6f));

    AddInfo("Battery charge / uncharge, corner cases");

    for (int32 i = 0; i < 100; ++i)
    {
        SUT_ForCharge.UnCharge();
    }
    TestTrueExpr(FMath::IsNearlyEqual(SUT_ForCharge.GetPercent(), 0.0f));

    for (int32 i = 0; i < 100; ++i)
    {
        SUT_ForCharge.Charge();
    }
    TestTrueExpr(FMath::IsNearlyEqual(SUT_ForCharge.GetPercent(), 1.0f));

    AddInfo("Battery comparison");
    Battery SUT_BatteryLow{0.3f};
    Battery SUT_BatteryHigh{0.9f};

    TestTrueExpr(SUT_BatteryHigh >= SUT_BatteryLow);
    // TestTrueExpr(FMath::Max(SUT_BatteryHigh, SUT_BatteryLow) == SUT_BatteryHigh);

    AddInfo("Battery in memory comparison");

    TestNotSame("Not the same batareies", SUT_BatteryLow, SUT_BatteryHigh);

    Battery SUT_BatteryHighDub{0.9f};
    TestNotSame("Not the same batareies", SUT_BatteryHighDub, SUT_BatteryHigh);

    Battery& SUT_BatteryHighRef = SUT_BatteryHigh;
    TestSame("The same batteries", SUT_BatteryHighRef, SUT_BatteryHigh);
    return true;
}

#endif