#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class AAIController;
class USkeletalMeshComponent;
struct FAIRequestID;
struct FPathFollowingResult;

UCLASS()
class ANDARILHA_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SkeletalMesh")
	USkeletalMeshComponent* HeadSKMComponent;

private:
	TObjectPtr<AActor> Target;
	TObjectPtr<AAIController> EnemyController;

	void OnMoveComplete(FAIRequestID RequestID, const FPathFollowingResult& Result);

	void Attack();
	void ChaseTarget();

	//UPROPERTY(EditAnywhere, Category = "Combat")
	//TObjectPtr<UAnimMontage> CombatMontage;

	//UPROPERTY(EditAnywhere, Category = "Combat")
	//TArray<FName> AttackNames;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackCooldown = 2.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AggroRadius = 800.f;
	float FieldOfView = 0.6f;

	bool bAggressive = false;

	FTimerHandle AttackCooldownTimer;
};
