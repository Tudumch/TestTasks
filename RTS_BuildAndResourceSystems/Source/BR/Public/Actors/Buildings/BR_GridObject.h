// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SelectableObject.h"
#include "BR_GridObject.generated.h"

class UBR_ResourceRefinerComponent;
class ABR_GridActor;

UCLASS()
class BR_API ABR_GridObject : public AActor, public ISelectableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABR_GridObject();

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION()
    virtual void Select(const bool IsSelected) override;

    UFUNCTION(BlueprintCallable)
    virtual void SetGrid(ABR_GridActor* GridActor) { Grid = GridActor; }
    UFUNCTION(BlueprintCallable)
    virtual ABR_GridActor* GetGrid() { return Grid; }
    
    UFUNCTION()
    virtual void AddLinkedBuilding(ABR_GridObject* Building) {LinkedBuildings.Add (Building);}
    UFUNCTION()
    virtual void RemoveLinkedBuilding(ABR_GridObject* Building) {LinkedBuildings.Remove (Building);}
    UFUNCTION()
    virtual TArray<ABR_GridObject*> GetLinkedBuildings() {return LinkedBuildings;}

protected:
    // Which Grid-actor is this building included in
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ABR_GridActor* Grid = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ABR_GridObject*> LinkedBuildings;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBR_ResourceRefinerComponent* RefinerComponent;
    
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable) 
    void RegisterBuildingsConnection();
    UFUNCTION(BlueprintCallable)
    void RegisterBuildingsDisconnection();


};

