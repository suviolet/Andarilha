#include "Triggerables/FloorButton.h"

AFloorButton::AFloorButton()
{
	StaticMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.25f));

	Box->InitBoxExtent(FVector(90.f, 90.f, 360.f));
	Box->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	Box->OnComponentBeginOverlap.AddDynamic(this, &AFloorButton::TurnOnTrigger);
	Box->OnComponentEndOverlap.AddDynamic(this, &AFloorButton::TurnOffTrigger);

	BoxTriggerSignal->InitBoxExtent(FVector(90.f, 90.f, 360.f));
	BoxTriggerSignal->SetRelativeLocation(FVector(0.f, 0.f, 360.f));
}

void AFloorButton::BeginPlay()
{
	Super::BeginPlay();

}

void AFloorButton::TurnOnTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//if (APCharacter* PCharacter = Cast<APCharacter>(OtherActor))
		if (OtherActor->ActorHasTag(FName("PCharacter")))
		{
			OnTrigger();
		}
	}
}

void AFloorButton::TurnOffTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//if (APCharacter* PCharacter = Cast<APCharacter>(OtherActor))
		if (OtherActor->ActorHasTag(FName("PCharacter")))
		{
			OnTrigger();
		}
	}
}

void AFloorButton::OnTrigger()
{
	if (!bHasBeenTriggered)
	{
		OnTriggerDelegate.ExecuteIfBound();
		bHasBeenTriggered = true;
	}
}