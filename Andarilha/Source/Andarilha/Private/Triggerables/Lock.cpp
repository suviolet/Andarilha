#include "Triggerables/Lock.h"

ALock::ALock()
{
	index = 0;
	Tags.Add(FName("Lock"));
}

void ALock::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetItem();
}

void ALock::SetItem()
{
	if (ItemsDataTable != nullptr)
	{
		TArray<FName> Rows = ItemsDataTable->GetRowNames(); // would like to get it by dropdown instead

		index = FMath::Clamp(index, 0, Rows.Num() - 1);
		static const FString ContextString(TEXT("Finding Row in Items Data Table")); // what's the reason of ContextString ?

		FItemStruct* RowItem = ItemsDataTable->FindRow<FItemStruct>(Rows[index], ContextString);

		if (RowItem != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("EEELock::SetItem RowItem ItemName: %s "), *FString(RowItem->Name));
			Item = *RowItem;
		}
	}
}

bool ALock::Interact(FItemStruct item)
{
	// Would like to validate Item is not null before...
	if (!bHasBeenTriggered && Item.Name == item.Name)
	{
		OnTrigger();
		return true;
	}
	return false;
}

void ALock::OnTrigger()
{
	UE_LOG(LogTemp, Warning, TEXT("ALock::OnTrigger"));
	OnTriggerDelegate.Execute();
	bHasBeenTriggered = true;
}