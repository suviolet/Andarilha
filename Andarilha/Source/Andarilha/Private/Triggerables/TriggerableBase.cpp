#include "Triggerables/TriggerableBase.h"

ATriggerableBase::ATriggerableBase()
{
	bHasBeenTriggered = false;

	Tags.Add(FName("TriggerableBase"));
}

void ATriggerableBase::BeginPlay()
{
	Super::BeginPlay();

	// ... improve it! gambiarra
	if (DoorTriggerableReference != nullptr)
	{
		OnTriggerDelegate.BindUFunction(DoorTriggerableReference, FName(TEXT("OnDoorTriggered"))); // Change to BindUOBject
	}
	if (MovableTriggerableReference != nullptr)
	{
		OnTriggerDelegate.BindUFunction(MovableTriggerableReference, FName(TEXT("OnMovableTriggered"))); // Change to BindUOBject
	}
}

void ATriggerableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATriggerableBase::Interact()
{
	OnTrigger();
}

void ATriggerableBase::OnTrigger()
{
	OnTriggerDelegate.Execute();
	bHasBeenTriggered = true;
}