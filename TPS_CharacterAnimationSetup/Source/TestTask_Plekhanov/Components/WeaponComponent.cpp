// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/WeaponComponent.h"

#include "Actors/Weapons/FirearmBase.h"
#include "Actors/Weapons/TTPRifle.h"
#include "Characters/TTPPlayerCharacter.h"
#include "TTPUtils.h"

DEFINE_LOG_CATEGORY_STATIC(Log_WeaponComponent, All, All);

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    SetupWeapon();

    // SetupAllAnimNotifies();
}

void UWeaponComponent::SetupWeapon()
{
    ATTPPlayerCharacter* Player = Cast<ATTPPlayerCharacter>(GetOwner());
    if (!Player) return;

    // If Player does not have any initial weapons in array - log error:
    if (WeaponClasses.Num() == 0)
    {
        UE_LOG(Log_WeaponComponent, Error, TEXT("SetupWeapon: %s: default weapons has not set!"),
            *Player->GetName());
        return;
    }

    // Spawning and adding to Player's inventory default weapons:
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    for (auto WeaponClass : WeaponClasses)
    {
        if (!WeaponClass) continue;

        AFirearmBase* Weapon = GetWorld()->SpawnActor<AFirearmBase>(WeaponClass);
        AddWeaponToPlayer(Weapon);
    }

    SwitchWeaponToIndex(0);
}

void UWeaponComponent::PlayAnimMontage(UAnimMontage* AnimMontage)
{

    ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());
    if (!PlayerCharacter) return;

    PlayerCharacter->PlayAnimMontage(AnimMontage);
}

void UWeaponComponent::StartFire()
{
    ATTPPlayerCharacter* Player = Cast<ATTPPlayerCharacter>(GetOwner());

    if (!Player || Player->GetCurrentStance() == EPlayerStances::Sprinting)
        return;  // Player can't sprint and shoot at the same time

    if (ATTPRifle* WeaponCasted = Cast<ATTPRifle>(ActiveWeapon)) WeaponCasted->StartFire();
}

void UWeaponComponent::StopFire()
{
    ATTPPlayerCharacter* Player = Cast<ATTPPlayerCharacter>(GetOwner());

    if (!Player) return;

    if (ATTPRifle* WeaponCasted = Cast<ATTPRifle>(ActiveWeapon)) WeaponCasted->StopFire();
}

void UWeaponComponent::SwitchWeaponToIndex(int32 Index)
{
    if (ActiveWeaponIdx == Index || Index >= CharacterWeapons.Num() || Index < 0)
    {
        return;
    }

    ATTPPlayerCharacter* Player = Cast<ATTPPlayerCharacter>(GetOwner());
    if (!Player) return;

    // putting the active weapon in the inventory:
    if (ActiveWeapon)
        ActiveWeapon->AttachToComponent(Player->GetMesh(),
            FAttachmentTransformRules::KeepRelativeTransform, ArmoryWeaponSocketName);

    ActiveWeapon = CharacterWeapons[Index];
    ActiveWeapon->AttachToComponent(Player->GetMesh(),
        FAttachmentTransformRules::KeepRelativeTransform, ActiveWeaponSocketName);
    ActiveWeaponIdx = Index;

    if (WeaponEquipAnimation)
    {
        PlayAnimMontage(WeaponEquipAnimation);
    }
}

void UWeaponComponent::SwitchWeaponToNext()
{
    if (CharacterWeapons.Num() == 0)
    {
        UE_LOG(Log_WeaponComponent, Error,
            TEXT("SwitchWeaponToNext: Character default weapons list is empty!"));
        return;
    }

    int32 NewActiveWeaponIdx = fabs((ActiveWeaponIdx + 1) % CharacterWeapons.Num());
    SwitchWeaponToIndex(NewActiveWeaponIdx);
}

void UWeaponComponent::SwitchWeaponToPrevious()
{
    if (CharacterWeapons.Num() == 0)
    {
        UE_LOG(Log_WeaponComponent, Error,
            TEXT("SwitchWeaponToPrevious: Character default weapons list is empty!"));
        return;
    }

    int32 NewActiveWeaponIdx = fabs((ActiveWeaponIdx - 1) % CharacterWeapons.Num());
    SwitchWeaponToIndex(NewActiveWeaponIdx);
}

void UWeaponComponent::ToggleZoom(bool ZoomON)
{
    // ActiveWeapon->ZoomFOV(ZoomON);
}

void UWeaponComponent::AddWeaponToPlayer(AFirearmBase* Weapon)
{
    ATTPPlayerCharacter* Player = Cast<ATTPPlayerCharacter>(GetOwner());
    if (!Player) return;

    Weapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
        ArmoryWeaponSocketName);

    Weapon->SetOwner(Player);
    CharacterWeapons.Add(Weapon);
}

EWeaponTypes UWeaponComponent::GetActiveWeaponType()
{
    if (!ActiveWeapon) return EWeaponTypes::Unarmed;

    return ActiveWeapon->WeaponType;
}
