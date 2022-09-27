
#pragma once

#include "Character/ImpulseDefaultCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeleeBase.generated.h"

UCLASS()
class IMPULSE_API AMeleeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AMeleeBase();

protected:
	
#pragma region MELEE SETTINGS
	
	UPROPERTY(EditDefaultsOnly, Category = "Melee Settings")
		class UStaticMeshComponent* MeleeMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Melee Settings")
		class UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, Category = "Melee Settings")
		class UAnimMontage* FPSMeleeMontage;

	UPROPERTY(EditAnywhere, Category = "Melee Settings")
		class UAnimMontage* TPSMeleeMontage;

	UPROPERTY(EditAnywhere, Category = "Melee Settings")
		float DelayBetweenMelee;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee Settings")
		TSubclassOf<UDamageType> TypeOfDamage;
	
	bool bCanMelee;

#pragma endregion
	
protected:
	
	virtual void BeginPlay() override;

#pragma region MELEE FUNCTIONS
	
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerMelee(FHitResult HitResult);
	
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void MultiMelee(FHitResult HitResult);

	void ControlMeleeDelay();

	FHitResult PerformBoxTrace(const AImpulseDefaultCharacter* ShootingPlayer) const;
	
#pragma endregion
	
public:

#pragma region MELEE FUNCTIONS
	
	void OnMelee();
	
#pragma endregion
	
};
