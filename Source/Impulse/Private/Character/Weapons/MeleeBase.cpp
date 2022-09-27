
#include "Character/Weapons/MeleeBase.h"

#include "Character/ImpulseDefaultCharacter.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CollisionShape.h"

AMeleeBase::AMeleeBase()
{
	MeleeMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeleeMesh;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	CollisionComponent->SetupAttachment(MeleeMesh);

	//AImpulseDefaultCharacter* Player = Cast<AImpulseDefaultCharacter>(GetOwner());
	//CollisionComponent->SetupAttachment(Player->GetFirstPersonCameraComponent());

	CollisionComponent->SetRelativeLocation(FVector(60.0f, 0.0f, -30.0f));
	CollisionComponent->InitBoxExtent(FVector(60.0f, 32.0f, 32.0f));
	CollisionComponent->SetHiddenInGame(true);
	
	DelayBetweenMelee = 1.f;
	bCanMelee = true;
	
	bReplicates = true;
}

void AMeleeBase::BeginPlay()
{
	Super::BeginPlay();
	
	MeleeMesh->SetHiddenInGame(false);
	MeleeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

FHitResult AMeleeBase::PerformBoxTrace(const AImpulseDefaultCharacter* ShootingPlayer) const
{
	FHitResult HitResult;	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(ShootingPlayer);

	FCollisionResponseParams ResponseParams;
	
	UE_LOG(LogTemp, Warning, TEXT("PERFORMING BOX TRACE"));
	
	GetWorld()->SweepSingleByChannel(OUT HitResult, CollisionComponent->GetComponentLocation(), CollisionComponent->GetComponentLocation(), CollisionComponent->GetComponentRotation().Quaternion(), ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeBox(FVector(CollisionComponent->GetScaledBoxExtent())), CollisionParams, ResponseParams);
	
	
	//DrawDebugBox(GetWorld(), (CollisionComponent->GetComponentRotation().Vector() * 100.f + CollisionComponent->GetComponentLocation()), FVector(CollisionComponent->GetComponentRotation().GetComponentForAxis(EAxis::X) * 100.f, CollisionComponent->GetComponentRotation().GetComponentForAxis(EAxis::Y) * 32.f, CollisionComponent->GetComponentRotation().GetComponentForAxis(EAxis::Z) * 32.f), FColor::Green, false, 2.f, 0, 3.f);
	
	return HitResult;	
}

void AMeleeBase::MultiMelee_Implementation(FHitResult HitResult)
{
	if(AImpulseDefaultCharacter* Player = Cast<AImpulseDefaultCharacter>(GetOwner()))
	{
		if(!Player->IsLocallyControlled())
		{
			if(TPSMeleeMontage)
			{
				if(UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance())
				{
					AnimInstance->Montage_Play(TPSMeleeMontage);
				}
			}
		}
	}
}

bool AMeleeBase::MultiMelee_Validate(FHitResult HitResult)
{
	return true;
}

void AMeleeBase::ServerMelee_Implementation(FHitResult HitResult)
{
	if(AImpulseDefaultCharacter* Player = Cast<AImpulseDefaultCharacter>(GetOwner()))
	{
		if(AActor* HitActor = HitResult.GetActor())
		{
			if(AImpulseDefaultCharacter* HitPlayer = Cast<AImpulseDefaultCharacter>(HitActor))
			{
				UE_LOG(LogTemp, Warning, TEXT("HIT PLAYER ON SERVER"));
				UGameplayStatics::ApplyPointDamage(HitPlayer, 30.f, Player->GetFirstPersonCameraComponent()->GetComponentRotation().Vector(), HitResult, GetOwner()->GetInstigatorController(), this, TypeOfDamage);
			}
		}
		MultiMelee(HitResult);
	}
}

bool AMeleeBase::ServerMelee_Validate(FHitResult HitResult)
{
	return true;
}

void AMeleeBase::ControlMeleeDelay()
{
	bCanMelee = true;
}

void AMeleeBase::OnMelee()
{
	if(AImpulseDefaultCharacter* Player = Cast<AImpulseDefaultCharacter>(GetOwner()))
	{
		if(bCanMelee)
		{
			if(UAnimInstance* AnimInstance = Player->GetMesh1P()->GetAnimInstance())
			{
				if(FPSMeleeMontage)
					AnimInstance->Montage_Play(FPSMeleeMontage);
			}

			bCanMelee = false;
		
			FHitResult HitResult = PerformBoxTrace(Player);
		
			if(AActor* HitActor = HitResult.GetActor())
			{
				if(AImpulseDefaultCharacter* HitPlayer = Cast<AImpulseDefaultCharacter>(HitActor))
				{
					UE_LOG(LogTemp, Warning, TEXT("HIT PLAYER ON CLIENT"));
				}
			}
		
			if(GetWorld()->IsNetMode(NM_DedicatedServer))
				MultiMelee(HitResult);
			else
				ServerMelee(HitResult);

			FTimerHandle MeleeRateHandle;
			GetWorld()->GetTimerManager().SetTimer(MeleeRateHandle, this, &AMeleeBase::ControlMeleeDelay, DelayBetweenMelee, false);
		}
	}
}






