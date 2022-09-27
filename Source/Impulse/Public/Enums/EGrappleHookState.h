#pragma once

#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum EGrappleHookState
{
	GRAPPLE_Ready		UMETA(DisplayName = "ReadyToFire"),
	GRAPPLE_Firing		UMETA(DisplayName = "Firing"),
	GRAPPLE_Attached	UMETA(DisplayName = "AttachedToTarget"),
};
