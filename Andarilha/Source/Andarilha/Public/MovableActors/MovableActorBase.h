#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "MovableActors/SplineBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "MovableActorBase.generated.h"

UCLASS()
class ANDARILHA_API AMovableActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovableActorBase();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void MoveToPoint(float Alpha);

	UFUNCTION()
	void OnMovableTriggered();

	UFUNCTION()
	void OnMoveNextTriggered();

	UFUNCTION()
	void OnMovePreviousTriggered();

	UArrowComponent* Arrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* Mesh;

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

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TimelineComp")
	UTimelineComponent* TimelineComp;

	FOnTimelineFloat TimelineCallback;

	FSplinePoint CurrenctLocation;
	FSplinePoint NextLocation;
};
