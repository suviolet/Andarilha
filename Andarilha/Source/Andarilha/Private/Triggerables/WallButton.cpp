#include "Triggerables/WallButton.h"

AWallButton::AWallButton()
{
	StaticMesh->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));

	Box->InitBoxExtent(FVector(75.f, 75.f, 55.f));
	Box->SetRelativeLocation(FVector(100.f, 0.f, 0.f));

	BoxTriggerSignal->InitBoxExtent(FVector(360.f, 360.f, 360.f));
	BoxTriggerSignal->SetRelativeLocation(FVector(360.f, 0.f, 0.f));

	ButtonCurve = CreateDefaultSubobject<UCurveFloat>(TEXT("ButtonCurve"));
	ButtonCurve->FloatCurve.UpdateOrAddKey(0.f, 0.f);
	ButtonCurve->FloatCurve.UpdateOrAddKey(2.f, -10.f);

	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
}

void AWallButton::BeginPlay()
{
	Super::BeginPlay();

	if (ButtonCurve)
	{
		TimelineCallback.BindDynamic(this, &AWallButton::Press);

		TimelineComp->AddInterpFloat(ButtonCurve, TimelineCallback);
	}

}

void AWallButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWallButton::OnTrigger()
{
	if (!bHasBeenTriggered)
	{
		OnTriggerDelegate.Execute();
		TimelineComp->Play();
		bHasBeenTriggered = true;
	}
}

void AWallButton::Press(float Output)
{
	FVector NewLocation = FVector(Output, 0.0f, 0.0f);
	StaticMesh->SetRelativeLocation(NewLocation);
}