#pragma once
#include <unordered_set>
#include <vector>
#include "Simulation/SimulationTypes.h"


class AStarPathFinder
{
public:
	AStarPathFinder(uint8_t SizeX, uint8_t SizeY);

	void SetObstacle(FSimulationCell Cell);
	void ClearObstacle(FSimulationCell Cell);
	bool HasObstacle(FSimulationCell Cell) const;

	std::vector<FSimulationCell> FindPath(FSimulationCell From, FSimulationCell To);

private:
	bool IsValidCell(FSimulationCell Cell, int XOffset, int YOffset) const;
	int Cost(FSimulationCell From, FSimulationCell To);
	std::vector<FSimulationCell> GetNeighbours(FSimulationCell Cell) const;

private:
	const uint8_t SizeX;
	const uint8_t SizeY;

	std::unordered_set<FSimulationCell> Obstacles;
};