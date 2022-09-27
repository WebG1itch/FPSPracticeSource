// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Abilities/AbilityBase.h"
#include "Character/ImpulseDefaultCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "ImpulseGrenade.generated.h"

class USphereComponent;
class URadialForceComponent;
class USoundCue;
class UAudioComponent;

UCLASS()
class IMPULSE_API AImpulseGrenade : public AAbilityBase
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* ImpulseGrenadeHitBox;

	UPROPERTY(EditDefaultsOnly)
	URadialForceComponent* RadialForceComponent;

	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* ProjectileComponent;
	
	UPROPERTY(EditDefaultsOnly)
	float ThrowForce = 4000.f;

	UPROPERTY(EditDefaultsOnly)
	float ImpulseRadius = 1000.f;

	UPROPERTY(EditDefaultsOnly)
	float ImpulseStrength = 250000.f;

	UPROPERTY(EditDefaultsOnly)
	float ImpulseDelay = 0.75f;
	
	UPROPERTY(EditDefaultsOnly)
	USoundCue* HitSoundCue;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* DetonationSoundCue;

private:
	
	UPROPERTY()
	UAudioComponent* AudioComponent;
	
public:	
	
	AImpulseGrenade();

	virtual void BeginAbility(AImpulseDefaultCharacter* Player, FVector LookRotation) override;
	
	virtual void OnHitActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	
	virtual void BeginPlay() override;
	
	void FireImpulse();

	void DestroyImpulseGrenade();

	UFUNCTION(Server, Unreliable)
	void ServerOnExplosion();

	UFUNCTION(NetMulticast, Unreliable)
	void MultiOnExplosion();

};


