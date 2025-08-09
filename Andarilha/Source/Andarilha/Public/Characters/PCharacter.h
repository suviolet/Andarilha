#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interactables/InteractableBase.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Components/SaveSystem/SaveSystemComponent.h"
#include "Components/Stamina/StaminaComponent.h"
#include "PCharacter.generated.h"

class UAnimMontage;
class UCameraComponent;
class UCapsuleComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
struct FInputActionValue;

UCLASS()
class ANDARILHA_API APCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APCharacter();

	//virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION()
	void RestartLastSave();

	UFUNCTION()
	void Run(const FInputActionValue& Value);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "GameOverWidgetClass"))
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UUserWidget* gameOverWidget;

	UCapsuleComponent* Capsule;

	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Component")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SaveSystem Component")
	USaveSystemComponent* SaveSystemComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stamina Component")
	UStaminaComponent* StaminaComponent;

	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* Camera;
	float cameraHeight;
	float cameraDefaultHeight;
	float cameraCrouchedHeight;

	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	TArray<AActor*> ignoreActors;
	TArray<AActor*> overlappedActors;
	UClass* seekClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items Data Table")
	UDataTable* ItemsDataTable;

	float sphereOverlapRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAlive;

	bool isCrouching; // add b
	bool isRunning;  // add b
	bool bCanRun;
	bool isTurnLocked;  // add b

private:


	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> JumpMontage;

	TArray<FName> RowNames;

protected:
	virtual void BeginPlay() override;

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

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveUpUIAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveDownUIAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> UseUIAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> DropUIAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> SaveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> LoadAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> ExitGameAction;

	////////////////////////////////////////// INPUTS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Turn(const FInputActionValue& Value);

	UFUNCTION()
	void LockTurn(const FInputActionValue& Value);

	UFUNCTION()
	void CustomCrouch(const FInputActionValue& Value);

	//UFUNCTION()
	//void Run(const FInputActionValue& Value);

	UFUNCTION()
	void JumpStart();

	UFUNCTION()
	void Start(const FInputActionValue& Value);

	UFUNCTION()
	bool Interaction(bool useSelectedItem = false);

	UFUNCTION()
	void Interact(const FInputActionValue& Value);

	UFUNCTION()
	void UseItem(const FInputActionValue& Value);

	UFUNCTION()
	void Landed(const FHitResult& Hit) override;

	UFUNCTION()
	void ExitGame(const FInputActionValue& Value);

	UFUNCTION()
	void LoadGame(const FInputActionValue& Value);

	UFUNCTION()
	void SaveGame(const FInputActionValue& Value);

};
