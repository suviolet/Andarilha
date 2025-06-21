#pragma once

#include "CoreMinimal.h"
#include "Collectables/ItemStruct.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

class UImage;
class UButton;
class UTextBlock;

UCLASS()
class ANDARILHA_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativePreConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Item"))
	FItemStruct Item;
};
