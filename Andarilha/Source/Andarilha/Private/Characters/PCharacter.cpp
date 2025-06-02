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
	PrimaryActorTick.bCanEverTick = true;

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
	MovementComponent->MaxWalkSpeed = 300.0f;
	MovementComponent->MaxWalkSpeedCrouched = 200.0f;


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
	}

}

void APCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

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

void APCharacter::Interact(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Oi?"));
	if (isAlive)
	{

		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), sphereOverlapRadius, traceObjectTypes, seekClass, ignoreActors, overlappedActors);
		DrawDebugSphere(GetWorld(), GetActorLocation(), sphereOverlapRadius, 12, FColor::Red, false, 1.0f);

		for (AActor* overlappedActor : overlappedActors)
		{
			AInteractableBase* Interactable = Cast<AInteractableBase>(overlappedActor);
			if (Interactable->ActorHasTag(FName("TriggerableBase"))) {
				UE_LOG(LogTemp, Log, TEXT("OverlappedActor  :) %s"), *Interactable->GetName());
				Interactable->Interact();
			}
		}
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

void APCharacter::Run(const FInputActionValue& Value)
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

		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &APCharacter::Jump);
		Input->BindAction(InteractAction, ETriggerEvent::Started, this, &APCharacter::Interact);
		Input->BindAction(LockTurnAction, ETriggerEvent::Started, this, &APCharacter::LockTurn);
		Input->BindAction(LockTurnAction, ETriggerEvent::Completed, this, &APCharacter::LockTurn);
		Input->BindAction(CrouchAction, ETriggerEvent::Started, this, &APCharacter::CustomCrouch);
		Input->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APCharacter::CustomCrouch);
		Input->BindAction(RunAction, ETriggerEvent::Started, this, &APCharacter::Run);
		Input->BindAction(RunAction, ETriggerEvent::Completed, this, &APCharacter::Run);
		Input->BindAction(StartAction, ETriggerEvent::Completed, this, &APCharacter::Start);
	}

}

