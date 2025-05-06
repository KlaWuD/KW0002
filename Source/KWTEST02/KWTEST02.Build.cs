// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KWTEST02 : ModuleRules
{
	public KWTEST02(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "KWTEST02" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "AIModule" });
	}
}
