#include "GameModes/PracticeRange/Gridshot/GridshotBall.h"

#include "Components/SphereComponent.h"
#include "GameModes/PracticeRange/Gridshot/BeginGridshot.h"

// Sets default values
AGridshotBall::AGridshotBall()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	SetReplicatingMovement(true);
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(16.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("WorldStatic");
	CollisionComp->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	CollisionComp->SetIsReplicated(true);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	
	RootComponent = CollisionComp;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(CollisionComp);
	BallMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	BallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BallMesh->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AGridshotBall::BeginPlay()
{
	Super::BeginPlay();	
}

