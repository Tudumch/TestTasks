// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "Characters/TTPPlayerCharacter.h"
#include "TTPUtils.h"

#include "TTPPlayerAnimInstance.generated.h"

UCLASS()
class TESTTASK_PLEKHANOV_API UTTPPlayerAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeBeginPlay() override;

    virtual void NativeInitializeAnimation() override;

    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    UPROPERTY (BlueprintReadOnly, Category = "BaseAnimInstance")
        ATTPPlayerCharacter* PlayerCharacter = nullptr;

    // Moving speed at this tick.
    UPROPERTY (BlueprintReadWrite, Category = "BaseAnimInstance|Speed")
        float CurrentSpeed = 0.0f;

    UPROPERTY (BlueprintReadWrite, Category = "BaseAnimInstance|Speed")
        float ForwardSpeed = 0.0f;

    UPROPERTY (BlueprintReadWrite, Category = "BaseAnimInstance|Speed")
        float RightSpeed = 0.0f;

    // Last speed when was falling(in air).
    UPROPERTY (BlueprintReadWrite, Category = "BaseAnimInstance|Speed")
        float SpeedBeforeLanding = 0.0f;

    // Current moving direction. In degrees -180 to 180.
    UPROPERTY (BlueprintReadWrite, Category = "BaseAnimInstance|Speed")
        float Direction = 0.0f;

    // This flag shows that person is in moving, it's speed > 0.
    UPROPERTY (BlueprintReadWrite, Category = "BaseAnimInstance")
        bool IsMoving = false;

    // This flag shows that person speed is increasing. Can be used for move-key press detecting.
    UPROPERTY (BlueprintReadWrite, Category = "BaseAnimInstance")
        bool IsAccelerating = false;

    // Min scale of acceleration after which IsAccelerating will be true.
    UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "BaseAnimInstance")
        float MinAcceleratingForAccept = 1.0f;

    // This flag shows is person aiming or not
    UPROPERTY (BlueprintReadWrite, Category = "BaseAnimInstance")
        bool IsAiming = false;

    // Calculates pawn rotation between two ticks
    UFUNCTION (BlueprintCallable, Category = "Calculations")
        void UpdatePawnDeltaRotation(float DeltaSeconds);

    // Stores value of pawn rotation between two ticks
    UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "BaseAnimInstance")
        float PawnRotationDelta = 0.f;

    // Stores current player stance: walking, crouching etc..
    UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "BaseAnimInstance")
        EPlayerStances CurrentPlayerStance;

    UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "BaseAnimInstance")
        EWeaponTypes CurrentPlayerWeaponType;

    // Updates some moving-variables
    void UpdatePlayerMovingStates();

    // Stores current player view in Pitch-axis
    UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "BaseAnimInstance")
        float AimPitch;
    // Stores current player view in Yaw-axis
    UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "BaseAnimInstance")
        float AimYaw;

    // Calculates current player view in Pitch and Yaw axes
    void UpdatePlayerAimViewVariables();

private:
    // Variable for calculation player rotation between two ticks
    FRotator LastFrameRotationValue = FRotator(0.f, 0.f, 0.f);
};
