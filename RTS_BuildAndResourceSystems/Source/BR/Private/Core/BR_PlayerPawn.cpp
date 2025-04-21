// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BR_PlayerPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnitConversion.h"

// Sets default values
ABR_PlayerPawn::ABR_PlayerPawn()
{
	PrimaryActorTick.bCanEverTick = false;

    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    RootComponent = RootSceneComponent;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootSceneComponent);
    SpringArmComponent->TargetArmLength = CameraHeight; 
    SpringArmComponent->bDoCollisionTest = false; 
    SpringArmComponent->SetRelativeRotation(CameraAngle); 

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);
}

void ABR_PlayerPawn::MoveCamera(const FVector2D& MovementVector)
{
    const FVector ForwardDirection = FVector::VectorPlaneProject(GetActorForwardVector(), FVector::UpVector).GetSafeNormal();
    const FVector RightDirection = FVector::VectorPlaneProject(GetActorRightVector(), FVector::UpVector).GetSafeNormal();

    const FVector Movement = (ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X) * MoveSpeed * GetWorld()->GetDeltaSeconds();
    AddActorWorldOffset(Movement);
}

void ABR_PlayerPawn::RotateCamera(const FVector2D& RotationVector)
{
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += RotationVector.X * RotationSpeed;
    SetActorRotation(NewRotation);
}

void ABR_PlayerPawn::ZoomCamera(float ZoomValue)
{
    float NewArmLength = SpringArmComponent->TargetArmLength - ZoomValue * ZoomSpeed * GetWorld()->GetDeltaSeconds();
    SpringArmComponent->TargetArmLength = FMath::Clamp(NewArmLength, MinZoom, MaxZoom);
}

void ABR_PlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

