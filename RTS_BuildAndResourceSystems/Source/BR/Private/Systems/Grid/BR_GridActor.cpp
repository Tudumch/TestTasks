// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Grid/BR_GridActor.h"

#include "StaticMeshAttributes.h"
#include "Actors/Buildings/BR_GridObject.h"
#include "Components/BR_ResourceRefinerComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/BR_HUD_Gameplay.h"
#include "UI/Widgets/BR_ResourceMapper.h"

// Sets default values
ABR_GridActor::ABR_GridActor()
{
	PrimaryActorTick.bCanEverTick = true;

    
    RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
    ResourcesWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("ResourcesWidgetComponent");
    ResourcesWidgetComponent->SetupAttachment(RootComponent);
    ResourcesWidgetComponent->AddLocalOffset(FVector(0, 0, ResourcesWidgetHeight));
    ResourcesWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ResourcesWidgetComponent->SetCastShadow(false);
    
    SetActorEnableCollision(false);

    StoredResources.Add(EResourceTypes::Energy, 0);
    StoredResources.Add(EResourceTypes::Water, 0);
    StoredResources.Add(EResourceTypes::Oxygen, 0);
    StoredResources.Add(EResourceTypes::Food, 0);
}

void ABR_GridActor::BeginPlay()
{
	Super::BeginPlay();

    SetResourcesWidget();
}

void ABR_GridActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    RotateWidgetTowardPlayerCamera();
}


void ABR_GridActor::AddBuilding(ABR_GridObject* &Building)
{
    if (!Building) return;
    
    if (!BuildingsInGrid.Contains(Building)) 
    {
        Building->SetGrid(this);
        BuildingsInGrid.Add(Building);
        UE_LOG(LogTemp, Display, TEXT("Building added in grid %s"), *this->GetName());
    }

    UpdatePosition();
}

void ABR_GridActor::AddBuildings(TSet<ABR_GridObject*> &Buildings)
{
    for (auto Building : Buildings)
    {
        AddBuilding(Building);
    }
}

void ABR_GridActor::RemoveBuilding(ABR_GridObject* &Building)
{
    Building->SetGrid(nullptr);
    BuildingsInGrid.Remove(Building);

    UpdatePosition();
}

void ABR_GridActor::RemoveBuildings(TSet<ABR_GridObject*> &Buildings)
{
    for (auto Building : Buildings)
        RemoveBuilding(Building);
}

TSet<ABR_GridObject*> ABR_GridActor::ClearGrid()
{
    if (BuildingsInGrid.IsEmpty()) return TSet<ABR_GridObject*>();

    TSet<ABR_GridObject*> OrphanBuildings;
    
    for (auto Building : BuildingsInGrid)
    {
        Building->SetGrid(nullptr);
        OrphanBuildings.Add(Building);
    }

    BuildingsInGrid.Empty();
    return OrphanBuildings;
}

void ABR_GridActor::DoProductionCycle()
{
    for (const auto Building : BuildingsInGrid)
        if (UBR_ResourceRefinerComponent* ResourceRefiner = Building->GetComponentByClass<UBR_ResourceRefinerComponent>())
            StoredResources = ResourceRefiner->ProduceResources(StoredResources);
    
    // Update info in the widget
    if (ResourcesWidget)
        ResourcesWidget->UpdateResources(StoredResources);
}

void ABR_GridActor::UpdatePosition()
{
    if (BuildingsInGrid.Num() == 0) return;
    
    FVector SumLocation = FVector::ZeroVector;
    
    for (AActor* Actor : BuildingsInGrid)
        SumLocation += Actor->GetActorLocation();
    
    SetActorLocation(SumLocation / BuildingsInGrid.Num());
}

void ABR_GridActor::SetResourcesWidget()
{
    const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;

    const ABR_HUD_Gameplay* HUD = Cast<ABR_HUD_Gameplay>(PlayerController->GetHUD());
    if (!HUD) return;

    ResourcesWidget = Cast<UBR_ResourceMapper>(CreateWidget<UUserWidget>(GetWorld(), HUD->GetGridResourcesWidgetClass()));
    ResourcesWidgetComponent->SetWidget(ResourcesWidget);
    ResourcesWidget->UpdateResources(StoredResources);
}

void ABR_GridActor::RotateWidgetTowardPlayerCamera()
{
    if (!ResourcesWidgetComponent) return;
    
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;

    APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
    if (!CameraManager) return;

    FVector CameraLocation = CameraManager->GetCameraLocation();
    FVector WidgetLocation = ResourcesWidgetComponent->GetComponentLocation();

    FVector DirectionToCamera = (CameraLocation - WidgetLocation).GetSafeNormal();

    FRotator NewRotation = DirectionToCamera.Rotation();
    NewRotation.Pitch = 0.0f;
    NewRotation.Roll = 0.0f;

    ResourcesWidgetComponent->SetWorldRotation(NewRotation);
}

void ABR_GridActor::AddResources(TMap<EResourceTypes, int32>& Resources)
{
    for (const auto Resource : Resources)
    {
        if (StoredResources.Find(Resource.Key))
            StoredResources[Resource.Key] += Resource.Value;
        else
            StoredResources.Add(Resource.Key, Resource.Value);
    }
}