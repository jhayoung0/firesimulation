// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FireSimulation : ModuleRules
{
	public FireSimulation(ReadOnlyTargetRules Target) : base(Target)
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
			"FireSimulation",
			"FireSimulation/Variant_Platforming",
			"FireSimulation/Variant_Platforming/Animation",
			"FireSimulation/Variant_Combat",
			"FireSimulation/Variant_Combat/AI",
			"FireSimulation/Variant_Combat/Animation",
			"FireSimulation/Variant_Combat/Gameplay",
			"FireSimulation/Variant_Combat/Interfaces",
			"FireSimulation/Variant_Combat/UI",
			"FireSimulation/Variant_SideScrolling",
			"FireSimulation/Variant_SideScrolling/AI",
			"FireSimulation/Variant_SideScrolling/Gameplay",
			"FireSimulation/Variant_SideScrolling/Interfaces",
			"FireSimulation/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
