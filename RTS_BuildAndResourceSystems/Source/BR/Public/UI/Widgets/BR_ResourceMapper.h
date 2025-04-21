// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Library/ResourcesList.h"
#include "BR_ResourceMapper.generated.h"

class UButton;
class UTextBlock;
class ABR_GridActor;

UCLASS()
class BR_API UBR_ResourceMapper : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void UpdateResources(const TMap<EResourceTypes, int32> &ResourceMap) const;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    
protected:
    UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
    UTextBlock* EnergyAmount;
    UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
    UTextBlock* WaterAmount;
    UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
    UTextBlock* OxygenAmount;
    UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
    UTextBlock* FoodAmount;
};