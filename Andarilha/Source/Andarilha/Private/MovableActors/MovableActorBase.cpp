#include "MovableActors/MovableActorBase.h"

AMovableActorBase::AMovableActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(FName("BlockAll"), false); //BlockAllDynamic
	Mesh->SetSimulatePhysics(false);//true
	Mesh->SetupAttachment(RootComponent);

	currentPointIdx = 0;
	bIsMovingForward = true;

	Curve = CreateDefaultSubobject<UCurveFloat>(TEXT("Curve"));
	Curve->FloatCurve.Reset();
	Curve->FloatCurve.UpdateOrAddKey(0.f, 0.f);
	Curve->FloatCurve.UpdateOrAddKey(1.f, 1.f);

	speedMovement = 5;
	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));

}

void AMovableActorBase::BeginPlay()
{
	Super::BeginPlay();
	Spline = SplineActor->Spline;
	TimelineCallback.BindDynamic(this, &AMovableActorBase::MoveToPoint);
	numberOfSplinePoints = SplineActor->Spline->GetNumberOfSplinePoints();
	TimelineComp->SetPlayRate(1 / speedMovement);
	TimelineComp->AddInterpFloat(Curve, TimelineCallback);
}

void AMovableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMovableActorBase::MoveToPoint(float Alpha)
{
	FRotator ArrowRotation = Arrow->GetRelativeRotation();
	FTransform start = Spline->GetTransformAtSplinePoint(CurrenctLocation.InputKey, ESplineCoordinateSpace::World);
	FTransform end = Spline->GetTransformAtSplinePoint(NextLocation.InputKey, ESplineCoordinateSpace::World);

	FVector newLocation = UKismetMathLibrary::VLerp(start.GetLocation(), end.GetLocation(), Alpha);
	FRotator newRotation = UKismetMathLibrary::RLerp(start.Rotator() + ArrowRotation, end.Rotator() + ArrowRotation, Alpha, true);

	this->RootComponent->SetWorldLocationAndRotation(newLocation, newRotation);
}

void AMovableActorBase::OnMovableTriggered()
{
	//int32 destinyIdx = nullptr    // MAKE THIS call based on a destiny
	if (bIsMovingForward)
	{
		OnMoveNextTriggered();
	}
	else
	{
		OnMovePreviousTriggered();
	}
}

void AMovableActorBase::OnMoveNextTriggered()
{
	if (currentPointIdx < numberOfSplinePoints - 1)
	{
		CurrenctLocation = Spline->GetSplinePointAt(currentPointIdx, ESplineCoordinateSpace::World);
		NextLocation = Spline->GetSplinePointAt(currentPointIdx+1, ESplineCoordinateSpace::World);
		currentPointIdx++;
	}
	else if (currentPointIdx == numberOfSplinePoints - 1)
	{
		bIsMovingForward = false;
		OnMovePreviousTriggered();
	}
	TimelineComp->PlayFromStart();
}

void AMovableActorBase::OnMovePreviousTriggered()
{
	if (0 < currentPointIdx and currentPointIdx < numberOfSplinePoints)
	{
		CurrenctLocation = Spline->GetSplinePointAt(currentPointIdx, ESplineCoordinateSpace::World);
		NextLocation = Spline->GetSplinePointAt(currentPointIdx-1, ESplineCoordinateSpace::World);
		currentPointIdx--;
	}
	else if (currentPointIdx == 0)
	{
		bIsMovingForward = true;
		OnMoveNextTriggered();
	}
	TimelineComp->PlayFromStart();
}

#if WITH_EDITOR
void AMovableActorBase::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);
	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AMovableActorBase, speedMovement))
	{
		TimelineComp->SetPlayRate(1/speedMovement);
	}
}
#endif