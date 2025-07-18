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

	virtual bool Interact(FItemStruct item = {}) override;

	TDelegate<void()> OnTriggerDelegate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DoorTriggerableReference")
	ADoorBase* DoorTriggerableReference;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MovableTriggerableReference")
	AMovableActorBase* MovableTriggerableReference;

	bool bHasBeenTriggered;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnTrigger();
};
