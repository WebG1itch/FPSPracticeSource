#include "Character/Abilities/Movement/GrappleHookCable.h"

#include "Kismet/KismetMathLibrary.h"

AGrappleHookCable::AGrappleHookCable()
{
	bReplicates = true;
	SetReplicatingMovement(true);

	GrappleCableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrappleHookMesh"));
	GrappleCableMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	GrappleCableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GrappleCableMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	GrappleCableMesh->bIgnoreRadialImpulse = true;
	GrappleCableMesh->bIgnoreRadialForce = true;
	GrappleCableMesh->SetEnableGravity(false);
	GrappleCableMesh->SetIsReplicated(true);

	RootComponent = GrappleCableMesh;
}

void AGrappleHookCable::BeginPlay()
{
	Super::BeginPlay();
}

void AGrappleHookCable::FollowGrappleHook(AGrappleHook* GrappleHook, FVector CableStart)
{
	if (GrappleHook)
	{
		FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), GrappleHook->GetActorLocation());
		LookRotation.Pitch += 90.f;
		SetActorRotation(LookRotation);
		SetActorLocation(GrappleHook->GetActorLocation());
		
		//const FVector MiddleLocation = FVector((GetOwner()->GetActorLocation().X + GrappleHook->GetActorLocation().X) / 2, (GetOwner()->GetActorLocation().Y + GrappleHook->GetActorLocation().Y) / 2, (GetOwner()->GetActorLocation().Z + GrappleHook->GetActorLocation().Z) / 2);
		/*FVector MiddleLocation = FVector(GetOwner()->GetActorLocation());
		SetActorLocation(MiddleLocation);*/
		
		const float Difference = UKismetMathLibrary::Vector_Distance(GetOwner()->GetActorLocation(), GrappleHook->GetActorLocation());
		//SetActorRelativeScale3D(FVector(1.f, 1.f, Difference / 100.f));
		SetActorRelativeScale3D(FVector(1.f, 1.f, Difference / 66.f));
	}
}
