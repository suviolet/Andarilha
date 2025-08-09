#include "Characters/PCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/InventoryWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

APCharacter::APCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	Capsule = GetCapsuleComponent();
	Capsule->SetCapsuleHalfHeight(75, true);
	Capsule->SetCapsuleRadius(25, true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(Capsule);
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 65.f));
	SpringArm->ProbeSize = 0;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 25.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	MovementComponent = this->GetCharacterMovement();
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->JumpZVelocity = 600.0f;
	MovementComponent->GravityScale = 1.5f;
	MovementComponent->MaxWalkSpeed = 300.0f;
	MovementComponent->MaxWalkSpeedCrouched = 200.0f;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
	InventoryComponent->MaxSlotSize = 6;
	this->AddOwnedComponent(InventoryComponent);

	SaveSystemComponent = CreateDefaultSubobject<USaveSystemComponent>(TEXT("SaveSystem Component"));
	this->AddOwnedComponent(SaveSystemComponent);

	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina Component"));
	this->AddOwnedComponent(StaminaComponent);

	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

	ignoreActors.Add(GetOwner());
	seekClass = AInteractableBase::StaticClass();

	sphereOverlapRadius = 75.0f;
	isAlive = true;
	isCrouching = false;
	isRunning = false;
	bCanRun = true;
	isTurnLocked = false;

	Tags.Add(FName("PCharacter"));
}

void APCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(Controller);
	PlayerController->PlayerCameraManager->ViewPitchMax = 15.f;
	PlayerController->PlayerCameraManager->ViewPitchMin = -45.f;
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMapping, 0);
	}

	RowNames = ItemsDataTable->GetRowNames();
}

//void APCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//	//this->SetCameraCrouchPosition();
//}

void APCharacter::Start(const FInputActionValue& Value)
{

}

void APCharacter::Move(const FInputActionValue& Value)
{
	if (isAlive)
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, MovementVector.Y);
		AddMovementInput(Right, MovementVector.X);
	}
}

void APCharacter::Turn(const FInputActionValue& Value)
{
	const FVector2D TurnAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(TurnAxisVector.X);
	AddControllerPitchInput(-TurnAxisVector.Y);
}

bool APCharacter::Interaction(bool useSelectedItem)
{
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), sphereOverlapRadius, traceObjectTypes, seekClass, ignoreActors, overlappedActors);
	DrawDebugSphere(GetWorld(), GetActorLocation(), sphereOverlapRadius, 12, FColor::Blue, false, 1.0f);

	for (AActor* overlappedActor : overlappedActors)
	{
		AInteractableBase* Interactable = Cast<AInteractableBase>(overlappedActor);
		UE_LOG(LogTemp, Log, TEXT("OverlappedActor  :) %s"), *Interactable->GetName());

		if (Interactable->ActorHasTag(FName("Lock")) && useSelectedItem)
		{
			FItemStruct item = InventoryComponent->GetSlot(InventoryComponent->currentSlotIndex)->Item;
			return Interactable->Interact(item);
		}
		else if (Interactable->ActorHasTag(FName("TriggerableBase")) && !Interactable->ActorHasTag(FName("Lock")))
		{
			return Interactable->Interact();
		}
		else if (Interactable->ActorHasTag(FName("CollectableBase")) and ItemsDataTable != nullptr)
		{
			static const FString ContextString(TEXT("Finding Row in Items Data Table"));

			FItemStruct* Item = ItemsDataTable->FindRow<FItemStruct>(RowNames[Interactable->index], ContextString);
			bool bWasItemAdd = InventoryComponent->AddItem(*Item);
			if (bWasItemAdd)
			{
				Interactable->Interact();
				return true;
			}
		}
	}
	return false;
}

void APCharacter::Interact(const FInputActionValue& Value)
{
	if (isAlive)
	{
		Interaction();
	}
}

void APCharacter::LockTurn(const FInputActionValue& Value)
{
	if (isAlive)
	{
		isTurnLocked = Value.Get<bool>();
		if (isTurnLocked) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("isTurnLocked"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("!isTurnLocked"));
		}
	}
}

void APCharacter::CustomCrouch(const FInputActionValue& Value)
{
	if (isAlive)
	{
		isCrouching = Value.Get<bool>();
		MovementComponent->bWantsToCrouch = isCrouching;
	}
}

void APCharacter::Run(const FInputActionValue& Value) //sprint
{
	if (isAlive)
	{
		isRunning = bool(Value.Get<float>());

		if (isRunning and bCanRun) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("isRunning"));
			MovementComponent->MaxWalkSpeed = 600.0f;
			StaminaComponent->DisplayWidget(true);
			StaminaComponent->DecreaseStamina();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("! isRunning"));
			MovementComponent->MaxWalkSpeed = 300.0f;
			StaminaComponent->RecoverStamina();
		}
	}
}

