#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GrappleHook.generated.h"

class UMyCharacterMovementComponent;
UCLASS()
class IMPULSE_API AGrappleHook : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* GrappleHookMesh;

	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpeed = 5000.f;
	
public:

	AGrappleHook();

	void FireGrappleHook(FVector FireDirection);
	

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHitActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnDestroyActor(AActor* DestroyedActor);

	UFUNCTION()
	UMyCharacterMovementComponent* GetMyMovementComponent() const;
};
