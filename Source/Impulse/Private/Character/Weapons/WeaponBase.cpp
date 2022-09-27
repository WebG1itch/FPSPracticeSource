
#include "Character/Weapons/WeaponBase.h"

#include "Character/ImpulseDefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameModes/PracticeRange/Gridshot/BeginGridshot.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = WeaponMesh;

	//SetReplicates(true);
	bReplicates = true;

	//DEFAULTS
	WeaponName = "Default Name";
	DelayBetweenShoots = 0.5;
	BulletDistance = 20000.f;
	
	MaxAmmo = 25;
	CurrentAmmo = MaxAmmo;
	
	bCanFire = true;
	bCanReload = true;
	bIsFiring = false;
	bIsReloading = false;

	WeaponMesh->SetHiddenInGame(true);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	OwningPlayer = Cast<AImpulseDefaultCharacter>(GetOwner());
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWeaponBase, CurrentAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AWeaponBase, MaxAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AWeaponBase, WeaponTracer, COND_SkipOwner);
}

TArray<FHitResult> AWeaponBase::PerformLineTrace(AImpulseDefaultCharacter* ShootingPlayer) const
{
	FVector CameraLocation = ShootingPlayer->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector CameraRotation = ShootingPlayer->GetFirstPersonCameraComponent()->GetComponentRotation().Vector();
	FVector End = CameraLocation + CameraRotation * BulletDistance;

	TArray<FHitResult> HitResults;	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(ShootingPlayer);

	FCollisionResponseParams CollisionResponse;

	GetWorld()->LineTraceMultiByChannel(OUT HitResults, CameraLocation, End, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams, CollisionResponse);

	if (HitResults.Num() > 0)
	{
		if (AActor* GridShotActor = HitResults[0].GetActor())
		{
			if (ABeginGridshot* GridStart = Cast<ABeginGridshot>(GridShotActor))
			{
				//GridStart->HitStart(ShootingPlayer);				
			}
		}
	}
	FVector SocketLocation = WeaponMesh->GetSocketLocation(FName("MuzzleFlash"));
	FVector EndTrace = End;
	if (HitResults.Num() > 0)
		EndTrace = HitResults[0].ImpactPoint;
	FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(SocketLocation, EndTrace);
	FActorSpawnParameters SpawnParameters;

	if (ShootingPlayer->IsLocallyControlled())
		GetWorld()->SpawnActor(WeaponTracer, &SocketLocation, &LookRotation, SpawnParameters); //do on client instantly
	
	ServerWeaponTrace(SocketLocation, LookRotation);
	
	//DrawDebugLine(GetWorld(), WeaponMesh->GetSocketLocation(FName("MuzzleFlash")), EndTrace, FColor::Red, false, 2.f, 0, 3.f);
	
	return HitResults;	
}

void AWeaponBase::MultiWeaponTrace_Implementation(const FVector StartLocation, const FRotator StartRotation) const
{
	if (OwningPlayer)
		if (!OwningPlayer->IsLocallyControlled())
			if (OwningPlayer->GetCurrentWeapon())
				if (OwningPlayer->GetCurrentWeapon()->WeaponMesh)
				{
					const FVector SocketLocation = OwningPlayer->GetCurrentWeapon()->WeaponMesh->GetSocketLocation(FName("MuzzleFlash"));
					const FActorSpawnParameters SpawnParameters;
					GetWorld()->SpawnActor(WeaponTracer, &SocketLocation, &StartRotation, SpawnParameters);
				}
	
}

bool AWeaponBase::MultiWeaponTrace_Validate(FVector Location, FRotator Rotation)
{
	return true;
}

void AWeaponBase::ServerWeaponTrace_Implementation(const FVector Location, const FRotator Rotation) const
{
	MultiWeaponTrace(Location, Rotation);
}

bool AWeaponBase::ServerWeaponTrace_Validate(FVector Location, FRotator Rotation)
{
	return true;
}

void AWeaponBase::MultiFire_Implementation(const FHitResult& HitResults)
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

bool AWeaponBase::MultiFire_Validate(const FHitResult& HitResults)
{
	return true;
}

void AWeaponBase::ServerFire_Implementation(const TArray<FHitResult>& HitResults, bool SubAmmo)
{
	if(CurrentAmmo > 0)
	{
		if (SubAmmo) //Dont subtract ammo for each shotgun pellet basically
			--CurrentAmmo;
		
		if(HitResults.Num() > 0)
		{
			for(FHitResult Result : HitResults)
			{
				if(AActor* HitActor = Result.GetActor())
				{
					if(AImpulseDefaultCharacter* Player = Cast<AImpulseDefaultCharacter>(HitActor))
					{
						FString HitBone = Result.BoneName.ToString();
						//UE_LOG(LogTemp, Warning, TEXT("HIT BONE %s ON SERVER"), *HitBone);
						EHitLocation HitLocation;
						if(HitBone == FString("Head"))
						{
							HitLocation = EHitLocation::Head;
						}
						else
						{
							HitLocation = EHitLocation::None;
						}
						AImpulseDefaultCharacter* ShootingPlayer = Cast<AImpulseDefaultCharacter>(GetOwner());
						if(AWeaponBase* OwnerWeapon = ShootingPlayer->GetCurrentWeapon())
						{
							float Damage = OwnerWeapon->GetWeaponDamage().GetDamage(HitLocation);
							UGameplayStatics::ApplyPointDamage(Player, Damage, ShootingPlayer->GetFirstPersonCameraComponent()->GetComponentRotation().Vector(), Result, GetOwner()->GetInstigatorController(), this, TypeOfDamage);
						}
					}
				}
			}
		}
		if(HitResults.Num() > 0)
			MultiFire(HitResults[0]);
		else
			MultiFire(FHitResult());
	}
}

