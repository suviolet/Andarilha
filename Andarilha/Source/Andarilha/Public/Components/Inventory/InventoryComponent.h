#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "../../Collectables/ItemStruct.h"
#include "InventorySlot.h"
#include "InventoryComponent.generated.h"

class UInventoryWidget;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANDARILHA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	TMulticastDelegate<void(TArray<FItemStruct>, int32)> OnInventoryChangeEvent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "InventoryWidgetClass"))
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "MaxSlotSize")) // SaveGame property
	int32 MaxSlotSize;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool AddItem(FItemStruct Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool RemoveItemAtIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void Clear();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual int32 GetLength();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual int32 GetMaxSize();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual UInventorySlot* GetSlot(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual TArray<UInventorySlot*> GetSlots();

	virtual TArray<FItemStruct> GetItems();

	void MoveUpUI(const FInputActionValue& Value);

	void MoveDownUI(const FInputActionValue& Value);

	void DropUI(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DisplayWidget(bool bCanDisplay);

	UInventoryWidget* widget;

	TArray<FItemStruct> Items;

	int32 currentSlotIndex;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "InventorySlots"))  // SaveGame property
	TArray<UInventorySlot*> InventorySlots;

};
