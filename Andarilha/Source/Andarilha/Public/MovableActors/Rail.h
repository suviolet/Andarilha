#pragma once

#include "CoreMinimal.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MovableActors/SplineBase.h"
#include "Rail.generated.h"

UCLASS()
class ANDARILHA_API ARail : public ASplineBase
{
	GENERATED_BODY()
	
public:	
	ARail();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMesh* Mesh;

protected:
	virtual void BeginPlay() override;
};
