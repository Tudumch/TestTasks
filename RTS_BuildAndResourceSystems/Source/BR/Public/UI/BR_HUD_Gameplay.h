// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widgets/BR_ResourceMapper.h"
#include "BR_HUD_Gameplay.generated.h"

class ABR_Bridge;
class ISelectableObject;
class UBR_Widget_GameplayMaster;
class ABR_BuildingBase;

UCLASS()
class BR_API ABR_HUD_Gameplay : public AHUD
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void StartPrimaryAction(const bool bIsMultiSelectionActive = false);
    UFUNCTION(BlueprintCallable)
    void StartSecondaryAction();
    
    UFUNCTION(BlueprintCallable)
    void SpawnBuildingProjection(const TSubclassOf<ABR_BuildingBase>& BuildingClass);
    
    UFUNCTION(BlueprintCallable)
    void ActivateDemolishMode();
    UFUNCTION(BlueprintCallable)
    void ActivateBridgeMode(const TSubclassOf<ABR_Bridge> BridgeClass);

    UFUNCTION(BlueprintPure)
    TSubclassOf<UBR_ResourceMapper> GetGridResourcesWidgetClass() const { return GridResourcesWidgetClass; };

protected:
    TArray<ISelectableObject*> SelectedObjects;
    
    UPROPERTY()
    bool bIsDemolishModeOn = false;
    UPROPERTY()
    bool bIsBridgeModeOn = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI/Widgets")
    UUserWidget* MasterWidget;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI/Widgets")
    TSubclassOf<UBR_ResourceMapper> GridResourcesWidgetClass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI/Coursors")
    UUserWidget* DemolishMouseCursor;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI/Coursors")
    UUserWidget* BridgeMouseCursor;
    
    UPROPERTY()
    ABR_BuildingBase* ActiveBuildingProjection;
    UPROPERTY()
    TSubclassOf<ABR_Bridge> BridgeType;
    UPROPERTY()
    ABR_Bridge* ActiveBridgeProjection;

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    UFUNCTION(BlueprintCallable)
    void ClearBuildingProjection();
    UFUNCTION(BlueprintCallable)
    void PlaceBuildingProjection();
    UFUNCTION(BlueprintCallable)
    void ClearBridgeProjection();
    UFUNCTION(BlueprintCallable)
    void SpawnBridgeProjection();

    UFUNCTION(BlueprintCallable)
    void PerformSelection(bool bIsMultiSelectionActive = false);
    UFUNCTION(BlueprintCallable)
    void PerformSelection_BridgeMode();
    UFUNCTION(BlueprintCallable)
    void ClearSelection();

    UFUNCTION(BlueprintCallable)
    void PerformDemolition();
};
