#include "MovableActors/MovableActorBase.h"
#include "Components/BoxComponent.h"

AMovableActorBase::AMovableActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	MovingDirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("MovingDirectionArrow"));
	MovingDirectionArrow->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(FName("BlockAll"), false);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetupAttachment(RootComponent);

	LeftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoorMesh"));
	LeftDoorMesh->SetCollisionProfileName(FName("BlockAll"), false);
	LeftDoorMesh->SetSimulatePhysics(false);
	LeftDoorMesh->SetupAttachment(Mesh);

	RightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoorMesh"));
	RightDoorMesh->SetCollisionProfileName(FName("BlockAll"), false);
	RightDoorMesh->SetSimulatePhysics(false);
	RightDoorMesh->SetupAttachment(Mesh);

	EntranceDoorBox = CreateDefaultSubobject<UBoxComponent>(TEXT("EntranceDoorBox"));
	EntranceDoorBox->SetupAttachment(RootComponent);
	EntranceDoorBox->OnComponentEndOverlap.AddDynamic(this, &AMovableActorBase::OnEntranceEndOverlap);

	EntranceDirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("EntranceDirectionArrow"));
	EntranceDirectionArrow->SetupAttachment(EntranceDoorBox);

	currentPointIdx = 0;
	bIsMovingForward = true;

	Curve = CreateDefaultSubobject<UCurveFloat>(TEXT("Curve"));
	Curve->FloatCurve.Reset();
	Curve->FloatCurve.UpdateOrAddKey(0.f, 0.f);
	Curve->FloatCurve.UpdateOrAddKey(1.f, 1.f);

	speedMovement = 5;
	TimelineMovementComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineMovementComp"));
	TimelineDoorComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineDoorComp"));

}

void AMovableActorBase::BeginPlay()
{
	Super::BeginPlay();

	Spline = SplineActor->Spline;
	numberOfSplinePoints = SplineActor->Spline->GetNumberOfSplinePoints();

	TimelineMovingCallback.BindDynamic(this, &AMovableActorBase::MoveToPoint);
	TimelineMovementComp->SetPlayRate(1 / speedMovement);
	TimelineMovementComp->AddInterpFloat(Curve, TimelineMovingCallback);

	TimelineOpenCloseDoorCallback.BindDynamic(this, &AMovableActorBase::CloseDoor);
	TimelineDoorComp->SetPlayRate(1 / speedMovement);
	TimelineDoorComp->AddInterpFloat(Curve, TimelineOpenCloseDoorCallback);
}

void AMovableActorBase::MoveToPoint(float Alpha)
{
	FRotator ArrowRotation = MovingDirectionArrow->GetRelativeRotation();
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
	TimelineMovementComp->PlayFromStart();
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
	TimelineMovementComp->PlayFromStart();
}

#if WITH_EDITOR
void AMovableActorBase::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);
	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AMovableActorBase, speedMovement))
	{
		TimelineMovementComp->SetPlayRate(1/speedMovement);
	}
}
#endif

void AMovableActorBase::OnEntranceEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->ActorHasTag(FName("PCharacter")))
		{
			float dotProduct = OtherActor->GetActorForwardVector() | EntranceDirectionArrow->GetForwardVector();
			if (dotProduct > 0)
			{
				//CloseDoor();
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("AMovableActorBase::OnEntranceEndOverlap"));
				TimelineDoorComp->PlayFromStart();
			}
		}
	}
}

void AMovableActorBase::OpenDoor(float Alpha)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("AMovableActorBase::OpenDoor"));
	// opens door
	// plays sound while opening door
}

void AMovableActorBase::CloseDoor(float Alpha)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("AMovableActorBase::CloseDoor"));
	// closes door
	// plays sound while closing door
}