#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "MovableActors/SplineBase.h"
#include "MovableActorBase.generated.h"

class UBoxComponent;

UCLASS()
class ANDARILHA_API AMovableActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovableActorBase();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void MoveToPoint(float Alpha);

	UFUNCTION()
	virtual void OnMovableTriggered();

	UFUNCTION()
	void OnMoveNextTriggered();

	UFUNCTION()
	void OnMovePreviousTriggered();

	UFUNCTION()
	void UnsetMovementBool();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* MovingDirectionArrow;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Sound")
	UAudioComponent* SfxComponent;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* MovingSfx;

	UPROPERTY(EditAnywhere, Category = "Sound")
	float startTime;

	USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	ASplineBase* SplineActor;

	UPROPERTY(EditAnywhere)
	UCurveFloat* MovementCurve;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float movementSpeed;

	int32 currentPointIdx;

	int32 numberOfSplinePoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IsTriggerable")
	bool bIsTriggerable;

	bool bIsMovingForward;

	bool bIsMoving;

protected:
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timeline")
	UTimelineComponent* TimelineMovementComp;

	FOnTimelineFloat TimelineMovingCallback;
	FOnTimelineEventStatic TimelineMovingFinishedCallback;

	FSplinePoint CurrenctLocation;
	FSplinePoint NextLocation;
};
