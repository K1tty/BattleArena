using UnrealBuildTool;


public class ArenaSimulation : ModuleRules
{
	public ArenaSimulation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.Add("Core");
	}
}
