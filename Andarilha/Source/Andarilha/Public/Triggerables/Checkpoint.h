#pragma once

#include "CoreMinimal.h"
#include "Triggerables/TriggerableBase.h"
#include "Checkpoint.generated.h"

UCLASS()
class ANDARILHA_API ACheckpoint : public ATriggerableBase
{
	GENERATED_BODY()

public:
	ACheckpoint();

	UFUNCTION()
	void SaveGame(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "SaveWidgetClass"))
	TSubclassOf<UUserWidget> SaveWidgetClass;

	UUserWidget* savingWidget;

protected:
	virtual void BeginPlay() override;

	void AutoDestroy();
};
