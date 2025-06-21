#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UItemSlotWidget;
class UWrapBox;
struct FItemStruct;

UCLASS()
class ANDARILHA_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativePreConstruct() override;

	void LoadInventory(TArray<FItemStruct> Items, int32 selectedIdx = -1);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "ItemSlotWidgetClass"))
	TSubclassOf<UItemSlotWidget> ItemSlotWidgetClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> WrapBox;

	FLinearColor selectedColor;
	FLinearColor unselectedColor;

};
