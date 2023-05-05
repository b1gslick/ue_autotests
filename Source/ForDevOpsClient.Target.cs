using UnrealBuildTool;
using System.Collections.Generic;
public class ForDevOpsClientTarget : TargetRules
{
    public ForDevOpsClientTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Client;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
        ExtraModuleNames.Add("ForDevOps");
    }
}
