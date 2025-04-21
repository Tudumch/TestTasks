// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class AFirearmBase;

/*
Class for interaction between the player and his weapon.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TESTTASK_PLEKHANOV_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

private:
    // Index of the current weapon in hands
    int32 ActiveWeaponIdx = -1;  // initial value is -1 to switch weapons correctly
                                 // when the player spawns (see SwitchWeaponToIndex logic)

public:
    UWeaponComponent();

    // Weapons in inventory
    TArray<AFirearmBase*> CharacterWeapons;
    // Weapon in hands
    AFirearmBase* ActiveWeapon;

    void StartFire();
    void StopFire();
    void SwitchWeaponToIndex(int32 Index);
    void SwitchWeaponToNext();
    void SwitchWeaponToPrevious();
    void ToggleZoom(bool ZoomON);

    // Adds Weapon to Player's inventory.
    void AddWeaponToPlayer(AFirearmBase* Weapon);

    UFUNCTION (BlueprintPure)
        EWeaponTypes GetActiveWeaponType();

protected:
    // Weapons that Player has at respawn
    UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        TArray<TSubclassOf<AFirearmBase>> WeaponClasses;

    virtual void BeginPlay() override;

private:
    UPROPERTY (EditDefaultsOnly, Category = "Weapon")
        FName ActiveWeaponSocketName = TEXT("WeaponSocket_r");
    UPROPERTY (EditDefaultsOnly, Category = "Weapon")
        FName ArmoryWeaponSocketName = TEXT("ArmorySocket");

    UPROPERTY (EditDefaultsOnly, Category = "Weapon")
        UAnimMontage* WeaponEquipAnimation;

    // Setups weapons in inventory after Player's respawn.
    void SetupWeapon();

    void PlayAnimMontage(UAnimMontage* AnimMontage);
};
