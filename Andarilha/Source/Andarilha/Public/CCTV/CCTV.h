#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCTV.generated.h"

class UBoxComponent;
class USceneCaptureComponent2D;
class TextureRenderTarget2D;

UCLASS()
class ANDARILHA_API ACCTV : public AActor
{
	GENERATED_BODY()
	
public:	
	ACCTV();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnPlayerEnterBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerExitBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* CameraMesh;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* HolderMesh;

	UPROPERTY(VisibleAnywhere, Category = "Scene")
	USceneCaptureComponent2D* SceneCaptureComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scene")
	UTextureRenderTarget2D* RenderTarget;

	UPROPERTY(VisibleAnywhere, Category = "Box")
	UBoxComponent* Box;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boolean")
	bool bCanFollowPlayer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boolean")
	bool bCanRecord;

protected:
	virtual void BeginPlay() override;

	AActor* PlayerToFollow;

	bool bIsPlayerOnSight;
};
