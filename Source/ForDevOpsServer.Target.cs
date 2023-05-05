using UnrealBuildTool;
using System.Collections.Generic;
public class ForDevOpsServerTarget : TargetRules
{
    public ForDevOpsServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
        ExtraModuleNames.Add("ForDevOps");
    }
}
