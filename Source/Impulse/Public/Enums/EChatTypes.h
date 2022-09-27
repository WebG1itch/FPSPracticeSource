#pragma once

#include "UObject/ObjectMacros.h"

/** Custom chat tags for text coloring. */
UENUM()
enum EChatTypes
{
	Global				UMETA(DisplayName = "Global"),
	Match				UMETA(DisplayName = "Match"),
	Enemy				UMETA(DisplayName = "Enemy"),
	Team				UMETA(DisplayName = "Team"),
	Party				UMETA(DisplayName = "Party"),
	Private				UMETA(DisplayName = "Private"),
	System				UMETA(DisplayName = "System")
};