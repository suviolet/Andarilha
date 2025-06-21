#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/InteractableBase.h"
#include "ItemStruct.h"
#include "CollectableBase.generated.h"

class UDataTable;

UCLASS()
class ANDARILHA_API ACollectableBase : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	ACollectableBase();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual bool Interact(FItemStruct item = {}) override;

	//TDelegate<void()> OnCollectDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items Data Table")
	UDataTable* ItemsDataTable;

	UFUNCTION(BlueprintCallable)
	void SetItem();

protected:
	UFUNCTION()
	virtual void OnCollect();
};
