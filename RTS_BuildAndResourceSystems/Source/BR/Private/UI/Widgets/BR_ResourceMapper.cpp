// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/BR_ResourceMapper.h"
#include "Components/TextBlock.h"

void UBR_ResourceMapper::UpdateResources(const TMap<EResourceTypes, int32> &ResourceMap) const
{
    EnergyAmount->SetText(FText::AsNumber(ResourceMap[EResourceTypes::Energy]));
    WaterAmount->SetText(FText::AsNumber(ResourceMap[EResourceTypes::Water]));
    OxygenAmount->SetText(FText::AsNumber(ResourceMap[EResourceTypes::Oxygen]));
    FoodAmount->SetText(FText::AsNumber(ResourceMap[EResourceTypes::Food]));
}

void UBR_ResourceMapper::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    
}