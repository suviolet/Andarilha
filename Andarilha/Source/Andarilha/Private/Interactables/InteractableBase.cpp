#include "Interactables/InteractableBase.h"


AInteractableBase::AInteractableBase()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;
	RootComponent->bVisualizeComponent = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetCollisionProfileName(FName("BlockAll"), false); //BlockAllDynamic
	StaticMesh->SetSimulatePhysics(false);//true
	StaticMesh->SetupAttachment(RootComponent);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->InitBoxExtent(FVector(32.f, 32.f, 32.f));
	Box->SetupAttachment(StaticMesh);

	BoxTriggerSignal = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTriggerSignal"));
	BoxTriggerSignal->InitBoxExtent(FVector(90.f, 90.f, 90.f));
	BoxTriggerSignal->SetupAttachment(StaticMesh);

	static ConstructorHelpers::FClassFinder<UUserWidget>TriggerSignalWidgetClassFound(TEXT("/Script/UMG.WidgetBlueprintGeneratedClass'/Game/BPs/WBPs/WBP_TriggerSignal.WBP_TriggerSignal_C'"));
	if (TriggerSignalWidgetClassFound.Class != nullptr)
	{
		TriggerSignalWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TriggerSignalWidget"));
		TriggerSignalWidget->SetWidgetClass(TriggerSignalWidgetClassFound.Class);
		TriggerSignalWidget->SetWidgetSpace(EWidgetSpace::Screen);
		TriggerSignalWidget->SetVisibility(false);
		TriggerSignalWidget->SetupAttachment(BoxTriggerSignal);
	}

	BoxTriggerSignal->OnComponentBeginOverlap.AddDynamic(this, &AInteractableBase::DisplaySignalWidget);
	BoxTriggerSignal->OnComponentEndOverlap.AddDynamic(this, &AInteractableBase::RemoveSignalWidget);

}

bool AInteractableBase::Interact(FItemStruct item) { return true; }

void AInteractableBase::DisplaySignalWidget(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//if (APCharacter* PCharacter = Cast<APCharacter>(OtherActor))
		if (OtherActor->ActorHasTag(FName("PCharacter")))
		{
			TriggerSignalWidget->SetVisibility(true);
		}
	}
}

void AInteractableBase::RemoveSignalWidget(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//if (APCharacter* PCharacter = Cast<APCharacter>(OtherActor))
		if (OtherActor->ActorHasTag(FName("PCharacter")))
		{
			TriggerSignalWidget->SetVisibility(false);
		}
	}
}