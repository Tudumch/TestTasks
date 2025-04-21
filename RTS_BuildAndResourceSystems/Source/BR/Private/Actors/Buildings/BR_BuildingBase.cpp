// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildings/BR_BuildingBase.h"

#include "Actors/Buildings/BR_Bridge.h"
#include "Components/SphereComponent.h"

ABR_BuildingBase::ABR_BuildingBase()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    SphereComp->SetCollisionObjectType(ECC_Pawn);
    SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SphereComp->SetupAttachment(RootComponent);
    
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(SphereComp);
    
    SelectionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionMesh"));
    SelectionMesh->SetHiddenInGame(true);
    SelectionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SelectionMesh->SetupAttachment(StaticMesh);
}

void ABR_BuildingBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (EndPlayReason == EEndPlayReason::Destroyed)
    {
        for (int32 i = LinkedBuildings.Num() - 1; i >= 0; --i)
            if (ABR_Bridge* Bridge = Cast<ABR_Bridge>(LinkedBuildings[i]))
                Bridge->Destroy();
    }
    
    Super::EndPlay(EndPlayReason);
}

void ABR_BuildingBase::Select(const bool IsSelected)
{
    IsSelected ? SelectionMesh->SetHiddenInGame(false) : SelectionMesh->SetHiddenInGame(true);
}

void ABR_BuildingBase::BeginPlay()
{
	Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &ThisClass::OnOverlapEnd);
	
}

void ABR_BuildingBase::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    UE_LOG(LogTemp, Display, TEXT("OnOverlapBegin"));
    if (!OverlappingActors.Contains(OtherActor))
        OverlappingActors.Add(OtherActor);
}

void ABR_BuildingBase::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
    OverlappingActors.Remove(OtherActor);
}


