#include "MovableActors/SplineBase.h"

ASplineBase::ASplineBase()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetDrawDebug(true);
	Spline->SetupAttachment(RootComponent);
}

void ASplineBase::BeginPlay()
{
	Super::BeginPlay();
	
}
