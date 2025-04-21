// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BR_GridObject.h"
#include "GameFramework/Actor.h"
#include "BR_BuildingBase.generated.h"

class UBR_ResourceRefinerComponent;

UCLASS()
class BR_API ABR_BuildingBase : public ABR_GridObject
{
	GENERATED_BODY()
	
public:	
	ABR_BuildingBase();

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION(BlueprintPure)
    bool IsNotOverlapSomething() const { return OverlappingActors.IsEmpty(); };

    virtual void Select(const bool IsSelected) override;

protected:
    UPROPERTY()
    TArray<AActor*> OverlappingActors;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class USphereComponent* SphereComp;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* SelectionMesh;

    
	virtual void BeginPlay() override;

    UFUNCTION()
    virtual void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
    UFUNCTION()
    virtual void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);
};
