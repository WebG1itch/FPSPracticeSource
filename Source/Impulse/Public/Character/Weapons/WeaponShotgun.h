// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Weapons/WeaponBase.h"
#include "Math/Vector2D.h"
#include "WeaponShotgun.generated.h"

UCLASS()
class IMPULSE_API AWeaponShotgun : public AWeaponBase
{
	GENERATED_BODY()

public:
	
	AWeaponShotgun();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	float BulletAngle;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	int32 NumPellets;

	UFUNCTION()
	FVector2D RandomPointInCircle(float Radius);

	UFUNCTION()
	TArray<FHitResult> PerformShotgunLineTrace(AImpulseDefaultCharacter* ShootingPlayer, FVector2D Point);
	
	bool bIsFiring;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUpdateShotgunAmmo();
	
public:

	virtual void Fire() override;
};
