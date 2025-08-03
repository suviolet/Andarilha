#include "MovableActors/EnclosedMovableActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

AEnclosedMovableActor::AEnclosedMovableActor()
{
	PrimaryActorTick.bCanEverTick = false;

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
	EntranceDoorBox->OnComponentEndOverlap.AddDynamic(this, &AEnclosedMovableActor::OnEntranceEndOverlap);

	EntranceDirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("EntranceDirectionArrow"));
	EntranceDirectionArrow->SetupAttachment(EntranceDoorBox);

	DoorCurve = CreateDefaultSubobject<UCurveFloat>(TEXT("DoorCurve"));

	openCloseDoorSpeed = 1.5f;

	TimelineDoorComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineDoorComp"));

	bIsDoorOpen = true;
}

void AEnclosedMovableActor::BeginPlay()
{
	Super::BeginPlay();

	TimelineMovingFinishedCallback.BindUFunction(this, "OpenDoor");
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

		TimelineOpenCloseDoorCallback.BindDynamic(this, &AEnclosedMovableActor::OpenCloseDoor);
		TimelineDoorComp->SetPlayRate(1 / openCloseDoorSpeed);
		TimelineDoorComp->AddInterpFloat(DoorCurve, TimelineOpenCloseDoorCallback);
	}
}

// improve this function
void AEnclosedMovableActor::OnMovableTriggered()
{
	//int32 destinyIdx = nullptr    // MAKE THIS call based on a destiny
	if (bIsTriggerable && bIsDoorOpen) // I have this if/else to play the movement after the door being closed ...
	{
		float delayTime = openCloseDoorSpeed;
		if (CloseDoorSfx != nullptr)
		{
			PlayOpenCloseDoorSfx(CloseDoorSfx);
			delayTime = CloseDoorSfx->GetDuration();
			TimelineDoorComp->SetPlayRate(1 / delayTime);
		}
		TimelineDoorComp->PlayFromStart();
		bIsDoorOpen = false;

		FTimerHandle TimerHandle;
		if (bIsMovingForward)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnclosedMovableActor::OnMoveNextTriggered, delayTime);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnclosedMovableActor::OnMovePreviousTriggered, delayTime);
		}

	}
	else
	{
		Super::OnMovableTriggered();
	}
}

void AEnclosedMovableActor::OpenDoor()
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

void AEnclosedMovableActor::OpenCloseDoor(float Alpha)
{
	FVector rLocation = RightDoorMesh->GetRelativeLocation();
	FVector lLocation = LeftDoorMesh->GetRelativeLocation();

	FVector rNewLocation = FVector(Alpha, rLocation.Y, rLocation.Z);
	FVector lNewLocation = FVector(-Alpha, lLocation.Y, lLocation.Z);

	RightDoorMesh->SetRelativeLocation(rNewLocation);
	LeftDoorMesh->SetRelativeLocation(lNewLocation);

}


void AEnclosedMovableActor::OnEntranceEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

void AEnclosedMovableActor::PlayOpenCloseDoorSfx(USoundBase* Sfx)
{
	if (Sfx != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("AMovableActorBase::PlayOpenCloseDoorSfx SoundName: %s , Duration: %f"), *Sfx->GetName(), Sfx->GetDuration());
		TimelineDoorComp->SetPlayRate(1 / Sfx->GetDuration()); // Forces the Open/Close movement follow the same SoundRate
		UGameplayStatics::PlaySound2D(this, Sfx);
	}
}