
#include "GameModes/PracticeRange/Gridshot/BeginGridshot.h"

#include "Character/ImpulseDefaultCharacter.h"
#include "Components/BoxComponent.h"
#include "GameModes/PracticeRange/Gridshot/GridshotBall.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABeginGridshot::ABeginGridshot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;
	
	StartHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StartHitBox"));
	StartHitBox->SetupAttachment(StaticMeshComponent);

	StartHitBox->OnComponentHit.AddDynamic(this, &ABeginGridshot::HitStartComp);
	
}

// Called when the game starts or when spawned
void ABeginGridshot::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABeginGridshot::HitMC(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!SelfActor->IsHidden())
	{
		Score++;
		SelfActor->SetHidden(true);
		UnHideRandomBall(OtherActor);
	}
}

void ABeginGridshot::UnHideRandomBall(AActor* OtherActor)
{
	if (OtherActor->GetLocalRole() > ROLE_SimulatedProxy)
	{
		if (MiddleCenterBall && MiddleLeftBall && MiddleRightBall)
		{
			if (TopCenterBall && TopLeftBall && TopRightBall)
			{
				if (BottomCenterBall && BottomLeftBall && BottomRightBall)
				{
					UE_LOG(LogTemp, Warning, TEXT("Score: %1.f"), Score);
					int InitialVisibilityIndex = FMath::RandRange(1.f, 9.f);
					switch(InitialVisibilityIndex)
					{
					case(1):
						{
							BottomLeftBall->SetHidden(false);
							break;
						}
					case(2):
						{
							BottomCenterBall->SetHidden(false);
							break;
						}
					case(3):
						{
							BottomRightBall->SetHidden(false);
							break;
						}
					case(4):
						{
							MiddleLeftBall->SetHidden(false);
							break;
						}
					case(5):
						{
							MiddleCenterBall->SetHidden(false);
							break;
						}
					case(6):
						{
							MiddleRightBall->SetHidden(false);
							break;
						}
					case(7):
						{
							TopLeftBall->SetHidden(false);
							break;
						}
					case(8):
						{
							TopCenterBall->SetHidden(false);
							break;
						}
					case(9):
						{
							TopRightBall->SetHidden(false);
							break;
						}
					default:
						{
							UE_LOG(LogTemp, Warning, TEXT("UNABLE TO SPAWN A GRIDSHOT BALL"));
						}
					}
				}
			}
		}
	}
}

void ABeginGridshot::HitML(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!SelfActor->IsHidden())
	{
		Score++;
		SelfActor->SetHidden(true);
		UnHideRandomBall(OtherActor);
	}
}

void ABeginGridshot::HitMR(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!SelfActor->IsHidden())
	{
		Score++;
		SelfActor->SetHidden(true);
		UnHideRandomBall(OtherActor);
	}
}

void ABeginGridshot::HitTC(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!SelfActor->IsHidden())
	{
		Score++;
		SelfActor->SetHidden(true);
		UnHideRandomBall(OtherActor);
	}
}

void ABeginGridshot::HitTL(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!SelfActor->IsHidden())
	{
		Score++;
		SelfActor->SetHidden(true);
		UnHideRandomBall(OtherActor);
	}
}

void ABeginGridshot::HitTR(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!SelfActor->IsHidden())
	{
		Score++;
		SelfActor->SetHidden(true);
		UnHideRandomBall(OtherActor);
	}
}

void ABeginGridshot::HitBC(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!SelfActor->IsHidden())
	{
		Score++;
		SelfActor->SetHidden(true);
		UnHideRandomBall(OtherActor);
	}
}

void ABeginGridshot::HitBL(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!SelfActor->IsHidden())
	{
		Score++;
		SelfActor->SetHidden(true);
		UnHideRandomBall(OtherActor);
	}
}

void ABeginGridshot::HitBR(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!SelfActor->IsHidden())
	{
		Score++;
		SelfActor->SetHidden(true);
		UnHideRandomBall(OtherActor);
	}
}

void ABeginGridshot::HitStart(AActor* Player)
{
	if (Player)
		if(Player->GetLocalRole() > ROLE_SimulatedProxy)
			if (!IsPlaying)
			{
				IsPlaying = true;
				ServerSpawnBall();
			}		
}

void ABeginGridshot::HitStartComp(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("called"))
	if (OtherActor)
		if (OtherActor->GetLocalRole() > ROLE_SimulatedProxy)
			if (!IsPlaying)
			{
				UE_LOG(LogTemp, Warning, TEXT("sserver"))
				IsPlaying = true;
				ServerSpawnBall();
			}
}

