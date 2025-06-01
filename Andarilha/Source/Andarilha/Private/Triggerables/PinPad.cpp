#include "Triggerables/PinPad.h"

APinPad::APinPad()
{
	StaticMesh->SetRelativeScale3D(FVector(0.1f, 0.25f, 0.25f));

	Box->InitBoxExtent(FVector(300.f, 75.f, 55.f));
	Box->SetRelativeLocation(FVector(300.f, 0.f, 0.f));

	BoxTriggerSignal->InitBoxExtent(FVector(360.f, 360.f, 360.f));
	BoxTriggerSignal->SetRelativeLocation(FVector(360.f, 0.f, 0.f));

	static ConstructorHelpers::FClassFinder<UUserWidget> PinPadWidgetClassFound(TEXT("/Script/UMG.WidgetBlueprintGeneratedClass'/Game/BPs/WBPs/WBP_PinPad.WBP_PinPad_C'"));
	if (PinPadWidgetClassFound.Class != nullptr)
	{
		PinPadWidgetClass = PinPadWidgetClassFound.Class;
	}
	PinPadWidgetOnWorld = CreateDefaultSubobject<UWidgetComponent>(TEXT("PinPadWidgetOnWorld"));
	PinPadWidgetOnWorld->SetWidgetClass(PinPadWidgetClass);
	PinPadWidgetOnWorld->SetWidgetSpace(EWidgetSpace::World);
	PinPadWidgetOnWorld->SetRelativeLocation(FVector(55.f, 0.f, 0.f));
	PinPadWidgetOnWorld->SetRelativeScale3D(FVector(0.125f, 0.125f, 0.125f));
	PinPadWidgetOnWorld->SetDrawSize(FVector2D(500.f, 800.f));
	PinPadWidgetOnWorld->SetVisibility(true);
	PinPadWidgetOnWorld->SetupAttachment(StaticMesh);

	Pin = "0710";
	bIsDialing = false;
}

void APinPad::BeginPlay()
{
	Super::BeginPlay();

	// Binds Event OnCorrectedPIN with widget
}

void APinPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APinPad::OnTrigger()
{
	if (!bHasBeenTriggered)
	{
		Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (!bIsDialing)
		{
			bIsDialing = true;
			Player->GetCharacterMovement()->DisableMovement();

			if (PinPadWidgetClass)
			{
				PinPadWidget = CreateWidget<UPinPadWidget>(Controller, PinPadWidgetClass, "Pin Pad Widget");
				PinPadWidget->SetPin(Pin);
				PinPadWidget->AddToViewport();
			}

			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(Controller);
			Controller->SetShowMouseCursor(true); // Probably not with joystick
		}
		else
		{
			ResumeGame();
		}

	}
}

void APinPad::ResumeGame()
{
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller);			//Controller->SetInputMode(FInputModeGameOnly());
	Controller->SetShowMouseCursor(false);
	bIsDialing = false;
}

void APinPad::OnCorrectedPin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("APinPad::OnCorrectedPin()"));
	bHasBeenTriggered = true;
	OnTriggerDelegate.Execute();
	//Play Sound
	ResumeGame();
	bIsDialing = false;
}

//Event OnCorrectedPIN with widget
// 
// Success path opens door
// 
//bHasBeenTriggered = true;
//OnTriggerDelegate.Execute();
//Play Sound
// Resume Game
//bIsDialing = false;