#include "Collectables/CollectableBase.h"
#include "Collectables/ItemStruct.h"
#include "Engine/DataTable.h"

ACollectableBase::ACollectableBase()
{
	index = 0;
	Tags.Add(FName("CollectableBase"));
}

void ACollectableBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetItem();
}


void ACollectableBase::SetItem()
{
	if (ItemsDataTable != nullptr)
	{
		TArray<FName> Rows = ItemsDataTable->GetRowNames(); // would like to get it by dropdown instead

		index = FMath::Clamp(index, 0, Rows.Num() - 1);
		static const FString ContextString(TEXT("Finding Row in Items Data Table")); // what's the reason of ContextString ?

		FItemStruct* Item = ItemsDataTable->FindRow<FItemStruct>(Rows[index], ContextString);

		if (Item != nullptr)
		{
			StaticMesh->SetStaticMesh(Item->Mesh);
		}
	}
}

bool ACollectableBase::Interact(FItemStruct item)
{
	OnCollect();
	return true;
}


void ACollectableBase::OnCollect()
{
	this->Destroy();
}