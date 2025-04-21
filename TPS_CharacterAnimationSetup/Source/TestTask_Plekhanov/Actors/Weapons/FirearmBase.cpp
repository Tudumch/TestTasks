// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Weapons/FirearmBase.h"

#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/TTPPlayerAnimInstance.h"

// Sets default values
AFirearmBase::AFirearmBase()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = SceneComponent;

    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    SkeletalMesh->SetupAttachment(SceneComponent);
}

void AFirearmBase::BeginPlay()
{
    Super::BeginPlay();
}

AController* AFirearmBase::GetOwnerController()
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn) return nullptr;

    return OwnerPawn->Controller;
}

void AFirearmBase::GetShotStartEndPoints(
    FHitResult& HitResult, FVector& StartPoint, FVector& EndPoint)
{
    FVector ViewLocation;
    FRotator ViewRotation;

    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn) return;

    float YawDelta = (OwnerPawn->GetControlRotation() - OwnerPawn->GetActorRotation()).Yaw;
    YawDelta > 180 ? YawDelta -= 360 : YawDelta;

    if (OwnerPawn->GetVelocity().Size() <= 50.f && -60.f <= YawDelta && YawDelta <= 60.f)
    {
        GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
        StartPoint = ViewLocation;
    }
    else
    {
        ViewRotation = SkeletalMesh->GetSocketRotation(MuzzleSocketName);
        StartPoint = SkeletalMesh->GetSocketLocation(MuzzleSocketName);
    }

    const auto ConeRadius =
        FMath::DegreesToRadians(BulletSpread);  // spread at the end of LineTrace

    FVector TraceDirection = ViewRotation.Vector();
    TraceDirection = FMath::VRandCone(TraceDirection, ConeRadius);
    StartPoint = StartPoint + TraceDirection * 100;  // offset for avoid player collision
    EndPoint = StartPoint + TraceDirection * WeaponRange;

    FCollisionQueryParams TraceParams(TEXT(""), false, GetOwner());  // ignore owner collision
    GetWorld()->LineTraceSingleByChannel(
        OUT HitResult, StartPoint, EndPoint, ECollisionChannel::ECC_Visibility, TraceParams);
}

bool AFirearmBase::Shoot()
{
    SpawnEffects();
    return true;
}

bool AFirearmBase::StartFire()
{
    if (bNowFiring) return false;
    bNowFiring = true;

    Shoot();

    GetWorldTimerManager().SetTimer(
        DelayBetweenShotsTimerHandle, this, &ThisClass::CallShootFunction, DelayBetweenShots, true);

    return true;
}

void AFirearmBase::StopFire()
{
    GetWorldTimerManager().ClearTimer(DelayBetweenShotsTimerHandle);

    if (!GetWorldTimerManager().TimerExists(DelayBetweenMouseClicksTimerHandle))
    {
        GetWorldTimerManager().SetTimer(DelayBetweenMouseClicksTimerHandle, this,
            &AFirearmBase::Unfire, DelayBetweenShots, false);
    }
}

void AFirearmBase::Unfire()
{
    bNowFiring = false;
}

void AFirearmBase::CallShootFunction()
{
    Shoot();
}

void AFirearmBase::SpawnEffects()
{
    if (MuzzleFlashParticle && ShotSound)
    {
        UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticle, SkeletalMesh, MuzzleSocketName);
        UGameplayStatics::SpawnSoundAttached(ShotSound, SkeletalMesh, MuzzleSocketName);
    }
}
