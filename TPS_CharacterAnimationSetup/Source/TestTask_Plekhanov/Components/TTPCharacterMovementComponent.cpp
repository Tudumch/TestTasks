// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TTPCharacterMovementComponent.h"
#include "TTPUtils.h"
#include "Characters/TTPPlayerCharacter.h"

float UTTPCharacterMovementComponent::GetMaxSpeed() const
{
    float MaxSpeed = Super::GetMaxSpeed();
    ATTPPlayerCharacter* Player = Cast<ATTPPlayerCharacter>(GetPawnOwner());

    if (!Player) return MaxSpeed;

    switch (Player->GetCurrentStance())
    {
        case EPlayerStances::Crouching: return MaxSpeed * CrouchingModifier; break;
        case EPlayerStances::Walking: return MaxSpeed; break;
        case EPlayerStances::Sprinting: return MaxSpeed * SprintingModifier; break;
        default: break;
    };

    return MaxSpeed;
}