bool AWeaponBase::ServerFire_Validate(const TArray<FHitResult>& HitResults, bool SubAmmo)
{
	return true;
}

void AWeaponBase::ControlFireDelay()
{
	bCanFire = true;
	bIsFiring = false;
}

void AWeaponBase::Fire()
{
	if(AImpulseDefaultCharacter* ShootingPlayer = Cast<AImpulseDefaultCharacter>(GetOwner()))
	{
		if(CurrentAmmo > 0 && bCanFire && !bIsReloading)
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
		
			TArray<FHitResult> HitResults = PerformLineTrace(ShootingPlayer);

			if(GetWorld()->IsNetMode(NM_DedicatedServer))
			{
				if(HitResults.Num() > 0)
					MultiFire(HitResults[0]);
				else
					MultiFire(FHitResult());
			}
			else
				ServerFire(HitResults, true);

			FTimerHandle FireRateHandle;
			GetWorld()->GetTimerManager().SetTimer(FireRateHandle, this, &AWeaponBase::ControlFireDelay, DelayBetweenShoots, false);
			
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

void AWeaponBase::StopFiring()
{
	bIsFiring = false;
}

void AWeaponBase::ChangeFireMode()
{
}

void AWeaponBase::MultiReload_Implementation()
{
	if (OwningPlayer)
	{
		if(!OwningPlayer->IsLocallyControlled() && ReloadAnimation)
		{
			if(UAnimInstance* AnimInstance = OwningPlayer->GetMesh()->GetAnimInstance())
			{
				if(ThirdPersonMontage)
				{
					AnimInstance->Montage_Play(ThirdPersonMontage);
					AnimInstance->Montage_JumpToSection(FName("Reload"), ThirdPersonMontage);
				}
			}
			WeaponMesh->PlayAnimation(ReloadAnimation, false);
		}
	}
}

bool AWeaponBase::MultiReload_Validate()
{
	return true;
}

void AWeaponBase::ServerReload_Implementation()
{
	Reload();
}

bool AWeaponBase::ServerReload_Validate()
{
	return true;
}

void AWeaponBase::Reload()
{
	if(CurrentAmmo != MaxAmmo && !bIsReloading && bCanReload)
	{
		if(UAnimInstance* AnimInstance = OwningPlayer->GetMesh1P()->GetAnimInstance())
		{
			if(FPSArmsFireMontage)
			{
				//PLAY RELOAD FP ANIM
			}
		}

		bIsReloading = true;
		
		if (OwningPlayer)
		{
			if(OwningPlayer->IsLocallyControlled() && ReloadAnimation)
			{
				WeaponMesh->PlayAnimation(ReloadAnimation, false);
			}
		}
		
		if(GetWorld()->IsNetMode(NM_DedicatedServer))
		{
			MultiReload();
		}
		else
		{
			ServerReload();
		}
	}
}

void AWeaponBase::MultiEndReload_Implementation()
{
	if (OwningPlayer)
	{
		if(!OwningPlayer->IsLocallyControlled() && ReloadAnimation)
		{
			if(UAnimInstance* AnimInstance = OwningPlayer->GetMesh()->GetAnimInstance())
			{
				if(ThirdPersonMontage)
				{
					AnimInstance->Montage_Stop(0.25f, ThirdPersonMontage);
					AnimInstance->Montage_JumpToSection(FName("Reload"), ThirdPersonMontage);
				}
			}
		}
	}
}

bool AWeaponBase::MultiEndReload_Validate()
{
	return true;
}

void AWeaponBase::EndReload()
{
	bIsReloading = false;
}

void AWeaponBase::EndedFireAnim()
{
	bCanReload = true;
	//bCanFire = true;
	if (CurrentAmmo == 0)
	{
		Reload();
	}
}

void AWeaponBase::CanGetAmmo()
{
	if (bIsReloading)
	{
		CurrentAmmo = MaxAmmo;
		OwningPlayer->UpdateHUDAmmo(CurrentAmmo, MaxAmmo);
		bIsReloading = false;
	}
}

UAnimMontage* AWeaponBase::GetFireAnimMontage() const
{
	// TODO: setup later to return FPS or TPS fire montage based on locally controlled
	return FPSArmsFireMontage;
}

void AWeaponBase::WeaponIsNowInHand(const bool InHand)
{
	WeaponMesh->SetHiddenInGame(!InHand);
	
	if (OwningPlayer)
	{
		if (OwningPlayer->IsLocallyControlled())
		{
			OwningPlayer->UpdateHUDAmmo(CurrentAmmo, MaxAmmo);
		}
	}
	
	if (bIsReloading)
	{
		if(ThirdPersonMontage)
		{
			EndReload();
			if (GetWorld()->IsNetMode(NM_DedicatedServer))
				MultiEndReload();
		}
	}
}