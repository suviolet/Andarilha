#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

class UUserWidget;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANDARILHA_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStaminaComponent();

	AActor* OwnerActor;

	APCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "StaminaWidgetClass"))
	TSubclassOf<UUserWidget> StaminaWidgetClass;

	UUserWidget* widget;

	float value;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float amountToDecrease;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float amountToRecover;

	UFUNCTION()
	void DecreaseStamina();

	UFUNCTION()
	void RecoverStamina();

protected:
	virtual void BeginPlay() override;

		
};
