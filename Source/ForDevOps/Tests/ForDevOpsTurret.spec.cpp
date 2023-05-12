// This is My project

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "ForDevOps/Tests/TestUtils.h"
#include "Weapon/ForDevOpsTurret.h"

// DEFINE_SPEC(FTestClassName, "Category.TestName",
//    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

BEGIN_DEFINE_SPEC(FTurret, "DevOpsGame.Turret",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;
const FTransform InitialTransform{FVector{80.0f, 70.0f, 180.0f}};
AForDevOpsTurret* SUT_Turret;

END_DEFINE_SPEC(FTurret)

using namespace ForDevOps::Test;

namespace
{
constexpr char* MapName = "/Game/Tests/EmptyTestLevel";
constexpr char* TurretBPName = "Blueprint'/Game/Weapon/BP_ForDevOpsTurret.BP_ForDevOpsTurret'";
constexpr char* TurretBPTestName = "Blueprint'/Game/Tests/BP_Test_ForDevOpsTurret.BP_Test_ForDevOpsTurret'";

}  // namespace

void FTurret::Define()
{
    Describe("Creational",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    // LevelScope("/Game/Tests/EmptyTestLevel");
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("Inventory item not exists", World);
                });

            It("Cpp instance can't created",
                [this]()
                {
                    const FString ExpectedWarningMsg = FString::Printf(
                        TEXT("SpawnActor failed because class %s is abstract"), *AForDevOpsTurret::StaticClass()->GetName());

                    AddExpectedError(ExpectedWarningMsg, EAutomationExpectedErrorFlags::Exact);
                    SUT_Turret = World->SpawnActor<AForDevOpsTurret>(AForDevOpsTurret::StaticClass(), InitialTransform);
                    TestNull("Turret doesn't exists", SUT_Turret);
                });
            It("Blueprint instance can created",
                [this]()
                {
                    SUT_Turret = CreateBlueprint<AForDevOpsTurret>(World, TurretBPName, InitialTransform);
                    TestNotNull("Turret exists", SUT_Turret);
                });
            // It("should return false when unsuccessful", [this]() { TestFalse("Execute", CustomClass->Execute()); });

            AfterEach([this]() { SpecCloseLevel(World); });
        });

    Describe("Defaults",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    // LevelScope("/Game/Tests/EmptyTestLevel");
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("Inventory item not exists", World);
                    SUT_Turret = CreateBlueprint<AForDevOpsTurret>(World, TurretBPTestName, InitialTransform);
                    TestNotNull("Turret exists", SUT_Turret);
                });

            const TArray<TTuple<int32, float>> TestData{{45, 2.0f}, {15, 3.0f}, {145, 1.0f}};
            for (const auto& Data : TestData)
            {
                const auto TestName = FString::Printf(TEXT("Ammo: %i and freq: %.0f should be set up correctryl"), Data.Key, Data.Value);
                It(TestName,
                    [this, Data]()
                    {
                        const auto [Ammo, Freq] = Data;
                        CallFuncByNameWithParams(SUT_Turret, "SetTurretData", {FString::FromInt(Ammo), FString::SanitizeFloat(Freq)});

                        const int32 AmmoCount = GetPropertyValueByName<AForDevOpsTurret, int32>(SUT_Turret, "AmmoCount");
                        TestTrueExpr(AmmoCount == Ammo);

                        const float FireFreq = GetPropertyValueByName<AForDevOpsTurret, float>(SUT_Turret, "FireFrequency");
                        TestTrueExpr(FireFreq == Freq);
                    });
            }

            // It("should return false when unsuccessful", [this]() { TestFalse("Execute", CustomClass->Execute()); });

            AfterEach([this]() { SpecCloseLevel(World); });
        });

    Describe("Ammo",
        [this]()
        {
            const int32 IntitialAmmoCount = 4;
            const float FireFreq = 1.5f;
            LatentBeforeEach(
                [this, IntitialAmmoCount, FireFreq](const FDoneDelegate& TestDone)
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("World exists", World);

                    SUT_Turret = CreateBlueprint<AForDevOpsTurret>(World, TurretBPTestName, InitialTransform);
                    TestNotNull("Turret exists", SUT_Turret);

                    CallFuncByNameWithParams(
                        SUT_Turret, "SetTurretData", {FString::FromInt(IntitialAmmoCount), FString::SanitizeFloat(FireFreq)});
                    TestDone.Execute();
                });

            const auto TestName = FString::Printf(TEXT("Should be empty after %i sec"), FMath::RoundToInt(IntitialAmmoCount * FireFreq));
            LatentIt(TestName, EAsyncExecution::ThreadPool,
                [this, IntitialAmmoCount, FireFreq](const FDoneDelegate& TestDone)
                {
                    AsyncTask(ENamedThreads::GameThread,
                        [&]()
                        {
                            const int32 AmmoCount = GetPropertyValueByName<AForDevOpsTurret, int32>(SUT_Turret, "AmmoCount");
                            TestTrueExpr(AmmoCount == IntitialAmmoCount);
                        });

                    FPlatformProcess::Sleep(IntitialAmmoCount * FireFreq + 0.5f);

                    AsyncTask(ENamedThreads::GameThread,
                        [&]()
                        {
                            const int32 AmmoCount = GetPropertyValueByName<AForDevOpsTurret, int32>(SUT_Turret, "AmmoCount");
                            TestTrueExpr(AmmoCount == 0);
                        });

                    TestDone.Execute();
                });

            LatentAfterEach(
                [this](const FDoneDelegate& TestDone)
                {
                    SpecCloseLevel(World);
                    TestDone.Execute();
                });
        });
}

#endif
