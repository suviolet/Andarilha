#include "Triggerables/Checkpoint.h"
#include "Characters/PCharacter.h"
#include "Kismet/GameplayStatics.h"

ACheckpoint::ACheckpoint()
{
	bDisplayWidget = false;
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::SaveGame);
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	currentWorld = this->GetWorld();

	if (IsValid(SaveWidgetClass))
	{
		savingWidget = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), SaveWidgetClass);
	}
}

void ACheckpoint::SaveGame(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (APCharacter* PCharacter = Cast<APCharacter>(OtherActor))
		{
			if (!bHasBeenTriggered && IsValid(savingWidget))
			{
				bHasBeenTriggered = true;
				savingWidget->AddToViewport();

				if (PCharacter != nullptr)
				//if (IsValid(PCharacter))
				{

					if (currentWorld != nullptr)
					//if (IsValid(currentWorld))
					{

						UE_LOG(LogTemp, Warning, TEXT(" PCharacter->GetWorld  "));
						PCharacter->SaveSystemComponent->Save();
						FTimerHandle TimerHandle;
						currentWorld->GetTimerManager().SetTimer(TimerHandle, this, &ACheckpoint::AutoDestroy, 2.f);
					}
				}
			}
		}
	}
}

void ACheckpoint::AutoDestroy()
{
	savingWidget->RemoveFromParent();
	this->Destroy();
}