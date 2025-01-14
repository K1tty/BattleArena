using UnrealBuildTool;


public class ArenaVisualization : ModuleRules
{
	public ArenaVisualization(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "ArenaSimulation" });
	}
}
