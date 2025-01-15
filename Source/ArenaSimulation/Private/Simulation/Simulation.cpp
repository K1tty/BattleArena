#include "Simulation/Simulation.h"


FSimulation::FSimulation(uint32_t Seed, uint8_t InSizeX, uint8_t InSizeY)
	: RandomGenerator(Seed)
	, SizeX(InSizeX)
	, SizeY(InSizeY)
{
	SpawnBall(ESimulationTeam::Red);
	SpawnBall(ESimulationTeam::Blue);
}

ESimulationState FSimulation::Step()
{
	bool HadAliveEnemies = false;

	// TODO: O(N^2) complexity. Requires optimization if we have many balls
	for (FSimulationBall& Ball : Balls)
	{
		Ball.TickAttackCooldown();

		if (!Ball.IsAlive())
			continue;

		FSimulationBall* Enemy = FindNearestEnemy(Ball);
		if (!Enemy)
			continue;

		HadAliveEnemies = true;

		if (IsWithinAttackRange(Ball, *Enemy))
		{
			if (!Ball.IsAttackCooldown())
				Attack(Ball, *Enemy);
		}
		else
		{
			MoveTo(Ball, *Enemy);
		}
	};

	return HadAliveEnemies ? ESimulationState::Running : ESimulationState::Finished;
}

std::optional<TSimulationEvent> FSimulation::PopEvent()
{
	if (EventLog.empty())
		return std::nullopt;

	const TSimulationEvent Event = EventLog.front();
	EventLog.pop();

	return Event;
}

FSimulationBall* FSimulation::FindNearestEnemy(const FSimulationBall& Source)
{
	FSimulationBall* NearestEnemy = nullptr;
	float MinDistance = std::numeric_limits<float>::max();

	for (FSimulationBall& Enemy : Balls)
	{
		if (!Enemy.IsAlive() || !Source.IsEnemy(Enemy))
			continue;

		const float Distance = GetDistanceSquared(Source.GetPosition(), Enemy.GetPosition());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			NearestEnemy = &Enemy;
		}
	}
	
	return NearestEnemy;
}

float FSimulation::GetDistanceSquared(const FSimulationCell From, const FSimulationCell To)
{
	const float XDiff = static_cast<float>(To.X - From.X);
	const float YDiff = static_cast<float>(To.Y - From.Y);

	return XDiff * XDiff + YDiff * YDiff;
}

bool FSimulation::IsWithinAttackRange(const FSimulationBall& Source, const FSimulationBall& Target) const
{
	const float AttackRadius = 2.0f;
	const float AttackRadiusSquared = AttackRadius * AttackRadius;

	return GetDistanceSquared(Source.GetPosition(), Target.GetPosition()) < AttackRadiusSquared;
}

void FSimulation::SpawnBall(ESimulationTeam Team)
{
	std::uniform_int_distribution<> XDistribution(0, SizeX - 1);
	std::uniform_int_distribution<> YDistribution(0, SizeY - 1);

	const FSimulationCell RandomPosition
	{
		 .X = static_cast<uint8_t>(XDistribution(RandomGenerator)),
		 .Y = static_cast<uint8_t>(YDistribution(RandomGenerator))
	};

	std::uniform_int_distribution<> HealthDistribution(FSimulationBall::MinHealth, FSimulationBall::MaxHealth);
	const uint8_t RandomHealth = static_cast<uint8_t>(HealthDistribution(RandomGenerator));

	const FSimulationBall Ball(static_cast<TBallId>(Balls.size()), Team, RandomPosition, RandomHealth);

	Balls.push_back(Ball);

	AddLogEvent(FSpawnEvent{ .SourceId = Ball.GetId(), .Position = Ball.GetPosition(), .Health = Ball.GetHealthPercent(), .Team = Ball.GetTeam() });
}

void FSimulation::MoveTo(FSimulationBall& Ball, const FSimulationBall& Target)
{
	const int8_t XDiff = std::clamp(Target.GetPosition().X - Ball.GetPosition().X, -1, 1);
	const int8_t YDiff = std::clamp(Target.GetPosition().Y - Ball.GetPosition().Y, -1, 1);

	const FSimulationCell Choice1 = FSimulationCell{ .X = static_cast<uint8_t>(Ball.GetPosition().X + XDiff), .Y = Ball.GetPosition().Y };
	const FSimulationCell Choice2 = FSimulationCell{ .X = Ball.GetPosition().X, .Y = static_cast<uint8_t>(Ball.GetPosition().Y + YDiff) };

	const float Distance1 = GetDistanceSquared(Choice1, Target.GetPosition());
	const float Distance2 = GetDistanceSquared(Choice2, Target.GetPosition());

	const FSimulationCell& NextPosition = (Distance1 < Distance2) ? Choice1 : Choice2;

	AddLogEvent(FMoveEvent{ .SourceId = Ball.GetId(), .From = Ball.GetPosition(), .To = NextPosition });
	Ball.SetPosition(NextPosition);
}

void FSimulation::Attack(FSimulationBall& Source, FSimulationBall& Target)
{
	Source.StartAttackCooldown();
	Target.ApplyAttackDamage();

	AddLogEvent(FAttackEvent{ .SourceId = Source.GetId(), .TargetId = Target.GetId(), .TargetHealth = Target.GetHealthPercent() });

	if (!Target.IsAlive())
		AddLogEvent(FDeathEvent{ .SourceId = Target.GetId() });
}

void FSimulation::AddLogEvent(const TSimulationEvent& Event)
{
	EventLog.push(Event);
}
