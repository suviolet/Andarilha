
#include "MovableActors/Rail.h"

ARail::ARail()
{
	sectionLenght = 100.f;
}

void ARail::BeginPlay()
{
	Super::BeginPlay();
}

void ARail::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	float myInt = 0.0f;
	modf(Spline->GetSplineLength() / sectionLenght, &myInt);

	for (int32 idx = 0; idx <= int32(myInt) - 1;)
	{
		USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass(), FName("SplineMesh" + FString::FromInt(idx)));
		FVector pointLocationStart, pointTangentStart, pointLocationEnd, pointTangentEnd;

		SplineMesh->SetStaticMesh(Mesh);
		SplineMesh->SetForwardAxis(ESplineMeshAxis::Y, true);
		SplineMesh->SetSplineUpDir(FVector(0.f, 1.f, 0.f));
		SplineMesh->SetCastShadow(false);

		pointLocationStart = Spline->GetLocationAtDistanceAlongSpline(idx * sectionLenght, ESplineCoordinateSpace::Type::World);
		pointTangentStart = Spline->GetTangentAtDistanceAlongSpline(idx * sectionLenght, ESplineCoordinateSpace::Type::World);
		pointTangentStart = UKismetMathLibrary::ClampVectorSize(pointTangentStart, 0.f, sectionLenght);

		pointLocationEnd = Spline->GetLocationAtDistanceAlongSpline((idx+1) * sectionLenght, ESplineCoordinateSpace::Type::World);
		pointTangentEnd = Spline->GetTangentAtDistanceAlongSpline((idx + 1) * sectionLenght, ESplineCoordinateSpace::Type::World);
		pointTangentEnd = UKismetMathLibrary::ClampVectorSize(pointTangentEnd, 0.f, sectionLenght);

		SplineMesh->SetStartAndEnd(pointLocationStart, pointTangentStart, pointLocationEnd, pointTangentEnd);
		SplineMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics); // ?
		SplineMesh->SetupAttachment(RootComponent);
		SplineMesh->RegisterComponent();

		idx++;
	}
}