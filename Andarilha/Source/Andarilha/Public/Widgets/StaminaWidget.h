#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StaminaWidget.generated.h"

class UProgressBar;

UCLASS()
class ANDARILHA_API UStaminaWidget : public UUserWidget
{
	GENERATED_BODY()

public: 

	void UpdateStaminaBar(float value);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;
};
