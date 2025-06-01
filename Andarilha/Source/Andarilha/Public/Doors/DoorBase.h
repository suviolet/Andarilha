#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "DirectionStruct.h"
#include "EOpeningDirection.h"
#include "GameFramework/Actor.h"
#include "DoorBase.generated.h"

UCLASS()
class ANDARILHA_API ADoorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ADoorBase();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnDoorTriggered();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* DefaultSceneRoot;

	UArrowComponent* Arrow;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* FrameMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
	EOpeningDirection Direction;

	FDirectionStruct DirectionStruct;

	UPROPERTY(EditAnywhere)
	UCurveFloat* DoorCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OpeningTime")
	float OpeningTime;

	bool bIsOpen;

protected:
	virtual void BeginPlay() override;

	void ChangeDirection(FName PropertyName);

	void ChangeCurve();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void Open();

	void Close();

	UFUNCTION()
	void Rotate(float Output);

	UFUNCTION()
	void SetState();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TimelineComp")
	UTimelineComponent* TimelineComp;

	FOnTimelineFloat TimelineCallback;

	FOnTimelineEventStatic TimelineFinishedCallback;

};