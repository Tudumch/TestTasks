// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Weapons/TTPRifle.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Characters/TTPPlayerCharacter.h"
#include "Components/FXComponent.h"

#define OUT

ATTPRifle::ATTPRifle()
{
    FXComponent = CreateDefaultSubobject<UFXComponent>(TEXT("FXComponent"));
}

bool ATTPRifle::Shoot()
{
    if (!Super::Shoot()) return false;

    FVector TraceStart;
    FVector TraceEnd;
    GetShotStartEndPoints(OUT RifleHitResult, OUT TraceStart, OUT TraceEnd);

    SpawnTraceFX(RifleHitResult.Location);

    return true;
}

void ATTPRifle::SpawnTraceFX(const FVector& EndPoint)
{
    FVector StartPoint = SkeletalMesh->GetSocketLocation(MuzzleSocketName);

    const auto TraceFXComponent =
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, StartPoint);

    if (TraceFXComponent) TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, EndPoint);

    FXComponent->PlayImpactFX(RifleHitResult);
}

bool ATTPRifle::ZoomFOV(bool ZoomON)
{
    return false;
}
