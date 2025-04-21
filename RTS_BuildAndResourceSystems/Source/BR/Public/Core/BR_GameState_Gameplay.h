// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BR_GameState_Gameplay.generated.h"

class UBR_GridManager;
/**
 * 
 */
UCLASS()
class BR_API ABR_GameState_Gameplay : public AGameStateBase
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure)
    UBR_GridManager* GetGridManager() const {return GridManager;}

protected:
    UPROPERTY(EditAnywhere)
    UBR_GridManager* GridManager;
    
    virtual void BeginPlay() override;
};
