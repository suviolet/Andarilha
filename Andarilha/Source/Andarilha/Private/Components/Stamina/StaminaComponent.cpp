#include "Components/Stamina/StaminaComponent.h"
#include "Widgets/StaminaWidget.h"
#include "Characters/PCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	stamina = 100.f;
	maxStamina = stamina;

	amountToDecrease = 6.f;
	amountToRecover = 2.f;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerActor = GetOwner();
	PlayerCharacter = Cast<APCharacter>(OwnerActor);

	if (StaminaWidgetClass != nullptr)
	{
		widget = CreateWidget<UStaminaWidget>(UGameplayStatics::GetPlayerController(this, 0), StaminaWidgetClass);
	}
}

void UStaminaComponent::DecreaseStamina()
{

	if (PlayerCharacter->isRunning)
	{
		stamina -= amountToDecrease;
		stamina = FMath::Max(stamina, 0);
		widget->UpdateStaminaBar(stamina / maxStamina);
		//UE_LOG(LogTemp, Warning, TEXT("UStaminaComponent::DecreaseStamina : stamina %f"), stamina);

		if (stamina <= 0)
		{
			PlayerCharacter->bCanRun = false;
			PlayerCharacter->Run(FInputActionValue(false));

			// stop decreasing / calling this func
			//UE_LOG(LogTemp, Warning, TEXT("UStaminaComponent::DecreaseStamina : stop decreasing / calling this func"));
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UStaminaComponent::DecreaseStamina, .2f);
		}
	}
}

void UStaminaComponent::RecoverStamina()
{
	if (!PlayerCharacter->isRunning || !PlayerCharacter->bCanRun)
	{
		stamina += amountToRecover;
		stamina = FMath::Min(stamina, maxStamina);
		widget->UpdateStaminaBar(stamina / maxStamina);
		//UE_LOG(LogTemp, Warning, TEXT("UStaminaComponent::RecoverStamina : stamina %f"), stamina);

		if (stamina >= maxStamina)
		{
			// stop increasing / calling this func
			//UE_LOG(LogTemp, Warning, TEXT("UStaminaComponent::RecoverStamina : stop increasing / calling this func"));
			DisplayWidget(false);
		}
		else
		{
			if(stamina > 75)
			{
				PlayerCharacter->bCanRun = true;
			}
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UStaminaComponent::RecoverStamina, .2f);
		}
	}
}

void UStaminaComponent::DisplayWidget(bool bCanDisplay)
{
	if (bCanDisplay)
	{
		widget->AddToViewport();
	}
	else
	{
		widget->RemoveFromParent();
	}
}
