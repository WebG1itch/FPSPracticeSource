// Copyright Spitfire Labs 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LobbyCharacter.generated.h"


class AWeaponBase;

UCLASS()
class IMPULSE_API ALobbyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALobbyCharacter();

protected:

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	TSubclassOf<AWeaponBase> FirstWeaponClass;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	TSubclassOf<AWeaponBase> SecondWeaponClass;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	TSubclassOf<AWeaponBase> ThirdWeaponClass;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	TSubclassOf<AWeaponBase> FourthWeaponClass;

	UPROPERTY()
	AWeaponBase* CurrentWeapon;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	AWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }

};
