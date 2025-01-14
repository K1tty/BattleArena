#pragma once
#include "Simulation/SimulationTypes.h"


struct FSimulationBall
{
	static const uint8_t AttackCooldown = 3;
	static const uint8_t MaxHealth = 5;
	static const uint8_t MinHealth = 2;

	TBallId Id;
	FSimulationCell Position;
	uint8_t Health;
	uint8_t AttackCooldownCounter;
	ESimulationTeam Team;

	bool IsAlive() const;
	bool IsEnemy(const FSimulationBall& Ball) const;

	bool IsAttackCooldown() const;
	void StartAttackCooldown();
	void TickAttackCooldown();
	void ApplyAttackDamage();

	float GetHealthPercent() const;
};