// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UUserWidget;
UCLASS()
class TESTTASK_PLEKHANOV_API APlayerHUD : public AHUD
{
    GENERATED_BODY()
protected:
    UPROPERTY (EditAnywhere)
        TSubclassOf<UUserWidget> WidgetHUDClass;

    virtual void BeginPlay() override;

private:
    UUserWidget* WidgetHUD;
};
