#include "Components/Inventory/InventoryComponent.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/ItemSlotWidget.h"
#include "Widgets/InventoryWidget.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
    InventorySlots.Reserve(GetMaxSize());
    currentSlotIndex = -1;
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
    if (InventoryWidgetClass != nullptr)
    {
        widget = CreateWidget<UInventoryWidget>(UGameplayStatics::GetPlayerController(this, 0), InventoryWidgetClass);
        //widget->AddToViewport(); // move to another logic, it is appearing on Main Menu, Pause Menu, etc..  !!!!!
    }
    OnInventoryChangeEvent.AddUObject(widget, &UInventoryWidget::LoadInventory);
}

bool UInventoryComponent::AddItem(FItemStruct Item)
{
    if (GetLength() < GetMaxSize()) {
        UInventorySlot* slot = NewObject<UInventorySlot>();
        slot->Item = Item;
        InventorySlots.Add(slot);
        OnInventoryChangeEvent.Broadcast(GetItems(), -1);
        return true;
    }
    return false;
}

bool UInventoryComponent::RemoveItemAtIndex(int32 Index)
{
    if (InventorySlots.IsValidIndex(Index))
    {
        InventorySlots.RemoveAt(Index);
        OnInventoryChangeEvent.Broadcast(GetItems(), -1);
        return true;
    }
    return false;
}

int32 UInventoryComponent::GetLength()
{
    return InventorySlots.Num();
}

int32 UInventoryComponent::GetMaxSize()
{
    return this->MaxSlotSize;
}

void UInventoryComponent::Clear()
{
    InventorySlots.Empty();
    OnInventoryChangeEvent.Broadcast(GetItems(), -1);
}

UInventorySlot* UInventoryComponent::GetSlot(int32 Index)
{
    if (InventorySlots.IsValidIndex(Index))
    {
        return InventorySlots[Index];
    }
    return nullptr;
}

TArray<UInventorySlot*> UInventoryComponent::GetSlots()
{
    return InventorySlots;
}

TArray<FItemStruct> UInventoryComponent::GetItems()
{
    TArray<UInventorySlot*> Slots = GetSlots();
    Items.Empty();
    for (int32 i = 0; i < Slots.Num(); i++)
    {
        Items.Add(Slots[i]->Item);
    }
    return Items;
}

void UInventoryComponent::MoveUpUI(const FInputActionValue& Value)
{
    int32 count = GetLength();
    int32 lastIdx = count - 1;
    if (count > 0)
    {
        if (currentSlotIndex <= 0)
        {
            currentSlotIndex = lastIdx;
        }
        else
        {
            currentSlotIndex--;
        }

        widget->LoadInventory(GetItems(), currentSlotIndex);
    }
    else
    {
        // ends idx because slots is empty
        currentSlotIndex = -1;
    }
}

void UInventoryComponent::MoveDownUI(const FInputActionValue& Value)
{
    int32 count = GetLength();
    if (count > 0)
    {
        if (currentSlotIndex == -1 || currentSlotIndex == count-1)
        {
            currentSlotIndex = 0;
        }
        else
        {
            currentSlotIndex++;
        }

        widget->LoadInventory(GetItems(), currentSlotIndex);
    }
    else
    {
        // ends idx because slots is empty
        currentSlotIndex = -1;
    }
}

void UInventoryComponent::DropUI(const FInputActionValue& Value)
{
    if (currentSlotIndex > -1)
    {
        RemoveItemAtIndex(currentSlotIndex);
        currentSlotIndex = -1;
    }

    // Spawn on World dropped item
}

void UInventoryComponent::DisplayWidget(bool bCanDisplay)
{
    if (bCanDisplay)
    {
        widget->AddToViewport();
    }
    else
    {
        widget->RemoveFromViewport();
    }
}
