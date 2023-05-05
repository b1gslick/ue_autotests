// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ForDevOps/Tests/Utils/InputRecordingUtils.h"

namespace ForDevOps
{
namespace Test
{
class JsonUtils
{
public:
    static bool WriteInputData(const FString& FileName, const FInputData& InputData);
    static bool ReadInputData(const FString& FileName, FInputData& InputData);
};
}  // namespace Test
}  // namespace ForDevOps