void APCharacter::JumpStart()
{
	if (MovementComponent->IsFalling()) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && JumpMontage && MovementComponent->MovementMode == EMovementMode::MOVE_Walking)
	{
		AnimInstance->Montage_Play(JumpMontage);
	}
}

void APCharacter::UseItem(const FInputActionValue& Value)
{
	if (isAlive)
	{
		if (InventoryComponent->currentSlotIndex > -1)
		{
			bool bInteracted = Interaction(true);
			if (bInteracted)
			{
				InventoryComponent->RemoveItemAtIndex(InventoryComponent->currentSlotIndex);
				InventoryComponent->currentSlotIndex = -1;
				//plays sound of consequence of triggerable on Object suffering consequence
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT(" NO ! "));
				//just plays "no" sound
			}
		}
	}
}


void APCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if (MovementComponent->IsFalling()) {
		const float fallSpeed = this->GetVelocity().Z;
		UE_LOG(LogTemp, Log, TEXT("APCharacter::Landed : speed %f"), fallSpeed);
		if (fallSpeed <= -1350.0f) // =~ 6 meters
		{
			this->Die();
		}
	}
}

void APCharacter::Die()
{
	if (isAlive)
	{
		isAlive = false;
		UE_LOG(LogTemp, Log, TEXT("APCharacter::Die"));

		UWidgetLayoutLibrary::RemoveAllWidgets(this);
		PlayerController->PlayerCameraManager->StartCameraFade(0.f, 1.f, 1.f, FLinearColor::Black, true, true);

		if (GameOverWidgetClass != nullptr)
		{
			gameOverWidget = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), GameOverWidgetClass);
			gameOverWidget->AddToViewport();
		}

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APCharacter::RestartLastSave, 3.f); 
	}
}

void APCharacter::ExitGame(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("APCharacter::ExitGame"));
	UKismetSystemLibrary::QuitGame(this, this->PlayerController, EQuitPreference::Quit, true);
}

void APCharacter::RestartLastSave()
{
	isAlive = true;
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
	SaveSystemComponent->Load();
	PlayerController->PlayerCameraManager->StopCameraFade();

	UInventoryWidget* inventoryWidget = InventoryComponent->widget;
	inventoryWidget->AddToViewport();
}

void APCharacter::LoadGame(const FInputActionValue& Value)
{
	SaveSystemComponent->Load();
}

void APCharacter::SaveGame(const FInputActionValue& Value)
{
	SaveSystemComponent->Save();
}

void APCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APCharacter::Move);
		Input->BindAction(TurnAction, ETriggerEvent::Triggered, this, &APCharacter::Turn);

		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &APCharacter::JumpStart);

		Input->BindAction(InteractAction, ETriggerEvent::Started, this, &APCharacter::Interact);

		Input->BindAction(LockTurnAction, ETriggerEvent::Started, this, &APCharacter::LockTurn);
		Input->BindAction(LockTurnAction, ETriggerEvent::Completed, this, &APCharacter::LockTurn);

		Input->BindAction(CrouchAction, ETriggerEvent::Started, this, &APCharacter::CustomCrouch);
		Input->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APCharacter::CustomCrouch);

		//Input->BindAction(RunAction, ETriggerEvent::Started, this, &APCharacter::Run);
		//Input->BindAction(RunAction, ETriggerEvent::Completed, this, &APCharacter::Run);

		Input->BindAction(RunAction, ETriggerEvent::Triggered, this, &APCharacter::Run);

		//Input->BindAction("RunAction", IE_Pressed, this, &APCharacter::Run);
		//Input->BindAction("RunAction", IE_Released, this, &APCharacter::Run);

		Input->BindAction(StartAction, ETriggerEvent::Completed, this, &APCharacter::Start);

		//// Inventory UI Interaction
		Input->BindAction(MoveUpUIAction, ETriggerEvent::Started, InventoryComponent, &UInventoryComponent::MoveUpUI);
		Input->BindAction(MoveDownUIAction, ETriggerEvent::Started, InventoryComponent, &UInventoryComponent::MoveDownUI);
		Input->BindAction(UseUIAction, ETriggerEvent::Started, this, &APCharacter::UseItem);
		Input->BindAction(DropUIAction, ETriggerEvent::Started, InventoryComponent, &UInventoryComponent::DropUI);
		////

		//// Menu elements
		Input->BindAction(SaveAction, ETriggerEvent::Started, this, &APCharacter::SaveGame);
		Input->BindAction(LoadAction, ETriggerEvent::Started, this, &APCharacter::LoadGame);

		Input->BindAction(ExitGameAction, ETriggerEvent::Started, this, &APCharacter::ExitGame);
		////
	}

}

