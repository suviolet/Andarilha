
#include "MovableActors/Rail.h"

ARail::ARail()
{
	//sectionLength = 100.f; // make this editable
}

void ARail::BeginPlay()
{
	Super::BeginPlay();
}

void ARail::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	float meshLength = FVector(Mesh->GetBoundingBox().Min.GetAbs() + Mesh->GetBoundingBox().Max.GetAbs()).Y;
	float meshQty = trunc(Spline->GetSplineLength() / meshLength);

	//float myInt = 0.0f;
	//modf(Spline->GetSplineLength() / sectionLength, &myInt);
	//for (int32 idx = 0; idx <= int32(myInt) - 1; idx++)

	for (int32 idx = 0; idx <= int32(meshQty) - 1; idx++)
	{
		USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass(), FName("SplineMesh" + FString::FromInt(idx)));
		FVector pointLocationStart, pointTangentStart, pointLocationEnd, pointTangentEnd;

		SplineMesh->SetStaticMesh(Mesh);
		SplineMesh->SetForwardAxis(ESplineMeshAxis::Y, true);
		//SplineMesh->SetSplineUpDir(FVector(0.f, 1.f, 0.f));
		//SplineMesh->SetSplineUpDir(Spline->GetUpVectorAtDistanceAlongSpline( ((idx * meshLength) + meshLength) / 2, ESplineCoordinateSpace::Type::World));
		SplineMesh->SetSplineUpDir(Spline->GetRightVectorAtDistanceAlongSpline(((idx * meshLength) + meshLength) / 2, ESplineCoordinateSpace::Type::World));
		//SplineMesh->SetSplineUpDir(Spline->GetForwardVector());
		SplineMesh->SetCastShadow(false);

		pointLocationStart = Spline->GetLocationAtDistanceAlongSpline(idx * meshLength, ESplineCoordinateSpace::Type::World);
		pointTangentStart = Spline->GetTangentAtDistanceAlongSpline(idx * meshLength, ESplineCoordinateSpace::Type::World);
		pointTangentStart = UKismetMathLibrary::ClampVectorSize(pointTangentStart, 0.f, meshLength);

		pointLocationEnd = Spline->GetLocationAtDistanceAlongSpline((idx + 1) * meshLength, ESplineCoordinateSpace::Type::World);
		pointTangentEnd = Spline->GetTangentAtDistanceAlongSpline((idx + 1) * meshLength, ESplineCoordinateSpace::Type::World);
		pointTangentEnd = UKismetMathLibrary::ClampVectorSize(pointTangentEnd, 0.f, meshLength);

		SplineMesh->SetStartAndEnd(pointLocationStart, pointTangentStart, pointLocationEnd, pointTangentEnd);
		SplineMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		SplineMesh->SetupAttachment(RootComponent);
		SplineMesh->RegisterComponent();
	}
}