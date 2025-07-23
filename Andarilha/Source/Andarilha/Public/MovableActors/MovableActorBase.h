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
	void OpenDoor(float Alpha);

	UFUNCTION()
	void CloseDoor(float Alpha);

	UFUNCTION()
	void OnMovableTriggered();

	UFUNCTION()
	void OnMoveNextTriggered();

	UFUNCTION()
	void OnMovePreviousTriggered();

	UFUNCTION()
	void OnEntranceEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* MovingDirectionArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* EntranceDirectionArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "LeftDoorMesh")
	UStaticMeshComponent* LeftDoorMesh;

	UPROPERTY(EditAnywhere, Category = "RightDoorMesh")
	UStaticMeshComponent* RightDoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EntranceDoorBox")
	UBoxComponent* EntranceDoorBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	ASplineBase* SplineActor;

	USplineComponent* Spline;

	UPROPERTY(EditAnywhere)
	UCurveFloat* Curve;

	UPROPERTY(EditAnywhere, Category = "SpeedMovement")
	float speedMovement;

	int32 currentPointIdx;

	int32 numberOfSplinePoints;

	bool bIsMovingForward;

protected:
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TimelineMovementComp")
	UTimelineComponent* TimelineMovementComp;

	FOnTimelineFloat TimelineMovingCallback;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TimelineDoorComp")
	UTimelineComponent* TimelineDoorComp;

	FOnTimelineFloat TimelineOpenCloseDoorCallback;

	FSplinePoint CurrenctLocation;
	FSplinePoint NextLocation;
};
