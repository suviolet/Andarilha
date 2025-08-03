#pragma once

#include "CoreMinimal.h"
#include "MovableActors/MovableActorBase.h"
#include "EnclosedMovableActor.generated.h"

UCLASS()
class ANDARILHA_API AEnclosedMovableActor : public AMovableActorBase
{
	GENERATED_BODY()
	

public:
	AEnclosedMovableActor();

	virtual void OnMovableTriggered() override;

	UFUNCTION()
	void OpenDoor();

	UFUNCTION()
	void OpenCloseDoor(float Alpha);

	UFUNCTION()
	void OnEntranceEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void PlayOpenCloseDoorSfx(USoundBase* Sfx);

	virtual void MoveToPoint(float Alpha) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* EntranceDirectionArrow;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* LeftDoorMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* RightDoorMesh;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* OpenDoorSfx;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* CloseDoorSfx;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EntranceDoorBox")
	UBoxComponent* EntranceDoorBox;

	UPROPERTY(EditAnywhere)
	UCurveFloat* DoorCurve;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float openCloseDoorSpeed;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	bool bCanRotate;

	bool bIsDoorOpen;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timeline")
	UTimelineComponent* TimelineDoorComp;

	FOnTimelineFloat TimelineOpenCloseDoorCallback;

};
