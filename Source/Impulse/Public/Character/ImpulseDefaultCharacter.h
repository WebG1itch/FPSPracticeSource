#pragma once

/* --- Commented code is untested. Test before changing. --- */
#include "CoreMinimal.h"
#include "Abilities/AbilityBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/MyCharacterMovementComponent.h"
#include "Enums/EMovementAbilities.h"
#include "Sound/SoundCue.h"
#include "ImpulseDefaultCharacter.generated.h"


class AImpulsePlayerController;
class AWeaponBase;
class AMeleeBase;
class AConcussiveGrenade;
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class USphereComponent;
class UTextRenderComponent;
class UWidgetComponent;
class UNameTagWidget;
class UInWorldUIComponent;

// Declaration of the delegate that will be called when the Primary Action is triggered
// It is declared as dynamic so it can be accessed also in Blueprints
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);

UCLASS(config=Game)
class AImpulseDefaultCharacter : public ACharacter
{
	GENERATED_BODY()
protected:

#pragma region CHARACTER SETTINGS
	
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(EditAnywhere, Category = "Character Settings")
	float ForwardVelocityScale;
	
	UPROPERTY(EditAnywhere, Category = "Character Settings")
	float BackwardVelocityScale;
	
	UPROPERTY(EditAnywhere, Category = "Character Settings")
	float RightVelocityScale;
	
	UPROPERTY(EditAnywhere, Category = "Character Settings")
	float LeftVelocityScale;
	
	UPROPERTY(EditAnywhere, Category = "Character Settings")
	float SensitivityScale;

	UPROPERTY(Replicated)
	float CharacterPitch;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerLookUp(float Pitch);

	UPROPERTY()
	UAudioComponent* FootStepAudioComponent;
	
	UPROPERTY(EditAnywhere, Category = "Sound Settings")
	USoundCue* FootstepSoundCue;

	UPROPERTY()
	UAudioComponent* UseAbilityAudioComponent;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* UseAbilitySoundCue;

	UPROPERTY()
	AImpulsePlayerController* ImpulsePlayerController;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* NearbyPlayerSphereComponent;

	// I don't have a better way of updating kill feed with whether or not you were headshot...
	bool bIsHeadShot;
	
public:
	UPROPERTY(EditDefaultsOnly)
	UInWorldUIComponent* InWorldUIComponent;

protected:
#pragma endregion

#pragma region WEAPON SETTINGS
	
	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	TSubclassOf<AWeaponBase> FirstWeaponClass;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	TSubclassOf<AWeaponBase> SecondWeaponClass;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	TSubclassOf<AWeaponBase> ThirdWeaponClass;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	TSubclassOf<AWeaponBase> FourthWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Settings")
	TSubclassOf<AMeleeBase> MeleeClass;

	UPROPERTY(ReplicatedUsing = OnRep_AttachWeapon)
	AWeaponBase* CurrentWeapon;

	UPROPERTY()
	AWeaponBase* PreviousWeapon;
	
	UPROPERTY(Replicated)
	AMeleeBase* MeleeBase;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAbilityBase> UtilityAbilityClass;

	UPROPERTY(EditDefaultsOnly)
	float ImpulseGrenadeCooldown = 8.f;

	UPROPERTY(Replicated)
	AAbilityBase* CurrentUtilityAbility;
	
	UFUNCTION()
		void OnRep_AttachWeapon();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSwitchWeapon(AWeaponBase* NewWeapon);
	
	int32 WeaponIndex;
	
	UPROPERTY(Replicated)
	TArray<AWeaponBase*> WeaponArray;

	int32 CurrentWeaponAmmo;
	int32 CurrentWeaponMaxAmmo;
	
#pragma endregion

public:

	FRotator Control_Rotation;
	
	UFUNCTION(Server, Unreliable)
	void ServerUpdateControlRotation(FRotator Rotation);

	UFUNCTION(Server, Unreliable)
	void MultiUpdateControlRotation(FRotator Rotation);

#pragma endregion
	
	AImpulseDefaultCharacter(const class FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	UMyCharacterMovementComponent* GetMyMovementComponent() const;

	TEnumAsByte<EMovementAbilities> CurrentMovementAbility;
	void SetMovementAbility(EMovementAbilities MovementAbility);

	bool IsMovementAbility(EMovementAbilities MovementAbility) const;

	UFUNCTION()
	void OnFootstep();

protected:
	
	virtual void BeginPlay();

#pragma region WEAPON FUNCTIONS
	
	void SwitchNextWeapon();
	void SwitchPreviousWeapon();

	void SwitchToFirstWeapon();
	void SwitchToSecondWeapon();
	
	void OnFire();
	virtual void OnStopFire();
	virtual void ChangeWeaponFireMode();

	void OnReload();

	void OnMelee();
	
#pragma endregion

#pragma region CHARACTER FUNCTIONS

	FVector GetCharacterLookLocation();
	
	virtual void Jump() override;
	virtual void StopJumping() override;

	void OnSprint();
	void OnStopSprint();
	
	void OnCrouch();
	void OnStopCrouch();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayerCrouched();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayerUnCrouched();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnFootstep();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MultiOnFootstep();

	bool CanUseUtilityAbility = true;
	
	void OnBeginUtilityAbility();

	void EndUtilityCooldown();
	
	UFUNCTION(Server, Unreliable)
	void ServerOnBeginUtilityAbilitySound();
    
	UFUNCTION(NetMulticast, Unreliable)
	void MultiOnBeginUtilityAbilitySound();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnBeginUtilityAbility(FRotator LookRotation);

	void OnBeginMovementAbility();
	
	void MoveForward(float Val);
	void MoveBackward(float Val);
	void MoveRight(float Val);
	void MoveLeft(float Val);
	
	void LookRight(float Val);
	void LookUp(float Val);

	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

	bool bHasFallenOutOfWorld;

	UFUNCTION()
	void OnReceivePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);
	
#pragma endregion

protected:
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

#pragma region HEALTH
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	class UImpulseDefaultHealthComponent* HealthComponent;

	UFUNCTION()
	void OnHealthChanged(UImpulseDefaultHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(BlueprintReadOnly, Category="Health")
	bool bDead;

	UPROPERTY(ReplicatedUsing = OnRep_OnHealthChanged)
	float CurrentHealth;

	UFUNCTION()
	void OnRep_OnHealthChanged();
	
	UPROPERTY(Replicated)
	AActor* LastActorToDamage;

public:
	UFUNCTION(Client, Reliable)
	void UpdateHUDHealth();

#pragma endregion

public:
	
#pragma region CHARACTER FUNCTIONS
	
	UFUNCTION(BlueprintPure)
	float GetCharacterPitch() const;
	
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:
	UFUNCTION()
	void OverlapWithNearbyPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void EndOverlapWithNearbyPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma endregion

#pragma region WEAPON FUNCTIONS
	
public:
	
	AWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }

	TArray<AWeaponBase*> GetWeaponArray() const { return WeaponArray; }

	void UpdateHUDAmmo(int32 CurrentAmmo, int32 MaxAmmo);
	
#pragma endregion 

protected:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool CanJumpInternal_Implementation() const override;
};
