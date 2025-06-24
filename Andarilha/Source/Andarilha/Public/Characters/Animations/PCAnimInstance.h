#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PCAnimInstance.generated.h"

class ACharacter;
class UCharacterMovementComponent;

UCLASS()
class ANDARILHA_API UPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> Character;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float MoveSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float YawRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsFalling;

private:
	void UpdateYawRotation();
};
