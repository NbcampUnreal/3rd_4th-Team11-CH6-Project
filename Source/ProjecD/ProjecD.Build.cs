// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjecD : ModuleRules
{
	public ProjecD(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"AIModule", 
			"NavigationSystem",

			// 게임 플레이 어빌리티 시스템
			"GameplayTasks",
            "GameplayAbilities",
            "GameplayTags"
        });

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore"
		});

		PublicIncludePaths.AddRange(new string[] { "ProjecD" });
		
	}
}
