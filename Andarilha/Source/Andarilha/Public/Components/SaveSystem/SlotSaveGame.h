#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SlotSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FTransform ActorTransform;

	UPROPERTY()
	TArray<FName> Levels;

	//UPROPERTY()
	//TArray<uint8> InventoryByteData;
};


UCLASS()
class ANDARILHA_API USlotSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FActorSaveData Data;
};
