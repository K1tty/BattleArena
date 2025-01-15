#pragma once
#include "Simulation/SimulationTypes.h"


class FSimulationBall
{
public:
	static const uint8_t AttackCooldown = 3;
	static const uint8_t MaxHealth = 5;
	static const uint8_t MinHealth = 2;

public:
	FSimulationBall(TBallId Id, ESimulationTeam Team, FSimulationCell Position, uint8_t Health);

	TBallId GetId() const;
	ESimulationTeam GetTeam() const;
	FSimulationCell GetPosition() const;
	void SetPosition(FSimulationCell Position);
	float GetHealthPercent() const;

	bool IsAlive() const;
	bool IsAttackCooldown() const;
	bool IsEnemy(const FSimulationBall& Ball) const;

	void StartAttackCooldown();
	void TickAttackCooldown();
	void ApplyAttackDamage();

private:
	const TBallId Id;
	const ESimulationTeam Team;

	uint8_t Health;
	uint8_t AttackCooldownCounter;
	FSimulationCell Position;
};