// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ImpulseServerTarget : TargetRules
{
	public ImpulseServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Impulse");
		bUsesSteam = true;
		GlobalDefinitions.Add("UE4_PROJECT_STEAMGAMEDESC=\"Impulse\"");
		GlobalDefinitions.Add("UE4_PROJECT_STEAMPRODUCTNAME=\"spacewar\"");
		GlobalDefinitions.Add("UE4_PROJECT_STEAMGAMEDIR=\"spacewar\"");
	}
}
