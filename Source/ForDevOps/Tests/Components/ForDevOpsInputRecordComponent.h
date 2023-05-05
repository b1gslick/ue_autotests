// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ForDevOps/Tests/Utils/InputRecordingUtils.h"
#include "ForDevOpsInputRecordComponent.generated.h"

class UPlayerInput;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FORDEVOPS_API UForDevOpsInputRecordComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UForDevOpsInputRecordComponent();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditAnywhere)
    FString FileName{"CharacterTestInput"};

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    UPlayerInput* PlayerInput;

    FInputData InputData;
    FBindingsData MakeBindingsData() const;
    FString GenerateFileName() const;
};
