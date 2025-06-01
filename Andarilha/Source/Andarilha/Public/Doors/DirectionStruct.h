#pragma once

#include "CoreMinimal.h"
#include "EOpeningDirection.h"
#include "DirectionStruct.generated.h"

USTRUCT(BlueprintType)
struct FDirectionStruct
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DStruct")
    EOpeningDirection Option;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DStruct")
    FVector DoorLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DStruct")
    FRotator DoorRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DStruct")
    FVector FrameLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DStruct")
    FRotator FrameRotation;

};
