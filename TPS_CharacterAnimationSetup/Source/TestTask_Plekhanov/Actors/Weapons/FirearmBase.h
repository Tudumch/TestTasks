// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TTPUtils.h"
#include "FirearmBase.generated.h"

UCLASS()
class TESTTASK_PLEKHANOV_API AFirearmBase : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AFirearmBase();

    UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Parameters")
        EWeaponTypes WeaponType = EWeaponTypes::Unarmed;

    virtual bool ZoomFOV(bool ZoomON) { return false; };

protected:
    UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Parameters")
        float BulletSpread = 1.5f;

    // ----------
    // BLUEPRINT COMPONENTS

    UPROPERTY (EditAnywhere)
        FName MuzzleSocketName = TEXT("Muzzle_Socket");
    UPROPERTY (EditAnywhere)
        USceneComponent* SceneComponent;
    UPROPERTY (EditAnywhere)
        USkeletalMeshComponent* SkeletalMesh;

    // ----------
    // FX

    UPROPERTY (EditAnywhere, Category = "FX")
        UParticleSystem* MuzzleFlashParticle;
    UPROPERTY (EditAnywhere, Category = "FX")
        USoundBase* ShotSound;

    virtual void SpawnEffects();

    // Gets player's Pawn and returns Pawn's Controller
    AController* GetOwnerController();

    virtual void BeginPlay() override;
    virtual void GetShotStartEndPoints(
        FHitResult& HitResult, FVector& StartPoint, FVector& EndPoint);

private:
    int32 WeaponRange = 50000;  // LineTrace length

    // ----------
    // FIRERATE
    // ----------
public:
    // Checks firerate Timers and calls the Shoot function
    virtual bool StartFire();
    // Does some operations with firerate Timers after shot
    virtual void StopFire();

protected:
    UPROPERTY (EditDefaultsOnly, Category = "Weapon Parameters")
        float DelayBetweenShots = 0.1f;
    bool bNowFiring = false;  // variable to prevent simultaneous shots. When true, you can't shoot
    FTimerHandle
        DelayBetweenShotsTimerHandle;  // delay between shots when the firing button is pressed
    FTimerHandle
        DelayBetweenMouseClicksTimerHandle;  // delay between repeatedly pressing the shot button

    virtual bool Shoot();

    // Function for use in timers. Calls Shoot function.
    virtual void CallShootFunction();

private:
    // Function for use in timers. Changes the value of the bNowFiring variable to false.
    void Unfire();
};
