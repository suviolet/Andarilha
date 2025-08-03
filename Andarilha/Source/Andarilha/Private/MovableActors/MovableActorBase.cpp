#include "MovableActors/MovableActorBase.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"

AMovableActorBase::AMovableActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	MovingDirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("MovingDirectionArrow"));
	MovingDirectionArrow->SetupAttachment(RootComponent);

	SfxComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SfxComponent"));
	SfxComponent->SetupAttachment(RootComponent);
	startTime = 0.f;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(FName("BlockAll"), false);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetupAttachment(RootComponent);

	currentPointIdx = 0;
	bIsTriggerable = true;
	bIsMovingForward = true;
	bIsMoving = false;

	MovementCurve = CreateDefaultSubobject<UCurveFloat>(TEXT("MovementCurve"));
	MovementCurve->FloatCurve.Reset();
	MovementCurve->FloatCurve.UpdateOrAddKey(0.f, 0.f);
	MovementCurve->FloatCurve.UpdateOrAddKey(1.f, 1.f);

	movementSpeed = 5;

	TimelineMovementComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineMovementComp"));
}

void AMovableActorBase::BeginPlay()
{
	Super::BeginPlay();

	Spline = SplineActor->Spline;
	numberOfSplinePoints = SplineActor->Spline->GetNumberOfSplinePoints();

	TimelineMovingCallback.BindDynamic(this, &AMovableActorBase::MoveToPoint);
	TimelineMovementComp->SetPlayRate(1 / movementSpeed);
	TimelineMovementComp->AddInterpFloat(MovementCurve, TimelineMovingCallback);

	if (MovingSfx != nullptr)
	{
		SfxComponent->SetSound(MovingSfx);
	}

	if (!bIsTriggerable)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMovableActorBase::BeginPlay IF"));
		TimelineMovingFinishedCallback.BindUFunction(this, "UnsetMovementBool");
		TimelineMovementComp->SetTimelineFinishedFunc(TimelineMovingFinishedCallback);

		CurrenctLocation = Spline->GetSplinePointAt(0, ESplineCoordinateSpace::World);
		NextLocation = Spline->GetSplinePointAt(numberOfSplinePoints - 1, ESplineCoordinateSpace::World);

		if (SfxComponent->GetSound() != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("AMovableActorBase::BeginPlay SoundName: %s , Duration: %f"), *MovingSfx->GetName(), MovingSfx->GetDuration());
			SfxComponent->Play(startTime);
		}
	}
}

void AMovableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsTriggerable && !bIsMoving)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMovableActorBase::Tick IF:"));
		bIsMoving = true;
		if (bIsMovingForward)
		{
			TimelineMovementComp->PlayFromStart();
			bIsMovingForward = false;
		}
		else
		{
			TimelineMovementComp->Reverse();
			bIsMovingForward = true;
		}
	}
}

void AMovableActorBase::UnsetMovementBool()
{
	bIsMoving = false;
}

void AMovableActorBase::OnMovableTriggered()
{
	if (bIsTriggerable)
	{
		if (bIsMovingForward)
		{
			OnMoveNextTriggered();
		}
		else
		{
			OnMovePreviousTriggered();
		}
	}
}

void AMovableActorBase::OnMoveNextTriggered()
{
	if (currentPointIdx < numberOfSplinePoints - 1)
	{
		CurrenctLocation = Spline->GetSplinePointAt(currentPointIdx, ESplineCoordinateSpace::World);
		NextLocation = Spline->GetSplinePointAt(currentPointIdx + 1, ESplineCoordinateSpace::World);
		currentPointIdx++;
	}
	else if (currentPointIdx == numberOfSplinePoints - 1)
	{
		bIsMovingForward = false;
		OnMovePreviousTriggered();
	}

	if (SfxComponent->GetSound() != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("AMovableActorBase::OnMoveNextTriggered SoundName: %s , Duration: %f"), *MovingSfx->GetName(), MovingSfx->GetDuration());
		SfxComponent->Play(startTime);
	}
	TimelineMovementComp->PlayFromStart();
}

void AMovableActorBase::OnMovePreviousTriggered()
{
	if (0 < currentPointIdx and currentPointIdx < numberOfSplinePoints)
	{
		CurrenctLocation = Spline->GetSplinePointAt(currentPointIdx, ESplineCoordinateSpace::World);
		NextLocation = Spline->GetSplinePointAt(currentPointIdx - 1, ESplineCoordinateSpace::World);
		currentPointIdx--;
	}
	else if (currentPointIdx == 0)
	{
		bIsMovingForward = true;
		OnMoveNextTriggered();
	}

	if (SfxComponent->GetSound() != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("AMovableActorBase::OnMovePreviousTriggered SoundName: %s , Duration: %f"), *MovingSfx->GetName(), MovingSfx->GetDuration());
		SfxComponent->Play(startTime);
	}
	TimelineMovementComp->PlayFromStart();
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

#if WITH_EDITOR
void AMovableActorBase::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);
	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AMovableActorBase, movementSpeed))
	{
		TimelineMovementComp->SetPlayRate(1 / movementSpeed);
	}
}
#endif
