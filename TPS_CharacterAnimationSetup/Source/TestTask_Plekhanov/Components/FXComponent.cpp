// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/FXComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFXComponent::UFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFXComponent::PlayImpactFX()
{
    UGameplayStatics::SpawnEmitterAtLocation(this, ImpactParticle, GetOwner()->GetActorLocation());
    UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetOwner()->GetActorLocation());
}

void UFXComponent::PlayImpactFX(const FHitResult& Hit)
{
    UGameplayStatics::SpawnEmitterAtLocation(this, ImpactParticle, Hit.Location);
    UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, Hit.Location);
}
