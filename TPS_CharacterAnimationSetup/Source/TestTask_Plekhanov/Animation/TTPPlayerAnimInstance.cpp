// Fill out your copyright notice in the Description page of Project Settings.

#include "TestTask_Plekhanov/Animation/TTPPlayerAnimInstance.h"

#include "Components/TTPCharacterMovementComponent.h"
#include "Components/WeaponComponent.h"

void UTTPPlayerAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();
}

void UTTPPlayerAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    PlayerCharacter = Cast<ATTPPlayerCharacter>(TryGetPawnOwner());
}

void UTTPPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!IsValid(PlayerCharacter)) return;

    UpdatePlayerMovingStates();

    UpdatePawnDeltaRotation(DeltaSeconds);

    UpdatePlayerAimViewVariables();

    CurrentPlayerWeaponType = PlayerCharacter->GetCurrentActiveWeaponType();
}

void UTTPPlayerAnimInstance::UpdatePawnDeltaRotation(float DeltaSeconds)
{
    float InterpSpeed = 10.f;
    float InterpDelta =
        (LastFrameRotationValue - PlayerCharacter->GetActorRotation().GetDenormalized()).Yaw /
        DeltaSeconds / InterpSpeed;

    LastFrameRotationValue = PlayerCharacter->GetActorRotation().GetDenormalized();

    PawnRotationDelta = FMath::FInterpTo(PawnRotationDelta, InterpDelta, DeltaSeconds, InterpSpeed);

    // fix for flips at 0-360 degrees:
    if (PawnRotationDelta > 180.f) PawnRotationDelta -= 360.f;
    if (PawnRotationDelta < -180.f) PawnRotationDelta += 360.f;
}

void UTTPPlayerAnimInstance::UpdatePlayerMovingStates()
{
    CurrentPlayerStance = PlayerCharacter->GetCurrentStance();

    CurrentSpeed = PlayerCharacter->GetVelocity().Size();

    ForwardSpeed = FVector::DotProduct(
        PlayerCharacter->GetVelocity(), PlayerCharacter->GetActorForwardVector());
    RightSpeed =
        FVector::DotProduct(PlayerCharacter->GetVelocity(), PlayerCharacter->GetActorRightVector());

    IsMoving = CurrentSpeed > 1.0f;

    IsAccelerating = PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() >
                     MinAcceleratingForAccept;
}

void UTTPPlayerAnimInstance::UpdatePlayerAimViewVariables()
{
    FRotator Delta = PlayerCharacter->GetControlRotation() - PlayerCharacter->GetActorRotation();
    // fix for degree-flips:
    Delta.Pitch > 180 ? Delta.Pitch -= 360 : Delta.Pitch;
    Delta.Yaw > 180 ? Delta.Yaw -= 360 : Delta.Yaw;

    AimPitch = Delta.Pitch;
    AimYaw = Delta.Yaw;
}
