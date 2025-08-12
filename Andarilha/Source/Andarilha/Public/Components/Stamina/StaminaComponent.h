#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

class APCharacter;
class UStaminaWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANDARILHA_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStaminaComponent();

	AActor* OwnerActor;

	APCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "StaminaWidgetClass"))
	TSubclassOf<UStaminaWidget> StaminaWidgetClass;

	UStaminaWidget* widget;

	UPROPERTY(BlueprintReadOnly)
	float stamina;

	UPROPERTY(BlueprintReadOnly)
	float maxStamina;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float amountToDecrease;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float amountToRecover;

	UFUNCTION()
	void DecreaseStamina();

	UFUNCTION()
	void RecoverStamina();

	UFUNCTION()
	void DisplayWidget(bool bCanDisplay);

protected:
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle;

		
};
