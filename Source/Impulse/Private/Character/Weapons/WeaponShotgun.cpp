// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Weapons/WeaponShotgun.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AWeaponShotgun::AWeaponShotgun()
{
	MaxAmmo = 10.f;
	BulletAngle = 5.f;
	NumPellets = 20.f;
	DelayBetweenShoots = 1.f;
}

void AWeaponShotgun::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;
}

FVector2D AWeaponShotgun::RandomPointInCircle(float Radius)
{
	const float Angle = UKismetMathLibrary::RandomFloatInRange(0.f, 360.f);
	const float DistanceFromCenter = UKismetMathLibrary::RandomFloatInRange(0.f, Radius);
	const float X = DistanceFromCenter * UKismetMathLibrary::DegCos(Angle);
	const float Y = DistanceFromCenter * UKismetMathLibrary::DegSin(Angle);
	return FVector2d(X, Y);
}

TArray<FHitResult> AWeaponShotgun::PerformShotgunLineTrace(AImpulseDefaultCharacter* ShootingPlayer, FVector2D Point)
{
	float X = Point.X;
	float Y = Point.Y;
	
	FVector CameraLocation = ShootingPlayer->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector CameraForward = ShootingPlayer->GetFirstPersonCameraComponent()->GetForwardVector();
	FVector CameraRight = ShootingPlayer->GetFirstPersonCameraComponent()->GetRightVector();
	FVector CameraUp = ShootingPlayer->GetFirstPersonCameraComponent()->GetUpVector();
	
	FVector FirstAdd = CameraLocation + (CameraForward * BulletDistance);
	FVector SecondAdd = CameraRight * X;
	FVector ThirdAdd = CameraUp * Y;
	FVector EndLocation = FirstAdd + SecondAdd + ThirdAdd;

	TArray<FHitResult> HitResults;
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(ShootingPlayer);

	FCollisionResponseParams CollisionResponse;
	
	GetWorld()->LineTraceMultiByChannel(OUT HitResults, CameraLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams, CollisionResponse);

	//DrawDebugLine(GetWorld(), CameraLocation, EndLocation, FColor::Red, false, 2.f, 0, 3.f);

	FVector SocketLocation = WeaponMesh->GetSocketLocation(FName("MuzzleFlash"));
	FVector EndTrace = EndLocation;
	if (HitResults.Num() > 0)
		EndTrace = HitResults[0].ImpactPoint;
	FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(SocketLocation, EndTrace);
	FActorSpawnParameters SpawnParameters;
	
	GetWorld()->SpawnActor(WeaponTracer, &SocketLocation, &LookRotation, SpawnParameters); //do on client instantly
	
	ServerWeaponTrace(SocketLocation, LookRotation);
	
	return HitResults;
}

void AWeaponShotgun::ServerUpdateShotgunAmmo_Implementation()
{
	if (CurrentAmmo > 0)
		CurrentAmmo--;
}

bool AWeaponShotgun::ServerUpdateShotgunAmmo_Validate()
{
	return true;
}

void AWeaponShotgun::Fire()
{	
	if (AImpulseDefaultCharacter* ShootingPlayer = Cast<AImpulseDefaultCharacter>(GetOwner()))
	{
		if (CurrentAmmo > 0 && bCanFire && !bIsReloading)
		{
			if(UAnimInstance* AnimInstance = ShootingPlayer->GetMesh1P()->GetAnimInstance())
			{
				if(FPSArmsFireMontage)
					AnimInstance->Montage_Play(FPSArmsFireMontage);
			}
			
			--CurrentAmmo;
			
			bCanFire = false;
			bCanReload = false;
			bIsFiring = true;

			if(FireAnimation)
			{
				WeaponMesh->PlayAnimation(FireAnimation, false);
			}
			
			const float Radius = UKismetMathLibrary::DegTan(BulletAngle) * BulletDistance;
			
			if (!GetWorld()->IsNetMode(NM_DedicatedServer))
				ServerUpdateShotgunAmmo();
			
			for(int32 i = 1; i <= NumPellets; i++)
			{
				const FVector2d Point = RandomPointInCircle(Radius);
				TArray<FHitResult> HitResults = PerformShotgunLineTrace(ShootingPlayer, Point);
				
				if(GetWorld()->IsNetMode(NM_DedicatedServer))
				{
					if(HitResults.Num() > 0)
						MultiFire(HitResults[0]);
					else
						MultiFire(FHitResult());
				}
				else
				{
					ServerFire(HitResults, false);
				}
			}

			FTimerHandle FireRateHandle;
			GetWorld()->GetTimerManager().SetTimer(FireRateHandle, this, &AWeaponShotgun::ControlFireDelay, DelayBetweenShoots, false);
			
			OwningPlayer->UpdateHUDAmmo(CurrentAmmo, MaxAmmo);
		}
		else
		{
			if (CurrentAmmo == 0)
				Reload();
			
			StopFiring();
		}
	}
}
