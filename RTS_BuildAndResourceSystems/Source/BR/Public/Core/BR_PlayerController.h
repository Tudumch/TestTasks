// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "BR_PlayerController.generated.h"


class ABR_PlayerPawn;
class UInputMappingContext;
class UInputAction;

UCLASS()
class BR_API ABR_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    ABR_PlayerController();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    float ZoomMultiplier = 10.f;
    
    // Input Actions
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
    UInputMappingContext* RTSInputMappingContext = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
    UInputAction* MoveAction = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
    UInputAction* RotateAction = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
    UInputAction* ZoomAction = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
    UInputAction* RotateToggleAction = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
    UInputAction* PrimaryAction = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
    UInputAction* SecondaryAction = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
    UInputAction* ShiftModifierAction = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
    UInputAction* ExitFromGameAction = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float ScreenEdgeThreshold = 100.0f; // in pixels
    
    UPROPERTY()
    ABR_PlayerPawn* ControlledPawn = nullptr;


    bool bIsRotating = false; // Camera rotation flag
    bool bIsShiftButtonPressed = false;

    // Input Handling Functions
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    
    UFUNCTION(BlueprintCallable, Category = Input)
    virtual void SetupInputComponent() override;
    UFUNCTION(BlueprintCallable, Category = Input)
    void OnMove (const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = Input)
    void OnRotate(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = Input)
    void OnZoom(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = Input)
    void OnRotateTogglePressed();
    UFUNCTION(BlueprintCallable, Category = Input)
    void OnRotateToggleReleased();
    UFUNCTION(BlueprintCallable, Category = Input)
    void OnPrimaryActionPressed();
    UFUNCTION(BlueprintCallable, Category = Input)
    void OnSecondaryActionPressed();
    UFUNCTION(BlueprintCallable, Category = Input)
    void OnShiftPressed();
    UFUNCTION(BlueprintCallable, Category = Input)
    void OnShiftReleased();
    UFUNCTION(BlueprintCallable, Category = Input)
    void OnExitFromGamePressed();
    
    UFUNCTION(BlueprintCallable, Category = Input)
    void UpdateCameraPosition(float DeltaTime);
};


