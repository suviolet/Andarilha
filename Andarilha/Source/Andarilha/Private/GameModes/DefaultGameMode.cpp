#include "GameModes/DefaultGameMode.h"


void ADefaultGameMode::StartPlay()
{
	Super::StartPlay();

	check(GEngine != nullptr);
}
