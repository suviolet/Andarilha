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
		// Assertion failed: !IsUnreachable() [File:D:\build\++UE5\Sync\Engine\Source\Runtime\CoreUObject\Private\UObject\ScriptCore.cpp] [Line: 1976] WBP_Saving_C /Engine/Transient.UnrealEdEngine_0:GameInstance_2.None Function: '/Script/UMG.UserWidget:PreConstruct'
		savingWidget = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), SaveWidgetClass);
	}
}

void ACheckpoint::SaveGame(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (APCharacter* PCharacter = Cast<APCharacter>(OtherActor))
		{
			if (!bHasBeenTriggered && savingWidget != nullptr)
			{
				bHasBeenTriggered = true;
				if (savingWidget->IsInViewport()) return;
				savingWidget->AddToViewport();

				//if (IsValid(PCharacter))
				if (PCharacter != nullptr)
				{

					//if (IsValid(currentWorld))
					if (currentWorld != nullptr)   // <----
					{

						UE_LOG(LogTemp, Warning, TEXT(" PCharacter->GetWorld  "));
						PCharacter->SaveSystemComponent->Save();

						currentWorld->GetTimerManager().ClearTimer(TimerHandle);
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