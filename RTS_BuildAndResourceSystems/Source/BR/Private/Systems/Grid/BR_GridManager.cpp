// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Grid/BR_GridManager.h"

#include "Actors/Buildings/BR_GridObject.h"
#include "Systems/Grid/BR_GridActor.h"

void UBR_GridManager::RegisterBuildingsInGrid(TSet<ABR_GridObject*> &BuildingsToRegister)
{
    TArray<ABR_GridActor*> FoundGrids;
    TSet<ABR_GridObject*> BuildingsWithoutGrid;

    // Check for buildings with Grid
    for (auto Building : BuildingsToRegister)
        if (Building->GetGrid())
            FoundGrids.Add(Building->GetGrid());
        else
            BuildingsWithoutGrid.Add(Building);

    // Assign Grid
    ABR_GridActor* NewGrid = nullptr;
    
    if (FoundGrids.IsEmpty())
        NewGrid = CreateGrid(BuildingsToRegister);
    else if (FoundGrids.Num() == 1)
    {
        FoundGrids[0]->AddBuildings(BuildingsWithoutGrid);
        NewGrid = FoundGrids[0];
    }
    else
    {
        MergeGrids(FoundGrids[0], FoundGrids[1]);
        NewGrid = FoundGrids[0];
    }

    // If there are still orphan-buildings - add them in the new Grid
    for (auto const Building : BuildingsWithoutGrid)
        Building->SetGrid(NewGrid);
}

void UBR_GridManager::OnBuildingDestroyed(ABR_GridObject* DestroyedBuilding)
{
    ABR_GridActor* Grid = DestroyedBuilding->GetGrid();
    if (!Grid) return;
    
    Grid->RemoveBuilding(DestroyedBuilding);
    
    
    if (Grid->GetBuildings().IsEmpty())
    {
        ActiveGridActors.Remove(Grid);
        DestroyGrid(Grid);
        return;
    }
    
    TSet<ABR_GridObject*> OrphanBuildings = UpdateGrid(Grid);
    AssignGridToOrphanBuildings(OrphanBuildings);
}

void UBR_GridManager::StartProductionCycle()
{
    if (!World) return;

    FTimerHandle ProductionCycleTimerHandle;
    World->GetTimerManager().SetTimer(ProductionCycleTimerHandle, this, &ThisClass::OnProductionCycleTick, 
    ProductionCycleTime, true);
}

ABR_GridActor* UBR_GridManager::CreateGrid(TSet<ABR_GridObject*> &BuildingsForGrid)
{
    if (!World) return nullptr;
    
    ABR_GridActor* GridActor = World->SpawnActor<ABR_GridActor>(ABR_GridActor::StaticClass());
    ActiveGridActors.Add(GridActor);

    GridActor->AddBuildings(BuildingsForGrid);
    
    return GridActor;
}

void UBR_GridManager::DestroyGrid(ABR_GridActor* Grid)
{
    Grid->ClearGrid();
    ActiveGridActors.Remove(Grid);
    Grid->Destroy();
}


TSet<ABR_GridObject*> UBR_GridManager::UpdateGrid(ABR_GridActor* Grid)
{
    if (!Grid) return TSet<ABR_GridObject*>();
    
    TSet<ABR_GridObject*> OrphanBuildings;
    TSet<ABR_GridObject*> ActualBuildingsInGrid = FindAllConnectedBuildings(*Grid->GetBuildings().begin());;

    for (auto Building : Grid->GetBuildings())
        if (!ActualBuildingsInGrid.Contains(Building))
            OrphanBuildings.Add(Building);

    Grid->RemoveBuildings(OrphanBuildings);
    if (Grid->GetBuildings().IsEmpty())
        DestroyGrid(Grid);

    return OrphanBuildings;
}

void UBR_GridManager::MergeGrids(ABR_GridActor* &ReceivingGrid, ABR_GridActor* &GivingGrid)
{
    if (GivingGrid == ReceivingGrid) return;
    
    TSet<ABR_GridObject*> OrphanBuildings = GivingGrid->ClearGrid();
    TMap<EResourceTypes, int32> GivingGridResources = GivingGrid->GetStoredResources();
    
    DestroyGrid(GivingGrid);

    ReceivingGrid->AddBuildings(OrphanBuildings);
    ReceivingGrid->AddResources(GivingGridResources);
}

void UBR_GridManager::AssignGridToOrphanBuildings(TSet<ABR_GridObject*> &OrphanBuildings)
{
    TSet<ABR_GridObject*> BuildingsToRemove;

    while (OrphanBuildings.Num() > 0)
    {
        ABR_GridObject* Building = *OrphanBuildings.begin();

        TSet<ABR_GridObject*> BuildingsForNewGrid = FindAllConnectedBuildings(Building);
            BuildingsForNewGrid.Add(Building);

        CreateGrid(BuildingsForNewGrid);

        BuildingsToRemove.Append(BuildingsForNewGrid);
        OrphanBuildings.Remove(Building);

        for (const auto Item : BuildingsForNewGrid)
            OrphanBuildings.Remove(Item);

        BuildingsToRemove.Reset();
    }
}

TSet<ABR_GridObject*> UBR_GridManager::FindAllConnectedBuildings(ABR_GridObject* &InitialBuilding)
{
    TSet<ABR_GridObject*> ConnectedBuildings;
    GetAllConnectedBuildingsRecursively(InitialBuilding, ConnectedBuildings);
    return ConnectedBuildings;
}

void UBR_GridManager::GetAllConnectedBuildingsRecursively(ABR_GridObject* &Building, 
TSet<ABR_GridObject*>& OutResult)
{
    if (!Building || OutResult.Contains(Building))
        return;
    
    OutResult.Add(Building);

    for (auto LinkedBuilding : Building->GetLinkedBuildings())
        GetAllConnectedBuildingsRecursively(LinkedBuilding, OutResult);
    
    UE_LOG(LogTemp, Display, TEXT("FindAllConnectedBuildingsRecursive activated."));
}

void UBR_GridManager::OnProductionCycleTick()
{
    for (const auto Grid : ActiveGridActors)
        Grid->DoProductionCycle();
}
