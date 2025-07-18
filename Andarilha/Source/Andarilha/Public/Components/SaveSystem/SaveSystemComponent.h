#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "SaveSystemComponent.generated.h"

class USlotSaveGame;
class APCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ANDARILHA_API USaveSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USaveSystemComponent();

	void Save(const FInputActionValue& Value);

	void Load(const FInputActionValue& Value);

	void CleanSlot();

	APCharacter* PlayerCharacter;

	UPROPERTY()
	FString SaveSlotName;

	UPROPERTY()
	TObjectPtr<USlotSaveGame> CurrentSaveGame;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName CurrentLevel;

protected:
	virtual void BeginPlay() override;

};
