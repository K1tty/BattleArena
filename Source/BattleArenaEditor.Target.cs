using UnrealBuildTool;

public class BattleArenaEditorTarget : TargetRules
{
	public BattleArenaEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("ArenaVisualization");
	}
}
