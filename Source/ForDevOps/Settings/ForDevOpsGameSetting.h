// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ForDevOpsGameSetting.generated.h"

struct FSettingOption
{
    FText Name;
    int32 Value;
};

UCLASS()
class UForDevOpsGameSetting : public UObject
{
    GENERATED_BODY()

public:
    void SetName(const FText& Name);
    void SetOption(const TArray<FSettingOption>& Options);

    FSettingOption GetCurrentOption() const;
    FText GetName() const;
    void AddGetter(TFunction<int32()> Func);
    void AddSetter(TFunction<void(int32)> Func);

    void ApplyNextOption();
    void ApplyPrevOption();

private:
    FText Name;
    TArray<FSettingOption> Options;
    TFunction<int32()> Getter;
    TFunction<void(int32)> Setter;

    int32 GetCurrentValue() const;
    void SetCurrentValue(int32 Value);
    int32 GetCurrentIndex() const;
};
