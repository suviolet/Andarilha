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

	UFUNCTION(BlueprintCallable)
	bool FirstSave();

	UFUNCTION(BlueprintCallable)
	bool Save();

	UFUNCTION(BlueprintCallable)
	bool Load();

	UFUNCTION(BlueprintCallable)
	void CleanSlot();

	APCharacter* PlayerCharacter;

	UPROPERTY()
	FString SaveSlotName;

	UPROPERTY()
	TObjectPtr<USlotSaveGame> CurrentSaveGame;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName CurrentLevel;

	AActor* OwnerActor;

	UWorld* currentWorld;

protected:
	virtual void BeginPlay() override;

	int UUID;
};
