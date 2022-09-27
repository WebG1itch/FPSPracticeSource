#pragma once

#include "UObject/ObjectMacros.h"

/** Custom movement modes for Characters. */
UENUM(BlueprintType)
enum EImpulseMovementMode
{
	CMOVE_Grounded		UMETA(DisplayName = "Grounded"),
	CMOVE_InAir			UMETA(DisplayName = "InAir"),
	CMOVE_WallRunning   UMETA(DisplayName = "WallRunning"),
	CMOVE_MAX			UMETA(Hidden),
};