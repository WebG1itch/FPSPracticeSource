// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Abilities/Utility/ImpulseGrenade.h"

#include "Camera/CameraComponent.h"
#include "Character/ImpulseDefaultCharacter.h"
#include "Character/Weapons/WeaponBase.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AImpulseGrenade::AImpulseGrenade()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// Use a sphere as a simple collision representation
	ImpulseGrenadeHitBox = CreateDefaultSubobject<USphereComponent>(TEXT("ImpulseGrenadeHitbox"));
	ImpulseGrenadeHitBox->InitSphereRadius(13.0f);
	ImpulseGrenadeHitBox->BodyInstance.SetCollisionProfileName("Projectile");
	ImpulseGrenadeHitBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Ignore);
	ImpulseGrenadeHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	ImpulseGrenadeHitBox->OnComponentHit.AddDynamic(this, &AImpulseGrenade::OnHitActor);		// set up a notification for when this component hits something blocking
	ImpulseGrenadeHitBox->SetIsReplicated(true);

	// Players can't walk on it
	ImpulseGrenadeHitBox->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	ImpulseGrenadeHitBox->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = ImpulseGrenadeHitBox;

	// Visual grenade mesh with no collision
	UtilityMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UtilityMesh"));
	UtilityMesh->SetupAttachment(ImpulseGrenadeHitBox);
	UtilityMesh->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
	UtilityMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UtilityMesh->SetIsReplicated(true);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileComponent->UpdatedComponent = ImpulseGrenadeHitBox;
	ProjectileComponent->InitialSpeed = ThrowForce;
	ProjectileComponent->MaxSpeed = ThrowForce;
	ProjectileComponent->bRotationFollowsVelocity = true;
	ProjectileComponent->bShouldBounce = false;
	ProjectileComponent->SetIsReplicated(true);

	// Radial force component to fire impulse on hit
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->ForceStrength = 0.f;
	RadialForceComponent->Radius = ImpulseRadius;
	RadialForceComponent->ImpulseStrength = ImpulseStrength;

	// Audio component for playing hit and detonation sounds
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ImpulseGrenadeAudioComponent"));
	AudioComponent->bAutoActivate = false;
	
	AudioComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AImpulseGrenade::BeginPlay()
{
	Super::BeginPlay();
	UtilityMesh->SetVisibility(true);
	if (AudioComponent->IsValidLowLevel())
		AudioComponent->SetSound(HitSoundCue);
}

void AImpulseGrenade::BeginAbility(AImpulseDefaultCharacter* Player,  FVector LookRotation)
{
	//ImpulseGrenadeHitBox->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform); ATTACH TO HAND FOR ANIMATION AND DETACH BEFORE THROWING
	
	LookRotation.Z += 0.05f;
	LookRotation = LookRotation.GetSafeNormal();
	ProjectileComponent->Velocity = LookRotation * ThrowForce;
	
	SetLifeSpan(20.f);
}

void AImpulseGrenade::OnHitActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{		
	if (GetOwner()->GetLocalRole() > ROLE_SimulatedProxy)
		ServerOnExplosion();
	
	FTimerHandle ImpulseTrigger;
	GetWorld()->GetTimerManager().SetTimer(ImpulseTrigger, this, &AImpulseGrenade::FireImpulse, ImpulseDelay, false);
}

void AImpulseGrenade::FireImpulse()
{
	AudioComponent->SetSound(DetonationSoundCue);
	
	if (GetOwner()->GetLocalRole() > ROLE_SimulatedProxy)
		ServerOnExplosion();
	
	RadialForceComponent->FireImpulse();
	UtilityMesh->SetVisibility(false);
	
	FTimerHandle DestroyImpulseGrenade;
	GetWorld()->GetTimerManager().SetTimer(DestroyImpulseGrenade, this, &AImpulseGrenade::DestroyImpulseGrenade, 1.f, false); //ends sound if destroyed immediately ??
}

void AImpulseGrenade::DestroyImpulseGrenade()
{
	Destroy();
}

void AImpulseGrenade::MultiOnExplosion_Implementation()
{
	if (AudioComponent)
		AudioComponent->Play(0.f);
}

void AImpulseGrenade::ServerOnExplosion_Implementation()
{
	MultiOnExplosion();
}
