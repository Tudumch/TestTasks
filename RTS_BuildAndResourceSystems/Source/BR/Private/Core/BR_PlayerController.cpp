// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BR_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Core/BR_PlayerPawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"
#include "UI/BR_HUD_Gameplay.h"

ABR_PlayerController::ABR_PlayerController()
{
    bShowMouseCursor = true; 
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void ABR_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    ControlledPawn = Cast<ABR_PlayerPawn>(GetPawn());

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(RTSInputMappingContext, 0);
    }
}

void ABR_PlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    UpdateCameraPosition(DeltaSeconds);
}

void ABR_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABR_PlayerController::OnMove);
        EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ABR_PlayerController::OnRotate);
        EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ABR_PlayerController::OnZoom);
        EnhancedInputComponent->BindAction(RotateToggleAction, ETriggerEvent::Started, this, &ABR_PlayerController::OnRotateTogglePressed);
        EnhancedInputComponent->BindAction(RotateToggleAction, ETriggerEvent::Completed, this, &ABR_PlayerController::OnRotateToggleReleased);
        EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Started, this, &ABR_PlayerController::OnPrimaryActionPressed);
        EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Started, this, &ABR_PlayerController::OnSecondaryActionPressed);
        EnhancedInputComponent->BindAction(ShiftModifierAction, ETriggerEvent::Started, this, &ABR_PlayerController::OnShiftPressed);
        EnhancedInputComponent->BindAction(ShiftModifierAction, ETriggerEvent::Completed, this, &ABR_PlayerController::OnShiftReleased);
        EnhancedInputComponent->BindAction(ExitFromGameAction, ETriggerEvent::Started, this, &ABR_PlayerController::OnExitFromGamePressed);
    }
}

void ABR_PlayerController::OnMove(const FInputActionValue& Value)
{
    if (ControlledPawn)
    {
        const FVector2D MovementVector = Value.Get<FVector2D>();
        ControlledPawn->MoveCamera(MovementVector);
    }
}

void ABR_PlayerController::OnRotate(const FInputActionValue& Value)
{
    if (ControlledPawn && bIsRotating)
    {
        const FVector2D RotationVector = Value.Get<FVector2D>();
        ControlledPawn->RotateCamera(RotationVector);
    }
}

void ABR_PlayerController::OnZoom(const FInputActionValue& Value)
{
    if (ControlledPawn)
    {
        const float ZoomValue = Value.Get<float>() * ZoomMultiplier;
        ControlledPawn->ZoomCamera(ZoomValue);
    }
}

void ABR_PlayerController::OnRotateTogglePressed()
{
    bIsRotating = true;
}

void ABR_PlayerController::OnRotateToggleReleased()
{
    bIsRotating = false;
}

void ABR_PlayerController::OnPrimaryActionPressed()
{
    if (ABR_HUD_Gameplay *HUD = Cast<ABR_HUD_Gameplay>(GetHUD()))
        HUD->StartPrimaryAction(bIsShiftButtonPressed);
}

void ABR_PlayerController::OnSecondaryActionPressed()
{
    if (ABR_HUD_Gameplay *HUD = Cast<ABR_HUD_Gameplay>(GetHUD()))
        HUD->StartSecondaryAction();
}

void ABR_PlayerController::OnShiftPressed()
{
    bIsShiftButtonPressed = true;
}

void ABR_PlayerController::OnShiftReleased()
{
    bIsShiftButtonPressed = false;
}

void ABR_PlayerController::OnExitFromGamePressed()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, true);
}

void ABR_PlayerController::UpdateCameraPosition(float DeltaTime)
{
    float MousePositionX;
    float MousePositionY;
    GetMousePosition(MousePositionX, MousePositionY);
    const FVector2D CurrentMousePosition = FVector2D(MousePositionX, MousePositionY);
    
    FVector2D ViewportSize;
    if (GEngine && GEngine->GameViewport)
        GEngine->GameViewport->GetViewportSize(ViewportSize);
    
    FVector2D CameraMovement = FVector2D();

    if (CurrentMousePosition.X < ScreenEdgeThreshold) // Left border
        CameraMovement.X = -1;
    else if (CurrentMousePosition.X > ViewportSize.X - ScreenEdgeThreshold) // Right border
        CameraMovement.X = 1;

    if (CurrentMousePosition.Y < ScreenEdgeThreshold) // Top border
        CameraMovement.Y = 1;
    else if (CurrentMousePosition.Y > ViewportSize.Y - ScreenEdgeThreshold) // Bottom border
        CameraMovement.Y = -1;
    
    // Apply movement to Pawn
    if (ControlledPawn)
        ControlledPawn->MoveCamera(CameraMovement);
}