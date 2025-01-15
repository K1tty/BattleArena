#pragma once
#include <vector>
#include <random>
#include <optional>
#include <queue>
#include "Simulation/ISimulation.h"
#include "SimulationBall.h"
#include "AStarPathFinder.h"


class FSimulation: public ISimulation
{
public:
	FSimulation(uint32_t Seed, uint8_t SizeX, uint8_t SizeY);

	virtual ESimulationState Step() override;
	virtual std::optional<TSimulationEvent> PopEvent() override;

private:
	static float GetDistanceSquared(const FSimulationCell From, const FSimulationCell To);

	FSimulationBall* FindNearestEnemy(const FSimulationBall& Source);
	bool IsWithinAttackRange(const FSimulationBall& Source, const FSimulationBall& Target) const;

	void SpawnBall(ESimulationTeam Team);
	void MoveTo(FSimulationBall& Ball, const FSimulationBall& Target);
	void Attack(FSimulationBall& Source, FSimulationBall& Target);
	void Death(FSimulationBall& Ball);

	void AddLogEvent(const TSimulationEvent& Event);
	
private:
	std::mt19937 RandomGenerator;
	AStarPathFinder PathFinder;
	uint8_t SizeX;
	uint8_t SizeY;
	std::vector<FSimulationBall> Balls;
	std::queue<TSimulationEvent> EventLog;
};