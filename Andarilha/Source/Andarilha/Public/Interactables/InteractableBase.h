#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Collectables/ItemStruct.h"
#include "Components/WidgetComponent.h"
#include "InteractableBase.generated.h"

UCLASS()
class ANDARILHA_API AInteractableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractableBase();

	UFUNCTION()
	virtual bool Interact(FItemStruct item = {});

	UFUNCTION()
	void DisplaySignalWidget(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void RemoveSignalWidget(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Box")
	UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BoxTriggerSignal")
	UBoxComponent* BoxTriggerSignal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Index")
	int32 index;

protected:
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* TriggerSignalWidget;
};
