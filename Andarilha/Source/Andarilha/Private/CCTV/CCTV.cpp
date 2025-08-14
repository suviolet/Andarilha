#include "CCTV/CCTV.h"
#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetMathLibrary.h"

ACCTV::ACCTV()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;
	RootComponent->SetMobility(EComponentMobility::Static);

	CameraMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraMesh"));
	CameraMesh->SetMobility(EComponentMobility::Movable);
	CameraMesh->SetCollisionProfileName(FName("NoCollision"), false);
	CameraMesh->SetSimulatePhysics(false);
	CameraMesh->SetupAttachment(RootComponent);

	HolderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HolderMesh"));
	HolderMesh->SetCollisionProfileName(FName("NoCollision"), false);
	HolderMesh->SetSimulatePhysics(false);
	HolderMesh->SetMobility(EComponentMobility::Static);
	HolderMesh->SetupAttachment(RootComponent);

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->CaptureSource = SCS_SceneColorHDR;
	SceneCaptureComponent->SetRelativeLocation(FVector(70.f, 0.f, 0.f));
	SceneCaptureComponent->SetupAttachment(CameraMesh);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetMobility(EComponentMobility::Static);
	Box->SetBoxExtent(FVector(300.f, 300.f, 300.f));
	Box->SetRelativeLocation(FVector(230.f, 0.f, 0.f));
	Box->SetupAttachment(RootComponent);

	bCanFollowPlayer = false;
	bCanRecord = false;
	bIsPlayerOnSight = false;

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACCTV::OnPlayerEnterBox);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACCTV::OnPlayerExitBox);
}

void ACCTV::BeginPlay()
{
	Super::BeginPlay();

	if (bCanRecord && RenderTarget != nullptr)
	{
		SceneCaptureComponent->TextureTarget = RenderTarget;
	}
}

void ACCTV::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanFollowPlayer && bIsPlayerOnSight && PlayerToFollow != nullptr)
	{
		FRotator rotation = (PlayerToFollow->GetActorLocation() - CameraMesh->GetComponentLocation()).Rotation();
		CameraMesh->SetWorldRotation(rotation);
	}
}

void ACCTV::OnPlayerEnterBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//if (APCharacter* PCharacter = Cast<APCharacter>(OtherActor))
		if (OtherActor->ActorHasTag(FName("PCharacter")))
		{
			bIsPlayerOnSight = true;
			PlayerToFollow = OtherActor;
		}
	}
}

void ACCTV::OnPlayerExitBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//if (APCharacter* PCharacter = Cast<APCharacter>(OtherActor))
		if (OtherActor->ActorHasTag(FName("PCharacter")))
		{
			bIsPlayerOnSight = false;
		}
	}
}