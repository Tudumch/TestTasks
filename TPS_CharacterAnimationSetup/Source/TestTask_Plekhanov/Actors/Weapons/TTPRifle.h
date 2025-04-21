// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapons/FirearmBase.h"
#include "TTPRifle.generated.h"

class UFXComponent;
class UNiagaraSystem;

UCLASS()
class TESTTASK_PLEKHANOV_API ATTPRifle : public AFirearmBase
{
    GENERATED_BODY()

public:
    ATTPRifle();

    virtual bool ZoomFOV(bool ZoomON) override;

protected:
    UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "VFX")
        UNiagaraSystem* TraceFX;

    UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "VFX")
        FString TraceTargetName = "TraceTarget";

    FHitResult RifleHitResult;

    virtual bool Shoot() override;

private:
    UPROPERTY (EditAnywhere)
        UFXComponent* FXComponent;

    void SpawnTraceFX(const FVector& EndPoint);
};
