// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class firepjt_first : ModuleRules
{
	public firepjt_first(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate",
			"Sockets",
			"Networking"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"firepjt_first",
			"firepjt_first/Variant_Horror",
			"firepjt_first/Variant_Horror/UI",
			"firepjt_first/Variant_Shooter",
			"firepjt_first/Variant_Shooter/AI",
			"firepjt_first/Variant_Shooter/UI",
			"firepjt_first/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
