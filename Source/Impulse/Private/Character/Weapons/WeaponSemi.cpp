
#include "Character/Weapons/WeaponSemi.h"

AWeaponSemi::AWeaponSemi()
{
	WeaponName = "PrototypePistol";
	MaxAmmo = 10;
}

void AWeaponSemi::BeginPlay()
{
	CurrentAmmo = MaxAmmo;
	Super::BeginPlay();
}
