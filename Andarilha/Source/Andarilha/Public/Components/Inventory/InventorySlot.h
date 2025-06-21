#pragma once

#include "Collectables/ItemStruct.h"
#include "CoreMinimal.h"
#include "InventorySlot.generated.h"

UCLASS(BlueprintType)
class ANDARILHA_API UInventorySlot : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Item"))
    FItemStruct Item;

    UInventorySlot();
};