// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BR_PlayerPawn.generated.h"

UCLASS()
class BR_API ABR_PlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABR_PlayerPawn();

    UFUNCTION() FORCENOINLINE
    void MoveCamera(const FVector2D& MovementVector);
    void RotateCamera(const FVector2D& RotationVector);
    void ZoomCamera(float ZoomValue);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* RootSceneComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UCameraComponent* CameraComponent;

    UPROPERTY(EditAnywhere, Category = "Camera Settings")
    float MoveSpeed = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "Camera Settings")
    float RotationSpeed = 2.0f;

    UPROPERTY(EditAnywhere, Category = "Camera Settings")
    float ZoomSpeed = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "Camera Settings")
    float MinZoom = 300.0f;

    UPROPERTY(EditAnywhere, Category = "Camera Settings")
    float MaxZoom = 2000.0f;

    // Initial values
    float CameraHeight = 1000.f; 
    FRotator CameraAngle = FRotator(-45.0f, 0.0f, 0.0f);
};