void ABeginGridshot::ServerSpawnBall_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("playing"))
	// Middle Row
	const FVector MiddleCenterBallLocation = StartHitBox->GetComponentLocation() + UKismetMathLibrary::TransformDirection(StartHitBox->GetComponentTransform(), FVector(DistanceOffset, MiddleColumnOffset, MiddleRowOffset));
	const FVector MiddleLeftBallLocation = StartHitBox->GetComponentLocation() + UKismetMathLibrary::TransformDirection(StartHitBox->GetComponentTransform(), FVector(DistanceOffset, MiddleColumnOffset + LeftColumnOffset, MiddleRowOffset));
	const FVector MiddleRightBallLocation = StartHitBox->GetComponentLocation() + UKismetMathLibrary::TransformDirection(StartHitBox->GetComponentTransform(), FVector(DistanceOffset, MiddleColumnOffset + RightColumnOffset, MiddleRowOffset));

	// Top Row
	FVector TopCenterBallLocation = MiddleCenterBallLocation;
	TopCenterBallLocation.Z += TopRowOffset;
	FVector TopLeftBallLocation = MiddleLeftBallLocation;
	TopLeftBallLocation.Z += TopRowOffset;
	FVector TopRightBallLocation = MiddleRightBallLocation;
	TopRightBallLocation.Z += TopRowOffset;

	// Bottom Row
	FVector BottomCenterBallLocation = MiddleCenterBallLocation;
	BottomCenterBallLocation.Z += BottomRowOffset;
	FVector BottomLeftBallLocation = MiddleLeftBallLocation;
	BottomLeftBallLocation.Z += BottomRowOffset;
	FVector BottomRightBallLocation = MiddleRightBallLocation;
	BottomRightBallLocation.Z += BottomRowOffset;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	// Spawn middle row
	MiddleCenterBall = GetWorld()->SpawnActor<AGridshotBall>(BallClass, MiddleCenterBallLocation, FRotator(0.f, 0.f, 0.f), SpawnParams);
	MiddleLeftBall = GetWorld()->SpawnActor<AGridshotBall>(BallClass, MiddleLeftBallLocation, FRotator(0.f, 0.f, 0.f), SpawnParams);
	MiddleRightBall = GetWorld()->SpawnActor<AGridshotBall>(BallClass, MiddleRightBallLocation, FRotator(0.f, 0.f, 0.f), SpawnParams);
	
	// Spawn top row
	TopCenterBall = GetWorld()->SpawnActor<AGridshotBall>(BallClass, TopCenterBallLocation, FRotator(0.f, 0.f, 0.f), SpawnParams);
	TopLeftBall = GetWorld()->SpawnActor<AGridshotBall>(BallClass, TopLeftBallLocation, FRotator(0.f, 0.f, 0.f), SpawnParams);
	TopRightBall = GetWorld()->SpawnActor<AGridshotBall>(BallClass, TopRightBallLocation, FRotator(0.f, 0.f, 0.f), SpawnParams);
	
	// Spawn bottom row
	BottomCenterBall = GetWorld()->SpawnActor<AGridshotBall>(BallClass, BottomCenterBallLocation, FRotator(0.f, 0.f, 0.f), SpawnParams);
	BottomLeftBall = GetWorld()->SpawnActor<AGridshotBall>(BallClass, BottomLeftBallLocation, FRotator(0.f, 0.f, 0.f), SpawnParams);
	BottomRightBall = GetWorld()->SpawnActor<AGridshotBall>(BallClass, BottomRightBallLocation, FRotator(0.f, 0.f, 0.f), SpawnParams);
	
	// Hide middle row
	MiddleCenterBall->SetHidden(false);
	MiddleLeftBall->SetHidden(true);
	MiddleRightBall->SetHidden(true);

	// Hide Top row
	TopCenterBall->SetHidden(true);
	TopLeftBall->SetHidden(true);
	TopRightBall->SetHidden(true);

	// Hide bottom row
	BottomCenterBall->SetHidden(true);
	BottomLeftBall->SetHidden(true);
	BottomRightBall->SetHidden(true);

	MiddleCenterBall->OnActorHit.AddDynamic(this, &ABeginGridshot::HitMC);
	MiddleLeftBall->OnActorHit.AddDynamic(this, &ABeginGridshot::HitML);
	MiddleRightBall->OnActorHit.AddDynamic(this, &ABeginGridshot::HitMR);
	
	TopCenterBall->OnActorHit.AddDynamic(this, &ABeginGridshot::HitTC);
	TopLeftBall->OnActorHit.AddDynamic(this, &ABeginGridshot::HitTL);
	TopRightBall->OnActorHit.AddDynamic(this, &ABeginGridshot::HitTR);

	BottomCenterBall->OnActorHit.AddDynamic(this, &ABeginGridshot::HitBC);
	BottomLeftBall->OnActorHit.AddDynamic(this, &ABeginGridshot::HitBL);
	BottomRightBall->OnActorHit.AddDynamic(this, &ABeginGridshot::HitBR);
}

bool ABeginGridshot::ServerSpawnBall_Validate()
{
	return true;
}

