#pragma once

#include "CoreMinimal.h"
#include "Triggerables/TriggerableBase.h"
#include "WallButton.generated.h"

UCLASS()
class ANDARILHA_API AWallButton : public ATriggerableBase
{
	GENERATED_BODY()

public:
	AWallButton();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UCurveFloat* ButtonCurve;

protected:
	virtual void BeginPlay() override;

	virtual void OnTrigger() override;

	UFUNCTION()
	void Press(float Output);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TimelineComp")
	UTimelineComponent* TimelineComp;

	FOnTimelineFloat TimelineCallback;
};
