#include "Components/Stamina/StaminaComponent.h"
#include "Blueprint/UserWidget.h"
#include "Characters/PCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	value = 100.f;

	amountToDecrease = 9.f;
	amountToRecover = 1.f;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();
	PlayerCharacter = Cast<APCharacter>(OwnerActor);

	if (StaminaWidgetClass != nullptr)
	{
		widget = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), StaminaWidgetClass);
		widget->AddToViewport(); // move to another logic, it is appearing on Main Menu, Pause Menu, etc..  !!!!!
	}
}

void UStaminaComponent::DecreaseStamina()
{

	if (PlayerCharacter->isRunning)
	{
		value -= amountToDecrease;
		if (value <= 0)
		{
			PlayerCharacter->bCanRun = false;
			PlayerCharacter->isRunning = false;
			PlayerCharacter->MovementComponent->MaxWalkSpeed = 300.f;
			value = 0;

			// stop decreasing / calling this func
			UE_LOG(LogTemp, Warning, TEXT("UStaminaComponent::DecreaseStamina : stop decreasing / calling this func"));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("UStaminaComponent::DecreaseStamina : stop decreasing / calling this func"));
		}
		else
		{

			UE_LOG(LogTemp, Warning, TEXT("UStaminaComponent::DecreaseStamina : value %f"), value);
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("UStaminaComponent::DecreaseStamina : value %f"), value));

			//Retriggerable
			FLatentActionInfo LatentAction;
			LatentAction.Linkage = 0;
			LatentAction.CallbackTarget = this;
			LatentAction.UUID = GetUniqueID();
			LatentAction.ExecutionFunction = "DecreaseStamina";
			UKismetSystemLibrary::RetriggerableDelay(OwnerActor, .2f, LatentAction);

		}
	}

}

void UStaminaComponent::RecoverStamina()
{
	if (!PlayerCharacter->isRunning)
	{
		value += amountToRecover;
		if (value >= 100)
		{
			// stop increasing / calling this func
			UE_LOG(LogTemp, Warning, TEXT("UStaminaComponent::RecoverStamina : stop increasing / calling this func"));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("UStaminaComponent::RecoverStamina : stop increasing / calling this func"));
		}
		else
		{
			if(value > 75)
			{
				PlayerCharacter->bCanRun = true;
			}
			UE_LOG(LogTemp, Warning, TEXT("UStaminaComponent::RecoverStamina : value %f"), value);
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("UStaminaComponent::RecoverStamina : value %f"), value));

			//Retriggerable
			FLatentActionInfo LatentAction;
			LatentAction.Linkage = 0;
			LatentAction.CallbackTarget = this;
			LatentAction.UUID = GetUniqueID();
			LatentAction.ExecutionFunction = "RecoverStamina";
			UKismetSystemLibrary::RetriggerableDelay(OwnerActor, .2f, LatentAction);
		}
	}
}