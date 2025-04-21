// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Actors/Buildings/BR_BuildingBase.h"

#include "BR_Button_BuildingSelector.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FButtonClicked, UBR_Button_BuildingSelector*, button, 
TSubclassOf<ABR_BuildingBase>, BuildingClass);

UCLASS()
class BR_API UBR_Button_BuildingSelector : public UButton
{
	GENERATED_BODY()

public:
    UBR_Button_BuildingSelector();
    
    UPROPERTY(EditAnywhere)
    TSubclassOf<ABR_BuildingBase> BuildingClass;
    
    UFUNCTION()
    void OnButtonClicked();
};
