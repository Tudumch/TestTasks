// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BR_GridManager.generated.h"

class ABR_GridObject;

UCLASS()
class BR_API UBR_GridManager : public UObject
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void RegisterBuildingsInGrid(TSet<ABR_GridObject*> &BuildingsToRegister);
    UFUNCTION(BlueprintCallable)
    void OnBuildingDestroyed(ABR_GridObject* DestroyedBuilding);

    UFUNCTION(BlueprintCallable)
    void SetWorld(UWorld* PreferredWorld) {World = PreferredWorld;}

    UFUNCTION(BlueprintCallable)
    void StartProductionCycle();
    
protected:
    // How many seconds pass between resource production.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ProductionCycleTime = 1.f;
    
    UPROPERTY()
    TArray<class ABR_GridActor*> ActiveGridActors;

    UPROPERTY()
    UWorld* World;

    // Returns created Grid.
    UFUNCTION(BlueprintCallable)
    ABR_GridActor* CreateGrid(TSet<ABR_GridObject*> &BuildingsForGrid);
    UFUNCTION(BlueprintCallable)
    void DestroyGrid(ABR_GridActor* Grid);
    
    // Removes all unlinked buildings from the Grid and returns array with orphan buildings.
    UFUNCTION(BlueprintCallable)
    TSet<ABR_GridObject*> UpdateGrid(ABR_GridActor* Grid);
    
    /* Unite two Grids in one.
     * GivingGrid - Grid which transfer its buildings to ReceivingGrid;
     * ReceivingGrid - Grid which get buildings from GivingGrid.
    */
    UFUNCTION(BlueprintCallable)
    void MergeGrids(ABR_GridActor* &GivingGrid, ABR_GridActor* &ReceivingGrid);
    
    UFUNCTION(BlueprintCallable)
    void AssignGridToOrphanBuildings(TSet<ABR_GridObject*> &OrphanBuildings);

    // Finds all buildings connected to InitialBuilding (directly and indirectly). Returns TArray with unique buildings. 
    UFUNCTION(BlueprintCallable)
    TSet<ABR_GridObject*> FindAllConnectedBuildings(ABR_GridObject* &InitialBuilding);

    // Helping recursive-function for FindAllConnectedBuildings-function 
    UFUNCTION(BlueprintCallable)
    void GetAllConnectedBuildingsRecursively(ABR_GridObject* &Building, TSet<ABR_GridObject*> &OutResult);

    UFUNCTION(BlueprintCallable)
    void OnProductionCycleTick();
};

