
#include "Character/Weapons/WeaponFull.h"

#include "Character/ImpulseDefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

AWeaponFull::AWeaponFull()
{
	MaxAmmo = 30;
	bIsInFullAuto = true;
	bIsSelectFire = false;
	FireRate = 540; //540 is soldier76
}

void AWeaponFull::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;

	FireRate = 60 / FireRate;
}

void AWeaponFull::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWeaponFull, bIsFiring, COND_SkipOwner);
}

void AWeaponFull::Server_StartFullAuto_Implementation(const bool IsFiring)
{
	bIsFiring = IsFiring;
	OnRep_StartFullAuto();
}

bool AWeaponFull::Server_StartFullAuto_Validate(bool IsFiring)
{
	return true;
}

void AWeaponFull::OnRep_StartFullAuto()
{
	if(bIsFiring)
	{
		GetWorld()->GetTimerManager().SetTimer(WeaponFireHandle, this, &AWeaponFull::PlayWeaponEffects, FireRate, true);
		PlayWeaponEffects();
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(WeaponFireHandle);
	}
	
}

void AWeaponFull::PlayWeaponEffects() const
{
	if (OwningPlayer)
	{
		if(!OwningPlayer->IsLocallyControlled() && FireAnimation)
		{
			if(UAnimInstance* AnimInstance = OwningPlayer->GetMesh()->GetAnimInstance())
			{
				if(ThirdPersonMontage)
				{
					AnimInstance->Montage_Play(ThirdPersonMontage);
					AnimInstance->Montage_JumpToSection(FName("FireHip"), ThirdPersonMontage);
				}
			}
			WeaponMesh->PlayAnimation(FireAnimation, false);
		}
	}
}

void AWeaponFull::ServerFire_Implementation(const TArray<FHitResult>& HitResults, bool bSubAmmo)
{
	if(CurrentAmmo > 0)
	{
		Super::ServerFire_Implementation(HitResults, true);
	}
}

void AWeaponFull::OnClientFire()
{
	if(CurrentAmmo > 0)
	{
		--CurrentAmmo;
		if(UAnimInstance* AnimInstance = OwningPlayer->GetMesh1P()->GetAnimInstance())
		{
			if(FPSArmsFireMontage)
			{
				AnimInstance->Montage_Play(FPSArmsFireMontage);
			}
		}
		if(FireAnimation)
		{
			WeaponMesh->PlayAnimation(FireAnimation, false);
		}
		TArray<FHitResult> HitResults = PerformLineTrace(OwningPlayer);
		if(HitResults.Num() > 0)
		{
			for(FHitResult& Result : HitResults)
			{
				if(AActor* HitActor = Result.GetActor())
				{
					if(AImpulseDefaultCharacter* HitPlayer = Cast<AImpulseDefaultCharacter>(HitActor))
					{
						FString HitBone = Result.BoneName.ToString();
						UE_LOG(LogTemp, Warning, TEXT("HIT BONE %s ON CLIENT"), *HitBone);
						EHitLocation HitLocation;
						if(HitBone == FString("Head"))
						{
							HitLocation = EHitLocation::Head;
						}
						else
						{
							HitLocation = EHitLocation::None;
						}
						if(AWeaponBase* OwnerWeapon = OwningPlayer->GetCurrentWeapon())
						{
							const float Damage = OwnerWeapon->GetWeaponDamage().GetDamage(HitLocation);
							UGameplayStatics::ApplyPointDamage(HitPlayer, Damage, OwningPlayer->GetFirstPersonCameraComponent()->GetComponentRotation().Vector(), Result, GetOwner()->GetInstigatorController(), this, TypeOfDamage);
						}
					}
				}
			}
			if(!GetWorld()->IsNetMode(NM_DedicatedServer))
				ServerFire(HitResults, true);
		}
		
		OwningPlayer->UpdateHUDAmmo(CurrentAmmo, MaxAmmo);
	}
	else
	{
		if (CurrentAmmo == 0)
			Reload();
		StopFiring();
	}
}

void AWeaponFull::Fire()
{
	if(!bIsFiring && bIsInFullAuto && bCanFire && !bIsReloading) //full auto
	{
		bIsFiring = true;
		
		bCanFire = false;
		FTimerHandle FireRateHandle;
		GetWorld()->GetTimerManager().SetTimer(FireRateHandle, this, &AWeaponFull::ControlFireDelay, FireRate, false);

		OnClientFire();
		GetWorld()->GetTimerManager().SetTimer(WeaponFireHandle, this, &AWeaponFull::OnClientFire, FireRate, true);

		if(!GetWorld()->IsNetMode(NM_DedicatedServer))
			Server_StartFullAuto(bIsFiring);
	}
	else //semi auto
	{
		Super::Fire();
	}
}

void AWeaponFull::StopFiring()
{
	GetWorld()->GetTimerManager().ClearTimer(WeaponFireHandle);
	bIsFiring = false;
	
	if(!GetWorld()->IsNetMode(NM_DedicatedServer))
		Server_StartFullAuto(bIsFiring);
}

void AWeaponFull::ChangeFireMode()
{
	if(bIsSelectFire)
		bIsInFullAuto = !bIsInFullAuto;
}

