// Fill out your copyright notice in the Description page of Project Settings.

#include "TestTask_Plekhanov/Characters/TTPPlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Components/TTPCharacterMovementComponent.h"
#include "Components/WeaponComponent.h"

// Sets default values
ATTPPlayerCharacter::ATTPPlayerCharacter(const class FObjectInitializer& ObjectInitializer)
    // overriding CharacterMovementComponent:
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UTTPCharacterMovementComponent>(
          ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
}

void ATTPPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    PlayerController = GetWorld()->GetFirstPlayerController();
    ConnectMappingContext();
}

void ATTPPlayerCharacter::ConnectMappingContext()
{
    if (PlayerController)
    {
        if (UEnhancedInputLocalPlayerSubsystem* InputSystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                    PlayerController->GetLocalPlayer()))
        {
            InputSystem->AddMappingContext(InputMappingContext, 0);
        }
    }
}

void ATTPPlayerCharacter::ToggleStance(EPlayerStances Stance)
{
    if (Stance == EPlayerStances::Crouching)
    {
        CurrentPlayerStance == EPlayerStances::Crouching
            ? CurrentPlayerStance = EPlayerStances::Walking
            : CurrentPlayerStance = EPlayerStances::Crouching;
        return;
    }
    if (Stance == EPlayerStances::Sprinting && GetVelocity().Size() <= 100.f)
    {
        return;
    }

    CurrentPlayerStance = Stance;
}

// Called every frame
void ATTPPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATTPPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent =
            CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(
            MoveIA, ETriggerEvent::Triggered, this, &ThisClass::Move);
        EnhancedInputComponent->BindAction(
            LookIA, ETriggerEvent::Triggered, this, &ThisClass::Look);

        // ----------
        // STANCE TOGGLING:

        DECLARE_DELEGATE_OneParam(FToggleStanceInputParams,
            EPlayerStances);  // to pass an attribute to a function by reference below

        EnhancedInputComponent->BindAction(CrouchIA, ETriggerEvent::Triggered, this,
            &ThisClass::ToggleStance, EPlayerStances::Crouching);
        EnhancedInputComponent->BindAction(SprintIA, ETriggerEvent::Started, this,
            &ThisClass::ToggleStance, EPlayerStances::Sprinting);
        EnhancedInputComponent->BindAction(SprintIA, ETriggerEvent::Completed, this,
            &ThisClass::ToggleStance, EPlayerStances::Walking);

        // ---------
        // WEAPONS:

        EnhancedInputComponent->BindAction(
            FireIA, ETriggerEvent::Started, WeaponComponent, &UWeaponComponent::StartFire);
        EnhancedInputComponent->BindAction(
            FireIA, ETriggerEvent::Completed, WeaponComponent, &UWeaponComponent::StopFire);

        // ----------
        // SWITCH WEAPON
        EnhancedInputComponent->BindAction(SwitchWeaponToNextIA, ETriggerEvent::Triggered,
            WeaponComponent, &UWeaponComponent::SwitchWeaponToNext);

        DECLARE_DELEGATE_OneParam(FSwitchWeaponInputParams,
            int32);  // to pass an attribute to a function by reference below
        EnhancedInputComponent->BindAction(SwitchWeaponToIndex00IA, ETriggerEvent::Triggered,
            WeaponComponent, &UWeaponComponent::SwitchWeaponToIndex, 0);
        EnhancedInputComponent->BindAction(SwitchWeaponToIndex01IA, ETriggerEvent::Triggered,
            WeaponComponent, &UWeaponComponent::SwitchWeaponToIndex, 1);
        EnhancedInputComponent->BindAction(SwitchWeaponToIndex02IA, ETriggerEvent::Triggered,
            WeaponComponent, &UWeaponComponent::SwitchWeaponToIndex, 2);

        // ----------
        // ZOOM:
        DECLARE_DELEGATE_OneParam(
            FZoomInputParams, bool);  // to pass an attribute to a function by reference below
        EnhancedInputComponent->BindAction(ToggleZoomIA, ETriggerEvent::Started, WeaponComponent,
            &UWeaponComponent::ToggleZoom, true);
        EnhancedInputComponent->BindAction(ToggleZoomIA, ETriggerEvent::Completed, WeaponComponent,
            &UWeaponComponent::ToggleZoom, false);
    }
}

void ATTPPlayerCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(ForwardDirection, MovementVector.X);

    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(RightDirection, MovementVector.Y);
}

void ATTPPlayerCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisValue = Value.Get<FVector2D>();
    if (GetController())
    {
        AddControllerYawInput(LookAxisValue.X);
        AddControllerPitchInput(LookAxisValue.Y);
    }
}

const EPlayerStances ATTPPlayerCharacter::GetCurrentStance()
{
    return CurrentPlayerStance;
}

const EWeaponTypes ATTPPlayerCharacter::GetCurrentActiveWeaponType()
{
    return WeaponComponent->GetActiveWeaponType();
}
