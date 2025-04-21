// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Buttons/BR_Button_BridgeMode.h"

#include "Actors/Buildings/BR_Bridge.h"
#include "UI/BR_HUD_Gameplay.h"

UBR_Button_BridgeMode::UBR_Button_BridgeMode()
{
    OnClicked.AddDynamic(this, &ThisClass::OnButtonClicked);
}

void UBR_Button_BridgeMode::OnButtonClicked()
{
    if (!BridgeClass) return;
    
    const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;

    ABR_HUD_Gameplay* HUD = Cast<ABR_HUD_Gameplay>(PlayerController->GetHUD());
    if (!HUD) return;
    
    HUD->ActivateBridgeMode(BridgeClass);
}