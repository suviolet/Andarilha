#pragma once

#include "CoreMinimal.h"
#include "EOpeningDirection.generated.h"

UENUM(BlueprintType)
enum class EOpeningDirection : uint8
{
	ToLeft UMETA(DisplayName = "Left"),
	ToRight UMETA(DisplayName = "Right")
};