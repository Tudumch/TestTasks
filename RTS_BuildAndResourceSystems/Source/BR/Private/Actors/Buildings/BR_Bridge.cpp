// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildings/BR_Bridge.h"

#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
ABR_Bridge::ABR_Bridge()
{
    PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = BoxComponent;
    BoxComponent->SetCollisionProfileName(TEXT("Custom"));
    BoxComponent->SetCollisionObjectType(ECC_Pawn);
    BoxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
    BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    
    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
    SplineComponent->SetupAttachment(GetRootComponent());

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
    WidgetComponent->SetupAttachment(SplineComponent);
}

// Called when the game starts or when spawned
void ABR_Bridge::BeginPlay()
{
	Super::BeginPlay();

    WidgetComponent->OnClicked.AddDynamic(this, &ThisClass::OnBridgeButtonClicked);
}

void ABR_Bridge::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    RotateWidgetTowardPlayerCamera();
}


void ABR_Bridge::OnBridgeButtonClicked(UPrimitiveComponent* TouchedComponent , FKey ButtonPressed)
{
    if (!bCanBePlaced) return;
    
    GenerateMeshAlongTheSpline(FinalMesh.Get());
    WidgetComponent->DestroyComponent();
    RegisterBuildingsConnection();
    bIsBridgePlaced = true;
}

bool ABR_Bridge::IsPathBetweenActorsClear(AActor* &ActorFirst, AActor* &ActorSecond)
{
    FCollisionQueryParams TraceParams;
    TraceParams.bTraceComplex = true; 
    TraceParams.AddIgnoredActor(this); 
    TraceParams.AddIgnoredActor(ActorFirst); 
    TraceParams.AddIgnoredActor(ActorSecond); 

    FHitResult HitResult;

    bCanBePlaced = !GetWorld()->LineTraceSingleByChannel(
        HitResult,
        ActorFirst->GetActorLocation(),
        ActorSecond->GetActorLocation(),
        ECC_Pawn, 
        TraceParams
    );

    return bCanBePlaced;
}

void ABR_Bridge::PlaceBridgeProjectionBetweenActors(AActor* &ActorFirst, AActor* &ActorSecond)
{
    SplineComponent->SetLocationAtSplinePoint(0, ActorFirst->GetActorLocation(), ESplineCoordinateSpace::World);
    SplineComponent->SetLocationAtSplinePoint(1, ActorSecond->GetActorLocation(), ESplineCoordinateSpace::World);

    LinkedBuildings.Add(Cast<ABR_GridObject>(ActorFirst));
    LinkedBuildings.Add(Cast<ABR_GridObject>(ActorSecond));
    
    if (IsPathBetweenActorsClear(ActorFirst, ActorSecond))
        GenerateMeshAlongTheSpline(ValidProjectionMesh.Get());
    else
        GenerateMeshAlongTheSpline(InvalidProjectionMesh.Get());
}

void ABR_Bridge::GenerateMeshAlongTheSpline(UStaticMesh* Mesh)
{
    if (!Mesh) return;
    
    // Clear old SplineMeshComponents
    for (USplineMeshComponent* SplineMeshComponent : SplineMeshComponents)
    {
        SplineMeshComponent->DestroyComponent(false);
    }
    SplineMeshComponents.Empty();

    // Get mesh length
    const FBox MeshBounds = Mesh->GetBoundingBox();
    const FVector MinBound = MeshBounds.Min.GetAbs();
    const FVector MaxBound = MeshBounds.Max.GetAbs();
    
    const double LengthX = (MinBound + MaxBound).X;;

    // Create mesh segments along the spline
    const int SplineSegmentsCount = FMath::TruncToInt(SplineComponent->GetSplineLength() / LengthX);

    for (int i = 0; i <= SplineSegmentsCount; i++)
    {
        // Create and setup SplineMeshComponent
        USplineMeshComponent* SplineMeshComponent = Cast<USplineMeshComponent>(AddComponentByClass
        (USplineMeshComponent::StaticClass(), false, FTransform::Identity, true));
        SplineMeshComponent->SetMobility(EComponentMobility::Movable);
        SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::X, true);
        SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        SplineMeshComponent->SetCollisionObjectType(ECC_Pawn);
        SplineMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
        SplineMeshComponent->SetStaticMesh(Mesh);
        FinishAddComponent(SplineMeshComponent, false, FTransform::Identity);


        // Get FVectors for SplineMeshComponent positioning
        const FVector StartPosition = SplineComponent->GetLocationAtDistanceAlongSpline(i*LengthX, 
        ESplineCoordinateSpace::Local);
        
        FVector StartTangents = SplineComponent->GetTangentAtDistanceAlongSpline(i*LengthX, 
        ESplineCoordinateSpace::Local);
        StartTangents = StartTangents.GetClampedToSize(0, LengthX);
        
        const FVector EndPosition = SplineComponent->GetLocationAtDistanceAlongSpline((i+1)*LengthX, 
        ESplineCoordinateSpace::Local);
        
        FVector EndTangents = SplineComponent->GetTangentAtDistanceAlongSpline((i+1)*LengthX, ESplineCoordinateSpace::Local);
        EndTangents = EndTangents.GetClampedToSize(0, LengthX);


        // Set SplineMeshComponent position
        SplineMeshComponent->SetStartAndEnd(StartPosition, StartTangents, EndPosition, EndTangents, true);
        SplineMeshComponents.Add(SplineMeshComponent);
    }
}

void ABR_Bridge::RotateWidgetTowardPlayerCamera()
{
    if (WidgetComponent && WidgetComponent->IsWidgetVisible())
    {
        const APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager; 
        const FVector WidgetLocation = WidgetComponent->GetComponentLocation();
        const FVector CameraLocation = CameraManager->GetCameraLocation();
        
        FRotator LookAtRotation = FRotationMatrix::MakeFromX(CameraLocation - WidgetLocation).Rotator();

        WidgetComponent->SetWorldRotation(LookAtRotation);
    }
}