// Copyright Spitfire Labs 2022


#include "Character/Lobby/LobbyCharacter.h"

#include "Character/Weapons/WeaponBase.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ALobbyCharacter::ALobbyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void ALobbyCharacter::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	int32 temp1gun = UKismetMathLibrary::RandomIntegerInRange(0, 3);
		
	if (temp1gun == 0)
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(FirstWeaponClass, SpawnParams);
	else if (temp1gun == 1)
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(SecondWeaponClass, SpawnParams);
	else if (temp1gun == 2)
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(ThirdWeaponClass, SpawnParams);
	else if (temp1gun == 3)
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(FourthWeaponClass, SpawnParams);

	if (CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("s_hand_r"));
		CurrentWeapon->GetWeaponMesh()->SetCastShadow(true);
		CurrentWeapon->GetWeaponMesh()->SetHiddenInGame(false);
	}
	
}

// Called every frame
void ALobbyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALobbyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

