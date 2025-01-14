#pragma once
#include "Modules/ModuleManager.h"
#include "Simulation/ISimulation.h"


class ARENASIMULATION_API FArenaSimulationModule: public FDefaultModuleImpl
{
public:
	static FArenaSimulationModule& Get();

	TUniquePtr<ISimulation> CreateSimulation(uint32_t Seed, uint8_t SizeX, uint8_t SizeY);
};