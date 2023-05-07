// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/ForDevOpsGameSetting.h"

DEFINE_LOG_CATEGORY_STATIC(LogDevOpsGameSetting, All, All);

void UForDevOpsGameSetting::SetName(const FText& InName)
{
    Name = InName;
}

void UForDevOpsGameSetting::SetOption(const TArray<FSettingOption>& InOptions)
{
    Options = InOptions;
}

FSettingOption UForDevOpsGameSetting::GetCurrentOption() const
{
    const int32 CurrentValue = GetCurrentValue();
    const auto Option = Options.FindByPredicate([&](const auto& Opt) { return CurrentValue == Opt.Value; });
    if (!Option)
    {
        UE_LOG(LogDevOpsGameSetting, Error, TEXT("Option doesn't exist"));
        return FSettingOption{};
    }
    return *Option;
}

FText UForDevOpsGameSetting::GetName() const
{
    return Name;
}

void UForDevOpsGameSetting::AddGetter(TFunction<int32()> Func)
{
    Getter = Func;
}

void UForDevOpsGameSetting::AddSetter(TFunction<void(int32)> Func)
{
    Setter = Func;
}

void UForDevOpsGameSetting::ApplyNextOption()
{
    const int32 CurrentIndex = GetCurrentIndex();
    if (CurrentIndex == INDEX_NONE) return;

    const int32 NextIndex = (CurrentIndex + 1) % Options.Num();
    SetCurrentValue(Options[NextIndex].Value);
}

void UForDevOpsGameSetting::ApplyPrevOption()
{
    const int32 CurrentIndex = GetCurrentIndex();
    if (CurrentIndex == INDEX_NONE) return;

    const int32 PrevIndex = CurrentIndex == 0 ? Options.Num() - 1 : CurrentIndex - 1;
    SetCurrentValue(Options[PrevIndex].Value);
}

int32 UForDevOpsGameSetting::GetCurrentValue() const
{
    if (!Getter)
    {
        UE_LOG(LogDevOpsGameSetting, Error, TEXT("Getter func is not set for %s"), *Name.ToString());
        return INDEX_NONE;
    }
    return Getter();
}

void UForDevOpsGameSetting::SetCurrentValue(int32 Value)
{
    if (!Setter)
    {
        UE_LOG(LogDevOpsGameSetting, Error, TEXT("Setter func is not set for %s"), *Name.ToString());
        return;
    }
    Setter(Value);
}

int32 UForDevOpsGameSetting::GetCurrentIndex() const
{
    const int32 CurrentValue = GetCurrentValue();

    return Options.IndexOfByPredicate([&](const auto& Opt) { return CurrentValue == Opt.Value; });
}
