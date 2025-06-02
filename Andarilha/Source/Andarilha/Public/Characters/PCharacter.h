#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interactables/InteractableBase.h"
#include "PCharacter.generated.h"

class UCapsuleComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class ANDARILHA_API APCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = "Camera")
	class UCameraComponent* Camera;
	float cameraHeight;
	float cameraDefaultHeight;
	float cameraCrouchedHeight;

	////////////////////////////////////////// INPUTS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> LockTurnAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> RunAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> StartAction;

	////////////////////////////////////////// INPUTS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	TArray<AActor*> ignoreActors;
	TArray<AActor*> overlappedActors;
	UClass* seekClass;

	float sphereOverlapRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAlive;

	bool isCrouching;
	bool isRunning;
	bool isTurnLocked;

private:

	UCharacterMovementComponent* MovementComponent;

	//UFUNCTION()
	//void OnPlayerLanded(const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Turn(const FInputActionValue& Value);

	UFUNCTION()
	void Interact(const FInputActionValue& Value);

	UFUNCTION()
	void LockTurn(const FInputActionValue& Value);

	UFUNCTION()
	void CustomCrouch(const FInputActionValue& Value);

	UFUNCTION()
	void Run(const FInputActionValue& Value);

	UFUNCTION()
	void Start(const FInputActionValue& Value);

	UFUNCTION()
	void Die();

};
