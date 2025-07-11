#pragma once

#include "CoreMinimal.h"
#include "Triggerables/TriggerableBase.h"
#include "FloorButton.generated.h"

UCLASS()
class ANDARILHA_API AFloorButton : public ATriggerableBase
{
	GENERATED_BODY()

public:
	AFloorButton();

	UFUNCTION()
	void TurnOnTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void TurnOffTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

	virtual void OnTrigger() override;
};
