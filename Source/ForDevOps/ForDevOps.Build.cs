// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ForDevOps : ModuleRules
{
    public ForDevOps(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay",
            "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "Json", "JsonUtilities", "UMG" });

        PublicIncludePaths.Add("ForDevOps");
    }
}
