#pragma once

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Triggerables/TriggerableBase.h"
#include "Widgets/PinPadWidget.h"
#include "PinPad.generated.h"

UCLASS()
class ANDARILHA_API APinPad : public ATriggerableBase
{
	GENERATED_BODY()
	

public:
	APinPad();

	virtual void Tick(float DeltaTime) override;

	class ACharacter* Player;

	class APlayerController* Controller;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PinPad")
	TSubclassOf<class UPinPadWidget> PinPadWidgetClass;

	UPROPERTY()
	TObjectPtr<UPinPadWidget> PinPadWidget;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* PinPadWidgetOnWorld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pin")
	FString Pin;

	bool bIsDialing;

protected:
	virtual void BeginPlay() override;

	virtual void OnTrigger() override;

	void ResumeGame();

	void OnCorrectedPin();
};
