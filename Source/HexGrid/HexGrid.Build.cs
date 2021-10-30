// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HexGrid : ModuleRules
{
	public HexGrid(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "ProceduralMeshComponent" });
    }
}
