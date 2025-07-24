#include "MovableActors/SplineBase.h"

ASplineBase::ASplineBase()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	DefaultSceneRoot->SetMobility(EComponentMobility::Static);
	RootComponent = DefaultSceneRoot;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetMobility(EComponentMobility::Static);
	Spline->SetDrawDebug(true);
	Spline->SetupAttachment(RootComponent);
}

void ASplineBase::BeginPlay()
{
	Super::BeginPlay();
	
}
