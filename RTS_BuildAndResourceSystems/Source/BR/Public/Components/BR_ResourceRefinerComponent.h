// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Library/ResourcesList.h"
#include "BR_ResourceRefinerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BR_API UBR_ResourceRefinerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBR_ResourceRefinerComponent();

    UFUNCTION(BlueprintCallable)
    TMap<EResourceTypes, int32> ProduceResources(TMap<EResourceTypes, int32> &InputResources);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
    TMap<EResourceTypes, int32> RequiredResources;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
    TMap<EResourceTypes, int32> ProducedResources;
    
	virtual void BeginPlay() override;

};
