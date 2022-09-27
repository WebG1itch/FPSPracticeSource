#include "Character/Abilities/Movement/GrappleHook.h"

#include "Character/ImpulseDefaultCharacter.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

AGrappleHook::AGrappleHook()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	SetReplicatingMovement(true);
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(16.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	CollisionComp->OnComponentHit.AddDynamic(this, &AGrappleHook::OnHitActor);
	CollisionComp->SetIsReplicated(true);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	
	RootComponent = CollisionComp;

	GrappleHookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrappleHookMesh"));
	GrappleHookMesh->SetupAttachment(CollisionComp);
	GrappleHookMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	GrappleHookMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GrappleHookMesh->SetIsReplicated(true);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileComponent->UpdatedComponent = CollisionComp;
	ProjectileComponent->InitialSpeed = ProjectileSpeed;
	ProjectileComponent->MaxSpeed = ProjectileSpeed;
	ProjectileComponent->bRotationFollowsVelocity = true;
	ProjectileComponent->bShouldBounce = false;
	ProjectileComponent->ProjectileGravityScale = 0.f;
	ProjectileComponent->SetIsReplicated(true);

	OnDestroyed.AddDynamic(this, &AGrappleHook::OnDestroyActor);
}

void AGrappleHook::FireGrappleHook(FVector FireDirection)
{
	ProjectileComponent->Velocity = FireDirection * ProjectileSpeed;
}

void AGrappleHook::BeginPlay()
{
	Super::BeginPlay();
}

void AGrappleHook::OnHitActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetOwner())
		GetMyMovementComponent()->OnGrappleHookHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

void AGrappleHook::OnDestroyActor(AActor* DestroyedActor)
{
	if (GetOwner())
		GetMyMovementComponent()->OnGrappleHookDestroyed(DestroyedActor);
}

UMyCharacterMovementComponent* AGrappleHook::GetMyMovementComponent() const
{
	if (AImpulseDefaultCharacter* Character = Cast<AImpulseDefaultCharacter>(GetOwner()))
		return static_cast<UMyCharacterMovementComponent*>(Character->GetCharacterMovement());
	return Cast<UMyCharacterMovementComponent>(GetOwner());
}