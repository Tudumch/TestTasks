// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TTPCharacterMovementComponent.generated.h"

/**
 *
 */
UCLASS()
class TESTTASK_PLEKHANOV_API UTTPCharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:
    virtual float GetMaxSpeed() const override;

private:
    UPROPERTY (EditDefaultsOnly, Category = "Movement", meta = (ClampMin = "0"))
        ;
    float SprintingModifier = 1.5f;
    UPROPERTY (EditDefaultsOnly, Category = "Movement", meta = (ClampMin = "0"))
        ;
    float CrouchingModifier = 0.4f;
};
