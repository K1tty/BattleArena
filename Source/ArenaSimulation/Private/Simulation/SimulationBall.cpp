#include "SimulationBall.h"


bool FSimulationBall::IsAlive() const
{
	return Health > 0;
}

bool FSimulationBall::IsEnemy(const FSimulationBall& Ball) const
{
	return Team != Ball.Team;
}

void FSimulationBall::ApplyAttackDamage()
{
	if (IsAlive())
		Health -= 1;
}

bool FSimulationBall::IsAttackCooldown() const
{
	return AttackCooldownCounter > 0;
}

void FSimulationBall::StartAttackCooldown()
{
	AttackCooldownCounter = FSimulationBall::AttackCooldown;
}

void FSimulationBall::TickAttackCooldown()
{
	if (IsAttackCooldown())
		--AttackCooldownCounter;
}

float FSimulationBall::GetHealthPercent() const
{
	return static_cast<float>(Health) / MaxHealth;
}