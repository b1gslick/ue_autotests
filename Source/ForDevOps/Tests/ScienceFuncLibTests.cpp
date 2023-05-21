// Fill out your copyright notice in the Description page of Project Settings.

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TEST)

#include "Tests/ScienceFuncLibTests.h"
#include "Tests/TestUtils.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "ForDevOps/Tests/Science/ScienceFuncLIb.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciSimple, "DevOpsGame.Science.Fibonacci.Simple",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciStress, "DevOpsGame.Science.Fibonacci.Stress",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::LowPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciLogHasErrors, "DevOpsGame.Science.Fibonacci.LogHasErrors",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

DEFINE_SPEC(FFactorial, "DevOpsGame.Science.Factorial",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

bool FFibonacciSimple::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci simple testing");
    // 0 1 1 2 3 5 8 13 .....

    const TArray<ForDevOps::Test::TestPayload<int32, int32>> TestData{{0, 0}, {1, 1}, {2, 1}, {3, 2}, {4, 3}, {5, 5}};
    for (const auto Data : TestData)
    {
        // TestTrueExpr(UScienceFuncLIb::Fibonacci(Data.TestValue) == Data.ExpectedValue);
        const FString InfoString = FString::Printf(TEXT("test value: %i, expected value: %i"), Data.TestValue, Data.ExpectedValue);
        TestEqual(InfoString, UScienceFuncLIb::Fibonacci(Data.TestValue), Data.ExpectedValue);
    }

    return true;
}

bool FFibonacciStress::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci stress testing");
    /*
    for (int32 i = 2; i < 40; ++i)
    {
        TestTrueExpr(UScienceFuncLIb::Fibonacci(i) ==  //
                     UScienceFuncLIb::Fibonacci(i - 1) + UScienceFuncLIb::Fibonacci(i - 2));
    }
    */

    int32 PrevPrevValue = 0;
    int32 PrevValue = 1;

    for (int32 i = 2; i < 40; ++i)
    {
        const int32 CurrentValue = UScienceFuncLIb::Fibonacci(i);
        TestTrueExpr(CurrentValue == PrevPrevValue + PrevValue);

        PrevPrevValue = PrevValue;
        PrevValue = CurrentValue;
    }
    return true;
}

bool FFibonacciLogHasErrors::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci negative number on input produces error");
    AddExpectedError("Invalid input for Fibonacci", EAutomationExpectedErrorFlags::Contains, 1);
    UScienceFuncLIb::Fibonacci(-10);
    return true;
}

void FFactorial::Define()
{
    Describe("Corner cases",
        [this]()
        {
            It("Factorial of 0 should return 1", [this]() { TestTrueExpr(UScienceFuncLIb::Factorial(0) == 0); });
            It("Factorial of 1 should return 1", [this]() { TestTrueExpr(UScienceFuncLIb::Factorial(1) == 1); });
            It("Factorial of -1 should return -1", [this]() { TestTrueExpr(UScienceFuncLIb::Factorial(-1) == -1); });
            // It("should return false when unsuccessful", [this]() { TestFalse("Execute", CustomClass->Execute()); });
        });

    Describe("Normal cases",
        [this]()
        {
            It("Factorial of 2 should return 2", [this]() { TestTrueExpr(UScienceFuncLIb::Factorial(2) == 2); });
            It("Factorial of 3 should return 6", [this]() { TestTrueExpr(UScienceFuncLIb::Factorial(3) == 6); });
            It("Factorial of 4 should return 24", [this]() { TestTrueExpr(UScienceFuncLIb::Factorial(4) == 24); });

            using namespace ForDevOps::Test;
            const TArray<TestPayload<int32, int32>> TestData{{5, 120}, {6, 720}, {7, 5040}, {8, 40320}};
            for (const auto& Data : TestData)
            {
                It(FString::Printf(TEXT("Factorial of %i should return %i"), Data.TestValue, Data.ExpectedValue),
                    [this, Data]() { TestTrueExpr(UScienceFuncLIb::Factorial(Data.TestValue) == Data.ExpectedValue); });
            }
        });
}

#endif
