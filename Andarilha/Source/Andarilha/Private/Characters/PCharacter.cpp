#include "Characters/PCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APCharacter::APCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	Capsule = GetCapsuleComponent();
	Capsule->SetupAttachment(RootComponent);
	Capsule->SetCapsuleHalfHeight(75, true);
	Capsule->SetCapsuleRadius(25, true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(Capsule);
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 65.f));
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


	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

	ignoreActors.Add(GetOwner());
	seekClass = AInteractableBase::StaticClass();

	sphereOverlapRadius = 75.0f;
	isAlive = true;
	isCrouching = false;
	isRunning = false;
	isTurnLocked = false;

	Tags.Add(FName("PCharacter"));
}

void APCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}

		PlayerController->PlayerCameraManager->ViewPitchMax = 15.f;
		PlayerController->PlayerCameraManager->ViewPitchMin = -45.f;
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
		isRunning = Value.Get<bool>();
		if (isRunning) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("isRunning"));
			MovementComponent->MaxWalkSpeed = 600.0f;
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("! isRunning"));
			MovementComponent->MaxWalkSpeed = 300.0f;
		}
	}
}

void APCharacter::JumpStart()
{
	if (MovementComponent->IsFalling()) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && JumpMontage)
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

//void APCharacter::OnPlayerLanded(const FHitResult& Hit)
//{
//	// Here we may create other mechanics based on where/how the player lands
//	if (MovementComponent->IsFalling()) {
//		const float fallSpeed = this->GetVelocity().Z;
//		if (fallSpeed <= -700.0f) {
//			this->Die();
//		}
//	}
//}

void APCharacter::Die()
{
	//UE_LOG(LogTemp, Warning, TEXT("Die!"))
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Die!"));
	//
	// it may apply some changes on the mesh/capsule collider
	// here changes GameMode?GameState to GameOver 
	// -> then will disable inputs 
	// -> display game over on screen 
	// -> reallow inputs to restart 
	// 
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

		Input->BindAction(RunAction, ETriggerEvent::Started, this, &APCharacter::Run);
		Input->BindAction(RunAction, ETriggerEvent::Completed, this, &APCharacter::Run);

		Input->BindAction(StartAction, ETriggerEvent::Completed, this, &APCharacter::Start);

		Input->BindAction(MoveUpUIAction, ETriggerEvent::Started, InventoryComponent, &UInventoryComponent::MoveUpUI);
		Input->BindAction(MoveDownUIAction, ETriggerEvent::Started, InventoryComponent, &UInventoryComponent::MoveDownUI);
		Input->BindAction(UseUIAction, ETriggerEvent::Started, this, &APCharacter::UseItem);
		Input->BindAction(DropUIAction, ETriggerEvent::Started, InventoryComponent, &UInventoryComponent::DropUI);
	}

}

