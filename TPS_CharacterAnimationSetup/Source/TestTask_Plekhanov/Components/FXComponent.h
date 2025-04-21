// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FXComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TESTTASK_PLEKHANOV_API UFXComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UFXComponent();

    // Uses for projectiles.
    void PlayImpactFX();
    // Uses for hitscans.
    void PlayImpactFX(const FHitResult& Hit);

protected:
    UPROPERTY (EditAnywhere, Category = "VFX")
        UParticleSystem* ImpactParticle;

    UPROPERTY (EditAnywhere, Category = "VFX")
        USoundBase* ImpactSound;
};
