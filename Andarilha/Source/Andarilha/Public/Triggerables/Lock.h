#pragma once

#include "CoreMinimal.h"
#include "Triggerables/TriggerableBase.h"
#include "Lock.generated.h"

struct FItemStruct;

UCLASS()
class ANDARILHA_API ALock : public ATriggerableBase
{
	GENERATED_BODY()

public:
	ALock();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual bool Interact(FItemStruct item = {}) override;

	UFUNCTION(BlueprintCallable)
	void SetItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items Data Table")
	UDataTable* ItemsDataTable;

	UPROPERTY()
	FItemStruct Item;


protected:
	virtual void OnTrigger() override;

};
