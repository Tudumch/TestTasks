// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "BR_Button_DemolishMode.generated.h"

/**
 * 
 */
UCLASS()
class BR_API UBR_Button_DemolishMode : public UButton
{
	GENERATED_BODY()

public:
    UBR_Button_DemolishMode();
    
    UFUNCTION()
    void OnButtonClicked();
};
