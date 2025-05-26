#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DefaultGameMode.generated.h"

UCLASS()
class ANDARILHA_API ADefaultGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void StartPlay() override;
};
