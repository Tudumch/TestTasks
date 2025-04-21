// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BR_HUD_Gameplay.h"

#include "Actors/Buildings/BR_Bridge.h"
#include "Actors/Buildings/BR_BuildingBase.h"
#include "Blueprint/UserWidget.h"
#include "Systems/Grid/BR_GridActor.h"

void ABR_HUD_Gameplay::BeginPlay()
{
    Super::BeginPlay();

    if (MasterWidget)
        MasterWidget->AddToViewport();
    else
        UE_LOG(LogTemp, Error, TEXT("HUD MasterWidget is not set!"));
}

void ABR_HUD_Gameplay::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ActiveBuildingProjection)
    {
        FHitResult HitResult;
        if (PlayerOwner->GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, HitResult))
            ActiveBuildingProjection->GetRootComponent()->SetWorldLocation(HitResult.ImpactPoint);
    }
}

void ABR_HUD_Gameplay::StartPrimaryAction(const bool bIsMultiSelectionActive)
{
    if (ActiveBuildingProjection)
    {
        PlaceBuildingProjection();
        return;
    }

    if (bIsBridgeModeOn)
        PerformSelection_BridgeMode();
    else
        PerformSelection(bIsMultiSelectionActive);

    if (bIsDemolishModeOn)
        PerformDemolition();
}

void ABR_HUD_Gameplay::StartSecondaryAction()
{
    ClearSelection();
    ClearBuildingProjection();
    ClearBridgeProjection();
    bIsDemolishModeOn = false;
    bIsBridgeModeOn = false;
    GetOwningPlayerController()->SetMouseCursorWidget(EMouseCursor::Default, nullptr);
}

void ABR_HUD_Gameplay::SpawnBuildingProjection(const TSubclassOf<ABR_BuildingBase>& BuildingClass)
{
    StartSecondaryAction(); // clearing all
    ActiveBuildingProjection = GetWorld()->SpawnActor<ABR_BuildingBase>(BuildingClass, FVector(), FRotator()); 
}

void ABR_HUD_Gameplay::ActivateDemolishMode()
{
    StartSecondaryAction(); // clearing selection and Modes
    bIsDemolishModeOn = true;
    GetOwningPlayerController()->SetMouseCursorWidget(EMouseCursor::Default, DemolishMouseCursor);
}

void ABR_HUD_Gameplay::ActivateBridgeMode(const TSubclassOf<ABR_Bridge> BridgeClass)
{
    BridgeType = BridgeClass;
    StartSecondaryAction(); // clearing selection and Modes
    bIsBridgeModeOn = true;
    GetOwningPlayerController()->SetMouseCursorWidget(EMouseCursor::Default, BridgeMouseCursor);
}

void ABR_HUD_Gameplay::ClearBuildingProjection()
{
    if (!ActiveBuildingProjection) return;
    
    ActiveBuildingProjection->Destroy();
    ActiveBuildingProjection = nullptr;
}

void ABR_HUD_Gameplay::PlaceBuildingProjection()
{
    if (ActiveBuildingProjection && ActiveBuildingProjection->IsNotOverlapSomething())
    {
        ActiveBuildingProjection = nullptr;
    }
}

void ABR_HUD_Gameplay::ClearBridgeProjection()
{
    if (!ActiveBridgeProjection) return;

    if (ActiveBridgeProjection->IsBridgePlaced() == false)
        ActiveBridgeProjection->Destroy();
    
    ActiveBridgeProjection = nullptr;
}

void ABR_HUD_Gameplay::SpawnBridgeProjection()
{
    ClearBridgeProjection();
    
    if (SelectedObjects.Num() > 1)
    {
        AActor* BuildingFirst = Cast<AActor>(SelectedObjects[0]);
        AActor* BuildingSecond = Cast<AActor>(SelectedObjects[1]);
        
        const FVector LocationFirst = BuildingFirst->GetActorLocation(); 
        const FVector LocationSecond = BuildingSecond->GetActorLocation(); 
        const FVector LocationMiddle = (LocationFirst + LocationSecond) / 2;

        ActiveBridgeProjection = GetWorld()->SpawnActor<ABR_Bridge>(BridgeType);
        ActiveBridgeProjection->SetActorLocation(LocationMiddle);

        ActiveBridgeProjection->PlaceBridgeProjectionBetweenActors(BuildingFirst, BuildingSecond);
    }
}

void ABR_HUD_Gameplay::PerformSelection(bool bIsMultiSelectionActive)
{
    if (!bIsMultiSelectionActive)
        ClearSelection();
    
    FHitResult HitResult;
    PlayerOwner->GetHitResultUnderCursor(ECC_Pawn, false, HitResult);

    if (ISelectableObject* SelectableObject = Cast<ISelectableObject>(HitResult.GetActor()))
    {
        if (SelectedObjects.Contains(SelectableObject)) return;
        SelectableObject->Select(true);
        SelectedObjects.Add(SelectableObject);
    }
    else
        ClearSelection();

    if (!SelectedObjects.IsEmpty())
    {
        ABR_GridObject* SelectedObject = Cast<ABR_GridObject>(SelectedObjects[0]);
        if (!SelectedObject) return;

        if (ABR_GridActor* Grid = SelectedObject->GetGrid())
            UE_LOG(LogTemp, Display, TEXT("Selected Grid: %s"), *Grid->GetName())
        else
            UE_LOG(LogTemp, Warning, TEXT("Selected Grid: NONE"));
    }
}

void ABR_HUD_Gameplay::PerformSelection_BridgeMode()
{
    FHitResult HitResult;
    PlayerOwner->GetHitResultUnderCursor(ECC_Pawn, false, HitResult);

    if (HitResult.GetActor()->IsA(ABR_Bridge::StaticClass()))
        return;

    if (ISelectableObject* SelectableObject = Cast<ISelectableObject>(HitResult.GetActor()))
    {
        if (SelectedObjects.Contains(SelectableObject)) return;
        SelectableObject->Select(true);
        SelectedObjects.Insert(SelectableObject, 0);

        if (SelectedObjects.Num() > 2)
            SelectedObjects.Pop(EAllowShrinking::Yes)->Select(false);
    }
    else
        ClearSelection();

    SpawnBridgeProjection();
}

void ABR_HUD_Gameplay::ClearSelection()
{
    for (ISelectableObject* Object : SelectedObjects)
    {
        Object->Select(false);
    }
    SelectedObjects.Empty();
}

void ABR_HUD_Gameplay::PerformDemolition()
{
    if (SelectedObjects.IsEmpty()) return;
    
    AActor* Object = Cast<AActor>(SelectedObjects.Last());
    if (!Object) return;
    
    Object->Destroy();
}

