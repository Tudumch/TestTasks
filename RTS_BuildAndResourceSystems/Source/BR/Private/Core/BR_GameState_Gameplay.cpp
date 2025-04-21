// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BR_GameState_Gameplay.h"

#include "Systems/Grid/BR_GridManager.h"

void ABR_GameState_Gameplay::BeginPlay()
{
    Super::BeginPlay();

    GridManager = NewObject<UBR_GridManager>();
    GridManager->SetWorld(GetWorld());
    GridManager->StartProductionCycle();
}