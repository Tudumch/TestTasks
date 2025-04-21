// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildings/BR_GridObject.h"

#include "Components/BR_ResourceRefinerComponent.h"
#include "Core/BR_GameState_Gameplay.h"
#include "Systems/Grid/BR_GridManager.h"

// Sets default values
ABR_GridObject::ABR_GridObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    RefinerComponent = CreateDefaultSubobject<UBR_ResourceRefinerComponent>(TEXT("ResourceRefiner"));
}

void ABR_GridObject::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (EndPlayReason == EEndPlayReason::Destroyed)
        RegisterBuildingsDisconnection();
    
    Super::EndPlay(EndPlayReason);
}

void ABR_GridObject::Select(const bool IsSelected)
{
}

// Called when the game starts or when spawned
void ABR_GridObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABR_GridObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABR_GridObject::RegisterBuildingsConnection()
{
    for (auto Building : LinkedBuildings)
        Building->AddLinkedBuilding(this);
    
    const ABR_GameState_Gameplay* GameState = Cast<ABR_GameState_Gameplay>(GetWorld()->GetGameState());
    if (!GameState) return;
    
    UBR_GridManager* GridManager = GameState->GetGridManager();
    if (!GridManager) return;

    TSet<ABR_GridObject*> BuildingsToRegistration;
    BuildingsToRegistration.Append(LinkedBuildings);
    BuildingsToRegistration.Add(this);

    GridManager->RegisterBuildingsInGrid(BuildingsToRegistration);
}

void ABR_GridObject::RegisterBuildingsDisconnection()
{
    for (auto Building : LinkedBuildings)
        Building->RemoveLinkedBuilding(this);

    LinkedBuildings.Empty();

    const ABR_GameState_Gameplay* GameState = Cast<ABR_GameState_Gameplay>(GetWorld()->GetGameState());
    if (!GameState) return;
    
    UBR_GridManager* GridManager = GameState->GetGridManager();
    if (!GridManager) return;

    GridManager->OnBuildingDestroyed(this);
}
