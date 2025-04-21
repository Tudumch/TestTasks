// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SelectableObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USelectableObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BR_API ISelectableObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual void Select(const bool IsSelected);
};
