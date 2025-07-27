// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HarvestGAS : ModuleRules
{
	public HarvestGAS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG", "GameplayAbilities", "GameplayTags", "GameplayTasks"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"HarvestGAS",
			"HarvestGAS/Variant_Platforming",
			"HarvestGAS/Variant_Combat",
			"HarvestGAS/Variant_Combat/AI",
			"HarvestGAS/Variant_SideScrolling",
			"HarvestGAS/Variant_SideScrolling/Gameplay",
			"HarvestGAS/Variant_SideScrolling/AI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
