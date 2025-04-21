#pragma once

#include "TTPUtils.generated.h"

UENUM(BlueprintType)
enum class EPlayerStances : uint8
{
    Crouching,
    Walking,
    Sprinting
};

UENUM(BlueprintType)
enum class EWeaponTypes : uint8
{
    Unarmed,
    Rifle
};
