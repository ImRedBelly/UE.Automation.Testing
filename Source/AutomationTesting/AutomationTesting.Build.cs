// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AutomationTesting : ModuleRules
{
	public AutomationTesting(ReadOnlyTargetRules Target) : base(Target)
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
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"AutomationTesting",
			"AutomationTesting/Variant_Platforming",
			"AutomationTesting/Variant_Platforming/Animation",
			"AutomationTesting/Variant_Combat",
			"AutomationTesting/Variant_Combat/AI",
			"AutomationTesting/Variant_Combat/Animation",
			"AutomationTesting/Variant_Combat/Gameplay",
			"AutomationTesting/Variant_Combat/Interfaces",
			"AutomationTesting/Variant_Combat/UI",
			"AutomationTesting/Variant_SideScrolling",
			"AutomationTesting/Variant_SideScrolling/AI",
			"AutomationTesting/Variant_SideScrolling/Gameplay",
			"AutomationTesting/Variant_SideScrolling/Interfaces",
			"AutomationTesting/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
