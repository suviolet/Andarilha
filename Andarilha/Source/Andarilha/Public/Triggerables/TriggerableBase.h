#pragma once

#include "CoreMinimal.h"
#include "Doors/DoorBase.h"
#include "MovableActors/MovableActorBase.h"
#include "Interactables/InteractableBase.h"
#include "TriggerableBase.generated.h"

UCLASS()
class ANDARILHA_API ATriggerableBase : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	ATriggerableBase();

	virtual void Tick(float DeltaTime) override;

	virtual void Interact() override;

	TDelegate<void()> OnTriggerDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorTriggerableReference")
	ADoorBase* DoorTriggerableReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovableTriggerableReference")
	AMovableActorBase* MovableTriggerableReference;

	bool bHasBeenTriggered;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnTrigger();
};
