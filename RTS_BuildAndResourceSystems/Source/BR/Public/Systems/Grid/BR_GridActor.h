// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Library/ResourcesList.h"
#include "BR_GridActor.generated.h"

class UBR_ResourceMapper;
class UWidgetComponent;
class ABR_GridObject;

/*
Grid-actor - actor who keeps track of which buildings are connected to each other.
*/
UCLASS()
class BR_API ABR_GridActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABR_GridActor();
    
    UFUNCTION(BlueprintCallable)
    void AddBuilding(ABR_GridObject* &Building);
    UFUNCTION(BlueprintCallable)
    void AddBuildings(TSet<ABR_GridObject*> &Buildings);
    UFUNCTION(BlueprintPure)
    TSet<ABR_GridObject*> GetBuildings() const {return BuildingsInGrid;};
    
    UFUNCTION(BlueprintCallable)
    void AddResources(TMap<EResourceTypes, int32> &Resources);
    UFUNCTION(BlueprintPure)
    TMap<EResourceTypes, int32> GetStoredResources() const {return StoredResources;}
    
    // Removes building from this Grid and clear it's Grid-property
    UFUNCTION(BlueprintCallable)
    void RemoveBuilding(ABR_GridObject* &Building);
    
    // Removes buildings from this Grid and clear their Grid-property
    UFUNCTION(BlueprintCallable)
    void RemoveBuildings(TSet<ABR_GridObject*> &Buildings);
    
    // Removes all buildings from this Grid and clear theirs Grid-property. Return array with all buildings removed from this Grid.
    UFUNCTION(BlueprintCallable)
    TSet<ABR_GridObject*> ClearGrid();

    UFUNCTION(BlueprintCallable)
    void DoProductionCycle();

protected:
    UPROPERTY(EditAnywhere)
    TSet<ABR_GridObject*> BuildingsInGrid;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EResourceTypes, int32> StoredResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UWidgetComponent* ResourcesWidgetComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBR_ResourceMapper* ResourcesWidget;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResourcesWidgetHeight = 400.f;
    
	virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Centers the position of the Grid between its child buildings.
    UFUNCTION(Blueprintable)
    void UpdatePosition();
    
    UFUNCTION(BlueprintCallable)
    void SetResourcesWidget();

    UFUNCTION(BlueprintCallable)
    void RotateWidgetTowardPlayerCamera();
};


