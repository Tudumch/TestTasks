// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PlayerHUD.h"

#include "Blueprint/UserWidget.h"

void APlayerHUD::BeginPlay()
{
    Super::BeginPlay();

    WidgetHUD = CreateWidget<UUserWidget>(GetWorld(), WidgetHUDClass);

    if (WidgetHUD) WidgetHUD->AddToViewport();
}
