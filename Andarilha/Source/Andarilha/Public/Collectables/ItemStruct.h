#pragma once

#include "CoreMinimal.h"
#include "ItemStruct.generated.h"

USTRUCT(BlueprintType)
struct ANDARILHA_API FItemStruct : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Name"), Category = "DStruct")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = "true", DisplayName = "Description"), Category = "DStruct")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Image"), Category = "DStruct")
    UTexture2D* Image;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Mesh"), Category = "DStruct")
    UStaticMesh* Mesh;
};

