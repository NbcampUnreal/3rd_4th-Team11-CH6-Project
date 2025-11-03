// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjecD : ModuleRules
{
	public ProjecD(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[] { "ProjecD" });
		
	}
}
