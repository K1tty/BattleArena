#include "Module/ArenaSimulationModule.h"
#include "Simulation/Simulation.h"

IMPLEMENT_MODULE(FArenaSimulationModule, ArenaSimulation)


FArenaSimulationModule& FArenaSimulationModule::Get()
{
	const FName ModuleName("ArenaSimulation");

	FModuleManager& ModuleManager = FModuleManager::Get();

	if (!FModuleManager::Get().IsModuleLoaded(ModuleName))
	{
		return FModuleManager::LoadModuleChecked<FArenaSimulationModule>(ModuleName);
	}

	return FModuleManager::GetModuleChecked<FArenaSimulationModule>(ModuleName);
}

TUniquePtr<ISimulation> FArenaSimulationModule::CreateSimulation(uint32_t Seed, uint8_t SizeX, uint8_t SizeY)
{
	return MakeUnique<FSimulation>(Seed, SizeX, SizeY);
}