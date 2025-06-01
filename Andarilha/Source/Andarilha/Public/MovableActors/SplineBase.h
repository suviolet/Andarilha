#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "SplineBase.generated.h"

UCLASS()
class ANDARILHA_API ASplineBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASplineBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Spline")
	USplineComponent* Spline;

protected:
	virtual void BeginPlay() override;

};
