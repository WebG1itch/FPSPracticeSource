
#pragma once

#include "CoreMinimal.h"
#include "Character/Weapons/WeaponBase.h"
#include "WeaponFull.generated.h"

UCLASS()
class IMPULSE_API AWeaponFull : public AWeaponBase
{
	GENERATED_BODY()
public:
	AWeaponFull();

protected:

#pragma region WEAPON SETTINGS
	
	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
		bool bIsSelectFire;
	bool bIsInFullAuto;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
		float FireRate;
	
	UPROPERTY(ReplicatedUsing = OnRep_StartFullAuto)
		bool bIsFiring;

	UFUNCTION()
		void OnRep_StartFullAuto();

	FTimerHandle WeaponFireHandle;
	void PlayWeaponEffects() const;

#pragma endregion 

protected:
	virtual void BeginPlay() override;

#pragma region WEAPON FUNTIONS
	
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_StartFullAuto(bool IsFiring);
	
	virtual void ServerFire_Implementation(const TArray<FHitResult>& HitResults, bool bSubAmmo) override;

	void OnClientFire();

#pragma endregion 
	
public:
	
#pragma region WEAPON FUNCTIONS
	
	virtual void Fire() override;
	virtual void StopFiring() override;
	virtual void ChangeFireMode() override;
	virtual bool IsFiring() override {return bIsFiring;}

#pragma endregion 
	
};
