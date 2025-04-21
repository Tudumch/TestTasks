// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "BR_Button_BridgeMode.generated.h"

class ABR_Bridge;
/**
 * 
 */
UCLASS()
class BR_API UBR_Button_BridgeMode : public UButton
{
	GENERATED_BODY()

public:
    UBR_Button_BridgeMode();
    
    UFUNCTION()
    void OnButtonClicked();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ABR_Bridge> BridgeClass;
};
