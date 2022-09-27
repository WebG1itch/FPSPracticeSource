// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ImpulseEditorTarget : TargetRules
{
	public ImpulseEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Impulse");
		
		bUsesSteam = true;
		BuildEnvironment = TargetBuildEnvironment.Shared;
		bCompileChaos = true;
	}
}
