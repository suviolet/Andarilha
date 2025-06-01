#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PinPadWidget.generated.h"

UCLASS()
class ANDARILHA_API UPinPadWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPin(FString pin);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pin")
	FString Pin;
	
};
