#include "Components/SaveSystem/SaveSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PCharacter.h"
#include "Components/SaveSystem/SlotSaveGame.h"


USaveSystemComponent::USaveSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SaveSlotName = TEXT("SaveGame00");
	UUID = 0;
}


void USaveSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();

	currentWorld = this->GetWorld();

	PlayerCharacter = Cast<APCharacter>(OwnerActor);
	CurrentSaveGame = CastChecked<USlotSaveGame>(UGameplayStatics::CreateSaveGameObject(USlotSaveGame::StaticClass()));
}

bool USaveSystemComponent::FirstSave()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT(" USaveSystemComponent::FirstSave! "));
	CleanSlot();

	CurrentSaveGame->Data.ActorTransform = PlayerCharacter->GetActorTransform();

	CurrentSaveGame->Data.Levels.Empty();
	TArray<FString> levelsToLoad = { "CityHall", "WalledBuildings", "Downtown" };
	const TArray<ULevelStreaming*>& streamedLevels = currentWorld->GetStreamingLevels();

	for (FString levelToLoad : levelsToLoad)
	{
		for (ULevelStreaming* streamedLevel : streamedLevels)
		{
			FString packageName = streamedLevel->GetWorldAssetPackageName();
			if (packageName.EndsWith(levelToLoad))
			{
				CurrentSaveGame->Data.Levels.Add(FName(packageName));

				FLatentActionInfo info;
				info.UUID = UUID;
				UGameplayStatics::LoadStreamLevel(this, FName(packageName), true, true, info);
				UUID++;
			}
		}
	}

	// Save Inventory?

	return UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
}

bool USaveSystemComponent::Save()
{
	UE_LOG(LogTemp, Warning, TEXT(" USaveSystemComponent::Save"));

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		CurrentSaveGame->Data.ActorTransform = PlayerCharacter->GetActorTransform();

		CurrentSaveGame->Data.Levels.Empty();
		const TArray<ULevelStreaming*>& streamedLevels = currentWorld->GetStreamingLevels();

		for (ULevelStreaming* streamedLevel : streamedLevels)
		{
			if (
				streamedLevel->GetLevelStreamingStatus() == EStreamingStatus::LEVEL_Loaded || 
				streamedLevel->GetLevelStreamingStatus() == EStreamingStatus::LEVEL_Visible
			)
			{
				FString packageName = streamedLevel->GetWorldAssetPackageName();
				CurrentSaveGame->Data.Levels.Add(FName(packageName));
				UE_LOG(LogTemp, Warning, TEXT("Saved Level   : %s"), *packageName);
			}
		}

		// Save Inventory?

		return UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" USaveSystemComponent::Save Calling FirstSave()! "));
		return FirstSave();
	}
}

bool USaveSystemComponent::Load()
{
	UE_LOG(LogTemp, Warning, TEXT(" USaveSystemComponent::Load"));
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{

		USaveGame* saveGame = UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0);
		CurrentSaveGame = CastChecked<USlotSaveGame>(saveGame);

		PlayerCharacter->SetActorTransform(CurrentSaveGame->Data.ActorTransform);

		for (FName level : CurrentSaveGame->Data.Levels)
		{
			FLatentActionInfo info;
			info.UUID = UUID;
			UE_LOG(LogTemp, Warning, TEXT(" USaveSystemComponent::Load  Level  %s"), *level.ToString());
			UGameplayStatics::LoadStreamLevel(this, level, true, true, info);
			UUID++;
		}
		// Load Inventory

		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("USaveSystemComponent::Load Doesn't have a previous save TO LOAD! "));
		return false;
	}
}

void USaveSystemComponent::CleanSlot()
{
	UGameplayStatics::DeleteGameInSlot(SaveSlotName, 0);
}