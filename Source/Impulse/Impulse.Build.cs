// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Impulse : ModuleRules
{
	public Impulse(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "PhysicsCore", 
			"OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemSteam", "SlateCore", "HTTP", "Json", "JsonUtilities"});

		//Compiler optimizations
		MinFilesUsingPrecompiledHeaderOverride = 1;
		bUseUnity = false;
		
		// PublicAdditionalLibraries.Add(LibraryLinkName);
		// string RuntimeLibrarySourcePath = Path.Combine(SDKBinariesDir, RuntimeLibraryFileName);
		// string RuntimeLibraryTargetPath = Path.Combine(ProjectBinariesDir, RuntimeLibraryFileName);
		// RuntimeDependencies.Add(RuntimeLibraryTargetPath, RuntimeLibrarySourcePath, StagedFileType.NonUFS);
	}
}
