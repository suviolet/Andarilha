

#include "Doors/DoorBase.h"

ADoorBase::ADoorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetCollisionProfileName(FName("BlockAll"), false); //BlockAllDynamic
	DoorMesh->SetSimulatePhysics(false);//true
	DoorMesh->SetupAttachment(RootComponent);

	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
	FrameMesh->SetCollisionProfileName(FName("BlockAll"), false); //BlockAllDynamic
	FrameMesh->SetSimulatePhysics(false);
	FrameMesh->SetMobility(EComponentMobility::Static);
	FrameMesh->SetupAttachment(RootComponent);

	DoorCurve = CreateDefaultSubobject<UCurveFloat>(TEXT("DoorCurve"));
	ChangeDirection(GET_MEMBER_NAME_CHECKED(ADoorBase, Direction));

	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
	
	bIsOpen = false;
	DoorClosesAfterTime = 0.f;
	//bIsReady = true;
}

void ADoorBase::BeginPlay()
{
	Super::BeginPlay();

	if (DoorCurve)
	{
		TimelineCallback.BindDynamic(this, &ADoorBase::Rotate);
		//TimelineFinishedCallback.BindUFunction(this, FName(TEXT("SetState")));

		TimelineComp->AddInterpFloat(DoorCurve, TimelineCallback);
		//TimelineComp->SetTimelineFinishedFunc(TimelineFinishedCallback);
	}

}


void ADoorBase::ChangeDirection(FName PropertyName)
{
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ADoorBase, Direction))
	{
		if (Direction == EOpeningDirection::ToLeft)
		{
			DirectionStruct = FDirectionStruct{
				EOpeningDirection::ToLeft,
				FVector(0.f, 0.f, 0.f),
				FRotator(0.f, 90.f, 0.f),
				FVector(0.f, -10.f, 0.f),
				FRotator(0.f, 90.f, 0.f)
			};
		}
		else if (Direction == EOpeningDirection::ToRight)
		{
			DirectionStruct = FDirectionStruct{
				EOpeningDirection::ToRight,
				FVector(0.f, 0.f, 0.f),
				FRotator(0.f, -90.f, 0.f),
				FVector(0.f, 10.f, 0.f),
				FRotator(0.f, -90.f, 0.f)
			};
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("ELSE"));
		}

		DoorMesh->SetRelativeRotation(DirectionStruct.DoorRotation);
		DoorMesh->SetRelativeLocation(DirectionStruct.DoorLocation);
		FrameMesh->SetRelativeRotation(DirectionStruct.FrameRotation);
		FrameMesh->SetRelativeLocation(DirectionStruct.FrameLocation);
	}
}

void ADoorBase::ChangeCurve()
{
	DoorCurve->FloatCurve.Reset();
	DoorCurve->FloatCurve.UpdateOrAddKey(0.f, DirectionStruct.DoorRotation.Yaw);
	DoorCurve->FloatCurve.UpdateOrAddKey(OpeningTime, 0.f);
}

#if WITH_EDITOR
void ADoorBase::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);
	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ADoorBase, Direction))
	{
		ChangeDirection(PropertyName);
		ChangeCurve();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(ADoorBase, OpeningTime))
	{
		ChangeCurve();
	}
}
#endif

void ADoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoorBase::Open()
{
	bIsOpen = true;
	if (TimelineComp != NULL)
	{
		TimelineComp->Play();
	}
	if (DoorClosesAfterTime > 0 && DoorClosesAfterTime > OpeningTime)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ADoorBase::Close, DoorClosesAfterTime);
	}
}

void ADoorBase::Close()
{
	bIsOpen = false;
	if (TimelineComp != NULL)
	{
		TimelineComp->Reverse();
	}
}

void ADoorBase::Rotate(float Output)
{
	FRotator NewRotation = FRotator(0.0f, Output, 0.f);
	DoorMesh->SetRelativeRotation(NewRotation);
}

void ADoorBase::OnDoorTriggered()
{
	//if (bIsReady)
	//{
	//	bIsReady = false;
		if (!bIsOpen)
		{
			Open();
		}
		else
		{
			Close();
		}
	//}
}

void ADoorBase::SetState()
{
	//bIsReady = true;
}