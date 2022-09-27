#pragma once

#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum EWeaponID
{
	PrototypePistol UMETA(DisplayName, "PrototypePistol"),
	PrototypeAssaultRifle UMETA(DisplayName, "PrototypeAssaultRifle"),
	PrototypeShotgun UMETA(DisplayName, "PrototypeShotgun"),
	PrototypeSniperRifle UMETA(DisplayName, "PrototypeSniperRifle"),
	NoWeapon UMETA(DisplayName, "NoWeapon")
};
