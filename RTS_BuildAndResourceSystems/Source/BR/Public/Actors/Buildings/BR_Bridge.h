// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BR_GridObject.h"
#include "BR_Bridge.generated.h"

class UBoxComponent;
class USplineMeshComponent;

UCLASS()
class BR_API ABR_Bridge : public ABR_GridObject
{
	GENERATED_BODY()
	
public:	
	ABR_Bridge();

    // Checks and visualizes is path between two buildings clear and allows to place bridge.
    UFUNCTION(BlueprintCallable)
    void PlaceBridgeProjectionBetweenActors(AActor* &ActorFirst, AActor* &ActorSecond);

    // Return true if bridge was built in the world
    UFUNCTION(BlueprintPure)
    bool IsBridgePlaced() const {return bIsBridgePlaced;}

protected:
    UPROPERTY()
    bool bIsBridgePlaced = false;
    UPROPERTY()
    bool bCanBePlaced = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UBoxComponent> BoxComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite);
    TObjectPtr<class UWidgetComponent> WidgetComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite);
    TObjectPtr<class USplineComponent> SplineComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Meshes");
    TObjectPtr<UStaticMesh> FinalMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Meshes");
    TObjectPtr<UStaticMesh> ValidProjectionMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Meshes");
    TObjectPtr<UStaticMesh> InvalidProjectionMesh;

    UPROPERTY()
    TArray<USplineMeshComponent*> SplineMeshComponents;

    
	virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnBridgeButtonClicked(UPrimitiveComponent* TouchedComponent , FKey ButtonPressed);

    UFUNCTION()
    bool IsPathBetweenActorsClear(AActor* &ActorFirst, AActor* &ActorSecond);
    
    UFUNCTION(BlueprintCallable)
    void GenerateMeshAlongTheSpline(UStaticMesh* Mesh);

    UFUNCTION(BlueprintCallable)
    void RotateWidgetTowardPlayerCamera();
};
