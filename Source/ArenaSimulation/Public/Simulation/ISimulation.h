#pragma once
#include <optional>
#include "SimulationEvents.h"


enum class ESimulationState
{
	Running,
	Finished
};

struct ISimulation
{
	virtual ~ISimulation() = default;

	virtual ESimulationState Step() = 0;
	virtual std::optional<TSimulationEvent> PopEvent() = 0;
};