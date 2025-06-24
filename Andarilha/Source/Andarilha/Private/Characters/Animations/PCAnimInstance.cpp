#include "Characters/Animations/PCAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPCAnimInstance::NativeInitializeAnimation()
{
	APawn* CharacterPawn = TryGetPawnOwner();
	if (CharacterPawn)
	{
		Character = Cast<ACharacter>(CharacterPawn);
		if (Character)
		{
			MovementComponent = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
		}
	}
}

void UPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (MovementComponent)
	{
		MoveSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
		bIsFalling = MovementComponent->IsFalling();
		UpdateYawRotation();
	}
}

FORCEINLINE void UPCAnimInstance::UpdateYawRotation()
{
	if (Character)
	{
		const FVector CurrentFacingDirection = Character->GetActorForwardVector();
		FVector CurrentVelocityDirection = MovementComponent->Velocity;
		CurrentVelocityDirection.Normalize();

		const float DotProduct = FVector::DotProduct(CurrentFacingDirection, CurrentVelocityDirection);
		YawRotation = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

		const FVector CrossProduct = FVector::CrossProduct(CurrentFacingDirection, CurrentVelocityDirection);

		YawRotation = CrossProduct.Z < 0 ? -YawRotation : YawRotation;
	}
}