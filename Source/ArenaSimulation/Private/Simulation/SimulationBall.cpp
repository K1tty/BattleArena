#include "SimulationBall.h"


FSimulationBall::FSimulationBall(TBallId InId, ESimulationTeam InTeam, FSimulationCell InPosition, uint8_t InHealth)
	: Id(InId)
	, Team(InTeam)
	, Position(InPosition)
	, Health(InHealth)
	, AttackCooldownCounter(0)
{
}

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

TBallId FSimulationBall::GetId() const
{
	return Id;
}

ESimulationTeam FSimulationBall::GetTeam() const
{
	return Team;
}

FSimulationCell FSimulationBall::GetPosition() const
{
	return Position;
}

void FSimulationBall::SetPosition(FSimulationCell InPosition)
{
	Position = InPosition;
}

float FSimulationBall::GetHealthPercent() const
{
	return static_cast<float>(Health) / MaxHealth;
}