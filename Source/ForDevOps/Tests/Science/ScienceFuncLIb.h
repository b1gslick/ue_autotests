// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScienceFuncLIb.generated.h"

UCLASS()
class FORDEVOPS_API UScienceFuncLIb : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = Science)
    static int32 Fibonacci(int32 Value);
};
