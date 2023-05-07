// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UButton;

/**
 *
 */
UCLASS()
class FORDEVOPS_API UPauseMenuWidget : public UUserWidget
{
    GENERATED_BODY()
protected:
    UPROPERTY(meta = (BindWidget))
    UButton* CloseMenuButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnCloseMenu();
};
