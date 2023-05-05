// Fill out your copyright notice in the Description page of Project Settings.

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TEST)

#include "Tests/SandboxTests.h"
#include "Tests/TestUtils.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "DevOpsGame.Math.MaxInt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FMathMaxInt::RunTest(const FString& Parameters)
{
    AddInfo("Max [int] func testing");
    using namespace ForDevOps::Test;

    typedef TArray<TestPayload<TInterval<int32>, int32>> MaxIntTestPayload;
    // clang-format off
    const MaxIntTestPayload TestData{
        {{13,25}, 25},
        {{25,25}, 25},
        {{0,123}, 123},
        {{0,0}, 0},
        {{-2345,0}, 0},
        {{-45,-67}, -45},
        {{-78,34}, 34},
    };
    // clang-format on

    for (const auto Data : TestData)
    {
        TestTrueExpr(FMath::Max(Data.TestValue.Min, Data.TestValue.Max) == Data.ExpectedValue);
    }

    return true;
}

#endif