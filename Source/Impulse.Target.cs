// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ImpulseTarget : TargetRules
{
	public ImpulseTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Impulse");

		bUsesSteam = true;
		GlobalDefinitions.Add("UE4_PROJECT_STEAMGAMEDESC=\"Impulse\"");
		GlobalDefinitions.Add("UE4_PROJECT_STEAMPRODUCTNAME=\"spacewar\"");
		GlobalDefinitions.Add("UE4_PROJECT_STEAMGAMEDIR=\"spacewar\"");
	}
}
