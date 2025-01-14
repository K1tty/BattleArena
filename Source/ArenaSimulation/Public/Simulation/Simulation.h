	#pragma once
#include <vector>
#include <random>
#include <variant>
#include <optional>
#include <queue>


enum class ETeam: uint8_t
{
	Red,
	Blue
};

struct FGridCell
{
	uint8_t X;
	uint8_t Y;
};

using TBallId = uint32_t;

struct FBall
{
	static const uint8_t AttackCooldown = 3;
	static const uint8_t MaxHealth = 5;
	static const uint8_t MinHealth = 2;

	TBallId Id;
	FGridCell Position;
	uint8_t Health;
	uint8_t AttackCooldownCounter;
	ETeam Team;

	bool IsAlive() const
	{
		return Health > 0;
	}

	bool IsEnemy(const FBall& Ball) const
	{
		return Team != Ball.Team;
	}

	bool IsAttackCooldown() const
	{
		return AttackCooldownCounter > 0;
	}

	void TickAttackCooldown()
	{
		if (IsAttackCooldown())
			--AttackCooldownCounter;
	}

	float GetHealthPercent() const
	{
		return static_cast<float>(Health) / MaxHealth;
	}
};

struct FSpawnEvent
{
	TBallId SourceId;
	FGridCell Position;
	float Health;
	ETeam Team;
};

struct FMoveEvent
{
	TBallId SourceId;
	FGridCell From;
	FGridCell To;
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

class ARENASIMULATION_API FSimulation
{
public:
	enum class EState
	{
		Running,
		Finished
	};

public:
	FSimulation(uint32_t Seed, uint8_t SizeX, uint8_t SizeY);

	EState Step();
	std::optional<TSimulationEvent> PopEvent();

private:
	static float GetDistanceSquared(const FGridCell From, const FGridCell To);

	FBall* FindNearestEnemy(const FBall& Source);
	bool IsWithinAttackRange(const FBall& Source, const FBall& Target) const;

	void SpawnBall(ETeam Team);
	void MoveTo(FBall& Ball, const FBall& Target);
	void Attack(FBall& Source, FBall& Target);

	void AddLogEvent(const TSimulationEvent& Event);
	
private:
	std::mt19937 RandomGenerator;
	uint8_t SizeX;
	uint8_t SizeY;
	std::vector<FBall> Balls;
	std::queue<TSimulationEvent> EventLog;
};