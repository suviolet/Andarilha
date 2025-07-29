#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "MovableActors/SplineBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "MovableActorBase.generated.h"

class UBoxComponent;

UCLASS()
class ANDARILHA_API AMovableActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovableActorBase();

	UFUNCTION()
	void MoveToPoint(float Alpha);

	UFUNCTION()
	void OpenDoor();

	UFUNCTION()
	void OpenCloseDoor(float Alpha);

	UFUNCTION()
	void OnMovableTriggered();

	UFUNCTION()
	void OnMoveNextTriggered();

	UFUNCTION()
	void OnMovePreviousTriggered();

	UFUNCTION()
	void OnEntranceEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void PlayOpenCloseDoorSfx(USoundBase* Sfx);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* MovingDirectionArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* EntranceDirectionArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* LeftDoorMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* RightDoorMesh;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* OpenDoorSfx;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* CloseDoorSfx;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* MovingSfx;

	UPROPERTY(EditAnywhere, Category = "Sound")
	UAudioComponent* SfxComponent;

	UPROPERTY(EditAnywhere, Category = "Sound")
	float startTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EntranceDoorBox")
	UBoxComponent* EntranceDoorBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	ASplineBase* SplineActor;

	USplineComponent* Spline;

	UPROPERTY(EditAnywhere)
	UCurveFloat* MovementCurve;

	UPROPERTY(EditAnywhere)
	UCurveFloat* DoorCurve;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float movementSpeed;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float openCloseDoorSpeed;

	int32 currentPointIdx;

	int32 numberOfSplinePoints;

	bool bIsMovingForward;

	bool bIsDoorOpen;

protected:
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timeline")
	UTimelineComponent* TimelineMovementComp;

	FOnTimelineFloat TimelineMovingCallback;

	FOnTimelineEventStatic TimelineMovingFinishedCallback;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timeline")
	UTimelineComponent* TimelineDoorComp;

	FOnTimelineFloat TimelineOpenCloseDoorCallback;

	FSplinePoint CurrenctLocation;
	FSplinePoint NextLocation;
};
