#pragma once
#include <variant>
#include "SimulationTypes.h"


struct FSpawnEvent
{
	TBallId SourceId;
	FSimulationCell Position;
	float Health;
	ESimulationTeam Team;
};

struct FMoveEvent
{
	TBallId SourceId;
	FSimulationCell From;
	FSimulationCell To;
};

struct FAttackEvent
{
	TBallId SourceId;
	TBallId TargetId;
	float TargetHealth; // in [0, 1] range
};

struct FDeathEvent
{
	TBallId SourceId;
};

using TSimulationEvent = std::variant<FSpawnEvent, FMoveEvent, FAttackEvent, FDeathEvent>;