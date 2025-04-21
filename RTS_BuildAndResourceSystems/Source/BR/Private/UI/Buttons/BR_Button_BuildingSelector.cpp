// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Buttons/BR_Button_BuildingSelector.h"

#include "Blueprint/WidgetTree.h"
#include "UI/BR_HUD_Gameplay.h"

UBR_Button_BuildingSelector::UBR_Button_BuildingSelector()
{
    OnClicked.AddDynamic(this, &ThisClass::OnButtonClicked);
}


void UBR_Button_BuildingSelector::OnButtonClicked()
{
    if (!BuildingClass) return;
    
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;

    ABR_HUD_Gameplay* HUD = Cast<ABR_HUD_Gameplay>(PlayerController->GetHUD());
    if (!HUD) return;
    
    HUD->SpawnBuildingProjection(BuildingClass);
}