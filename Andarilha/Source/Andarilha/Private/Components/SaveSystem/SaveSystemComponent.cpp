#include "Components/SaveSystem/SaveSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PCharacter.h"
#include "Components/SaveSystem/SlotSaveGame.h"


USaveSystemComponent::USaveSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SaveSlotName = TEXT("SaveGame00");
}


void USaveSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	PlayerCharacter = Cast<APCharacter>(OwnerActor);

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT(" It has a Save at BegingPlay! "));
		CurrentSaveGame = Cast<USlotSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

		//PlayerCharacter->SetActorTransform(CurrentSaveGame->Data.ActorTransform);
		//PlayerCharacter->Controller.transfo
		// Load Level?
		// Load Inventory?
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT(" Create first Save ! "));
		CurrentSaveGame = CastChecked<USlotSaveGame>(UGameplayStatics::CreateSaveGameObject(USlotSaveGame::StaticClass()));

		CurrentSaveGame->Data.ActorTransform = PlayerCharacter->GetActorTransform();
		CurrentSaveGame->Data.Level = CurrentLevel;   
		// Save Inventory?

		UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
	}
}

// I wanna call Save regardless this InputActions arg...
void USaveSystemComponent::Save(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT(" Save ! "));

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		CurrentSaveGame->Data.ActorTransform = PlayerCharacter->GetActorTransform();
		CurrentSaveGame->Data.Level = CurrentLevel;
		// Save Inventory?

		UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT(" Doesn't have a previous save! "));
	}

}

// I wanna call Load regardless this InputActions arg...
void USaveSystemComponent::Load(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT(" Load ! "));

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, CurrentSaveGame->Data.Level, true, false, LatentInfo);
		PlayerCharacter->SetActorTransform(CurrentSaveGame->Data.ActorTransform);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT(" Doesn't have a previous save TO LOAD! "));
	}
}

void USaveSystemComponent::CleanSlot()
{
	UGameplayStatics::DeleteGameInSlot(SaveSlotName, 0);
}