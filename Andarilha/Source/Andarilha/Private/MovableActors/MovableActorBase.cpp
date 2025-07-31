#include "MovableActors/MovableActorBase.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

AMovableActorBase::AMovableActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

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

	MovementCurve = CreateDefaultSubobject<UCurveFloat>(TEXT("MovementCurve"));
	MovementCurve->FloatCurve.Reset();
	MovementCurve->FloatCurve.UpdateOrAddKey(0.f, 0.f);
	MovementCurve->FloatCurve.UpdateOrAddKey(1.f, 1.f);

	DoorCurve = CreateDefaultSubobject<UCurveFloat>(TEXT("DoorCurve"));

	movementSpeed = 5;
	openCloseDoorSpeed = 1.5f; // Important to take into account OpenCloseDoorSfx time
	TimelineMovementComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineMovementComp"));
	TimelineDoorComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineDoorComp"));

	bIsDoorOpen = true;

}

void AMovableActorBase::BeginPlay()
{
	Super::BeginPlay();

	Spline = SplineActor->Spline;
	numberOfSplinePoints = SplineActor->Spline->GetNumberOfSplinePoints();

	TimelineMovingCallback.BindDynamic(this, &AMovableActorBase::MoveToPoint);
	TimelineMovingFinishedCallback.BindUFunction(this, "OpenDoor");
	TimelineMovementComp->SetPlayRate(1 / movementSpeed);
	TimelineMovementComp->AddInterpFloat(MovementCurve, TimelineMovingCallback);
	TimelineMovementComp->SetTimelineFinishedFunc(TimelineMovingFinishedCallback);

	UStaticMesh* rMesh = RightDoorMesh->GetStaticMesh();
	UStaticMesh* lMesh = LeftDoorMesh->GetStaticMesh();
	if ((rMesh != nullptr || lMesh != nullptr))
	{
		float rightBounding = rMesh->GetBoundingBox().GetSize().GetAbs().X;
		float leftBounding = lMesh->GetBoundingBox().GetSize().GetAbs().X;
		DoorCurve->FloatCurve.Reset();
		DoorCurve->FloatCurve.UpdateOrAddKey(1.f, 0.f);
		DoorCurve->FloatCurve.UpdateOrAddKey(0.f, rightBounding);

		TimelineOpenCloseDoorCallback.BindDynamic(this, &AMovableActorBase::OpenCloseDoor);
		TimelineDoorComp->SetPlayRate(1 / openCloseDoorSpeed); // maybe this line become useless
		TimelineDoorComp->AddInterpFloat(DoorCurve, TimelineOpenCloseDoorCallback);
	}

	if (MovingSfx != nullptr)
	{
		SfxComponent->SetSound(MovingSfx);
	}
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

// improve this function
void AMovableActorBase::OnMovableTriggered()
{
	//int32 destinyIdx = nullptr    // MAKE THIS call based on a destiny
	if (bIsDoorOpen) // I have this if/else to play the movement after the door being closed ...
	{
		if (CloseDoorSfx != nullptr)
		{
			PlayOpenCloseDoorSfx(CloseDoorSfx);
		}
		TimelineDoorComp->PlayFromStart();
		bIsDoorOpen = false;

		FTimerHandle TimerHandle;
		float delayTime = CloseDoorSfx != nullptr ? CloseDoorSfx->GetDuration() : openCloseDoorSpeed;
		if (bIsMovingForward)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMovableActorBase::OnMoveNextTriggered, delayTime);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMovableActorBase::OnMovePreviousTriggered, delayTime);
		}

	}
	else
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
		NextLocation = Spline->GetSplinePointAt(currentPointIdx+1, ESplineCoordinateSpace::World);
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
		NextLocation = Spline->GetSplinePointAt(currentPointIdx-1, ESplineCoordinateSpace::World);
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

void AMovableActorBase::OnEntranceEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->ActorHasTag(FName("PCharacter")))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("AMovableActorBase::OnEntranceEndOverlap"));
			float dotProduct = OtherActor->GetActorForwardVector() | EntranceDirectionArrow->GetForwardVector();
			if (dotProduct > 0)
			{
				if (CloseDoorSfx != nullptr)
				{
					PlayOpenCloseDoorSfx(CloseDoorSfx);
				}
				TimelineDoorComp->PlayFromStart();
				bIsDoorOpen = false;
			}
		}
	}
}

void AMovableActorBase::OpenDoor()
{
	if (SfxComponent->GetSound() != nullptr)
	{
		SfxComponent->Stop();
	}

	if (OpenDoorSfx != nullptr)
	{
		PlayOpenCloseDoorSfx(OpenDoorSfx);
	}
	TimelineDoorComp->Reverse();
	bIsDoorOpen = true;
}

void AMovableActorBase::OpenCloseDoor(float Alpha)
{
	FVector rLocation = RightDoorMesh->GetRelativeLocation();
	FVector lLocation = LeftDoorMesh->GetRelativeLocation();

	FVector rNewLocation = FVector(Alpha, rLocation.Y, rLocation.Z);
	FVector lNewLocation = FVector(-Alpha, lLocation.Y, lLocation.Z);

	RightDoorMesh->SetRelativeLocation(rNewLocation);
	LeftDoorMesh->SetRelativeLocation(lNewLocation);

}

void AMovableActorBase::PlayOpenCloseDoorSfx(USoundBase* Sfx)
{
	if (Sfx != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("AMovableActorBase::PlayOpenCloseDoorSfx SoundName: %s , Duration: %f"), *Sfx->GetName(), Sfx->GetDuration());
		TimelineDoorComp->SetPlayRate(1 / Sfx->GetDuration()); // Forces the Open/Close movement follow the same SoundRate
		UGameplayStatics::PlaySound2D(this, Sfx);
	}
}