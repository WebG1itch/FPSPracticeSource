#pragma once

#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum EWallRunSide
{
	kLeft 	UMETA(DisplayName = "Left", ToolTip = "Running along the left side of a wall"),
	kRight	UMETA(DisplayName = "Right", ToolTip = "Running along the right side of a wall"),
	kStraight UMETA(DisplayName = "Straight", ToolTip = "Not WallRunning"),
};