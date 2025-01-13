#include "Simulation.h"


FSimulation::FSimulation(uint32_t Seed, uint8_t InSizeX, uint8_t InSizeY)
	: RandomGenerator(Seed)
	, SizeX(InSizeX)
	, SizeY(InSizeY)
{
	const int BallsPerTeam = 1;
	for (int i = 0; i < BallsPerTeam; ++i)
	{
		AddBall(ETeam::Red);
		AddBall(ETeam::Blue);
	}
}

void FSimulation::AddBall(ETeam Team)
{
	std::uniform_int_distribution<> PositionDistribution(0, 100);
	std::uniform_int_distribution<> HealthDistribution(2, 5);

	const FGridCell RandomPosition
	{
		 .X = static_cast<uint8_t>(PositionDistribution(RandomGenerator)),
		 .Y = static_cast<uint8_t>(PositionDistribution(RandomGenerator))
	};

	const FBall Ball
	{
		.Id = IdCounter++,
		.Position = RandomPosition,
		.Health = static_cast<uint8_t>(HealthDistribution(RandomGenerator)),
		.Team = Team
	};

	Balls.push_back(Ball);
}

FSimulation::EState FSimulation::Step()
{
	bool HadAliveEnemies = false;

	// TODO: O(N^2) complexity. Requires optimization if we have many balls
	for (FBall& Ball : Balls)
	{
		Ball.TickAttackCooldown();

		if (!Ball.IsAlive())
			continue;

		FBall* Enemy = FindNearestEnemy(Ball);
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

	return HadAliveEnemies ? EState::Running : EState::Finished;
}

std::optional<FSimulation::TEvent> FSimulation::PopEvent()
{
	if (EventLog.empty())
		return std::nullopt;

	const TEvent Event = EventLog.front();
	EventLog.pop();

	return Event;
}

FBall* FSimulation::FindNearestEnemy(const FBall& Source)
{
	FBall* NearestEnemy = nullptr;
	float MinDistance = std::numeric_limits<float>::max();

	for (FBall& Enemy : Balls)
	{
		if (!Enemy.IsAlive() || !Source.IsEnemy(Enemy))
			continue;

		const float Distance = GetDistanceSquared(Source, Enemy);
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			NearestEnemy = &Enemy;
		}
	}
	
	return NearestEnemy;
}

float FSimulation::GetDistanceSquared(const FGridCell From, const FGridCell To)
{
	const float XDiff = static_cast<float>(To.X - From.X);
	const float YDiff = static_cast<float>(To.X - From.X);

	return XDiff * XDiff + YDiff * YDiff;
}

float FSimulation::GetDistanceSquared(const FBall& From, const FBall& To)
{
	return GetDistanceSquared(From.Position, To.Position);
}

bool FSimulation::IsWithinAttackRange(const FBall& Source, const FBall& Target) const
{
	const float AttackRadius = 2.0f;
	const float AttackRadiusSquared = AttackRadius * AttackRadius;

	return GetDistanceSquared(Source, Target) < AttackRadiusSquared;
}

void FSimulation::MoveTo(FBall& Ball, const FBall& Target)
{
	const int8_t XDiff = std::clamp(Target.Position.X - Ball.Position.X, -1, 1);
	const int8_t YDiff = std::clamp(Target.Position.Y - Ball.Position.Y, -1, 1);

	const FGridCell Choice1 = FGridCell{ .X = static_cast<uint8_t>(Ball.Position.X + XDiff), .Y = Ball.Position.Y };
	const FGridCell Choice2 = FGridCell{ .X = Ball.Position.X, .Y = static_cast<uint8_t>(Ball.Position.Y + YDiff) };

	const float Distance1 = GetDistanceSquared(Choice1, Target.Position);
	const float Distance2 = GetDistanceSquared(Choice2, Target.Position);

	const FGridCell& NextPosition = (Distance1 < Distance2) ? Choice1 : Choice2;

	AddLogEvent(FMoveEvent{ .Source = Ball, .From = Ball.Position, .To = NextPosition });
	Ball.Position = NextPosition;
}

void FSimulation::Attack(FBall& Source, FBall& Target)
{
	Source.AttackCooldownCounter = FBall::AttackCooldown;
	Target.Health -= 1;

	AddLogEvent(FAttackEvent{ .Source = Source, .Target = Target });

	if (!Target.IsAlive())
		AddLogEvent(FDeathEvent{ .Source = Target });
}

void FSimulation::AddLogEvent(const TEvent& Event)
{
	EventLog.push(Event);
}
