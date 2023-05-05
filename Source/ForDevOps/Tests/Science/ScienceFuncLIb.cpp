// Fill out your copyright notice in the Description page of Project Settings.

#include "Tests/Science/ScienceFuncLIb.h"

DEFINE_LOG_CATEGORY_STATIC(LogFibonacci, All, All);

int32 UScienceFuncLIb::Fibonacci(int32 Value)
{
    // check(Value >= 0);
    if (Value < 0)
    {
        UE_LOG(LogFibonacci, Error, TEXT("Invalid input for Fibonacci: %i"), Value);
    }
    return Value <= 1 ? Value : Fibonacci(Value - 1) + Fibonacci(Value - 2);
}