// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BR_ResourceRefinerComponent.h"

UBR_ResourceRefinerComponent::UBR_ResourceRefinerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

TMap<EResourceTypes, int32> UBR_ResourceRefinerComponent::ProduceResources(TMap<EResourceTypes, int32> 
&InputResources)
{
    // Check is InputResources amount enough for production
    for (const auto RequiredResource : RequiredResources)
    {
        if (!InputResources.Find(RequiredResource.Key))
            return InputResources;
        if (InputResources[RequiredResource.Key] < RequiredResource.Value)
            return InputResources;
    }

    // Take required amount of resources from InputResources
    for (const auto RequiredResource : RequiredResources)
        InputResources[RequiredResource.Key] -= RequiredResource.Value;

    // Add produced resources to InputResources
    for (const auto ProducedResource : ProducedResources)
    {
        if (InputResources.Find(ProducedResource.Key))
            InputResources[ProducedResource.Key] += ProducedResource.Value;
        else
            InputResources.Add(ProducedResource.Key, ProducedResource.Value);
    }
    
    return InputResources;
}


void UBR_ResourceRefinerComponent::BeginPlay()
{
	Super::BeginPlay();
}