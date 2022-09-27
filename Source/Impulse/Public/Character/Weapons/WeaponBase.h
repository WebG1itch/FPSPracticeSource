
#pragma once

#include "CoreMinimal.h"
#include "Character/ImpulseDefaultCharacter.h"
#include "GameFramework/Actor.h"
#include "Enums/EWeaponID.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum EHitLocation
{
	None UMETA(DisplayName, "None"),
	Head UMETA(DisplayName, "Head")
};

USTRUCT(BlueprintType)
struct FWeaponDamage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
		float BaseDamage = 20.0f;
	UPROPERTY(EditDefaultsOnly)
		float HeadMultiplier = 2.0f;
	float GetDamage(EHitLocation HitLocation)
	{
		if(HitLocation == EHitLocation::Head)
			return BaseDamage * HeadMultiplier;
		else
			return BaseDamage;
	}
};

UCLASS()
class IMPULSE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AWeaponBase();

protected:

#pragma region WEAPON SETTINGS
	
	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
		class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
		TEnumAsByte<EWeaponID> WeaponID;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
		class UAnimationAsset* FireAnimation;

	UPROPERTY(Replicated, EditAnywhere, Category = "Weapon Settings")
		class UClass* WeaponTracer;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
		class UAnimationAsset* ReloadAnimation;
	
	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
		class UAnimMontage* FPSArmsFireMontage;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
		class UAnimMontage* ThirdPersonMontage;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
		FString WeaponName;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
		float DelayBetweenShoots;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
		FWeaponDamage WeaponDamage;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
		float BulletDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Settings")
		TSubclassOf<UDamageType> TypeOfDamage;
	
	UPROPERTY(Replicated)
		int32 CurrentAmmo;

	UPROPERTY(Replicated, EditAnywhere, Category = "Weapon Settings")
		int32 MaxAmmo;
	
	
	bool bCanFire;
	bool bCanReload;
	void ControlFireDelay();

	bool bIsReloading;
	
	UPROPERTY()
	class AImpulseDefaultCharacter* OwningPlayer;
public:
	int32 GetCurrentAmmo() const { return CurrentAmmo; }
	int32 GetMaxAmmo() const { return MaxAmmo; }
	
#pragma endregion 
	
	
	virtual void BeginPlay() override;

#pragma region WEAPON FUNCTIONS
	
	TArray<FHitResult> PerformLineTrace(AImpulseDefaultCharacter* ShootingPlayer) const;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerWeaponTrace(FVector StartLocation, FRotator StartRotation) const;

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void MultiWeaponTrace(FVector StartLocation, FRotator StartRotation) const;
	
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFire(const TArray<FHitResult>& HitResults, bool SubAmmo);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void MultiFire(const FHitResult& HitResult);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerReload();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void MultiReload();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void MultiEndReload();

#pragma endregion
	
private:
	bool bIsFiring;
	
public:

#pragma region WEAPON SETTINGS
	
	virtual void Fire();
	virtual void StopFiring();
	virtual void ChangeFireMode();
	FWeaponDamage GetWeaponDamage() const { return WeaponDamage; }
	virtual bool IsFiring() {return bIsFiring;}
	
	virtual void Reload();
	virtual void EndReload();

	UFUNCTION()
	void EndedFireAnim();
	
	UFUNCTION()
	virtual void CanGetAmmo();

	UAnimMontage* GetFireAnimMontage() const;
	
	TEnumAsByte<EWeaponID> GetWeaponID() const { return WeaponID; }

	void WeaponIsNowInHand(const bool InHand);

	USkeletalMeshComponent* GetWeaponMesh() const {return WeaponMesh;}
	
#pragma endregion
	
};
