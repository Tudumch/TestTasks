// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TTPUtils.h"
#include "TTPPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UWeaponComponent;

UCLASS()
class TESTTASK_PLEKHANOV_API ATTPPlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ATTPPlayerCharacter(const class FObjectInitializer& ObjectInitializer);

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    const EPlayerStances GetCurrentStance();

    UFUNCTION (BlueprintPure)
        const EWeaponTypes GetCurrentActiveWeaponType();

    UPROPERTY (EditAnywhere)
        UWeaponComponent* WeaponComponent;

protected:
    virtual void BeginPlay() override;

    UPROPERTY (EditAnywhere, Category = "Input")
        EPlayerStances CurrentPlayerStance = EPlayerStances::Walking;

    // Connects EnhancedInputs's MappingContext to Player Controller
    virtual void ConnectMappingContext();

    // -----------------
    // INPUT ACTIONS

    UPROPERTY (EditAnywhere, Category = "Input")
        UInputMappingContext* InputMappingContext;

    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* MoveIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* CrouchIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* SprintIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* LookIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* FireIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* AimIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* ToggleZoomIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* SwitchWeaponToNextIA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* SwitchWeaponToIndex00IA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* SwitchWeaponToIndex01IA;
    UPROPERTY (EditAnywhere, Category = "Input")
        UInputAction* SwitchWeaponToIndex02IA;

private:
    APlayerController* PlayerController;

    // Changes boolean variables that switch stances
    void ToggleStance(EPlayerStances Stance);
};
