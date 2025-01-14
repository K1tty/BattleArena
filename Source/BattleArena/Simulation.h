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

struct FBall
{
	static const uint8_t AttackCooldown = 3;

	int Id;
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
};

struct FMoveEvent
{
	const FBall& Source;
	FGridCell From;
	FGridCell To;
};

struct FAttackEvent
{
	const FBall& Source;
	const FBall& Target;
};

struct FDeathEvent
{
	const FBall& Source;
};

class FSimulation
{
public:
	using TEvent = std::variant<FMoveEvent, FAttackEvent, FDeathEvent>;

	enum class EState
	{
		Running,
		Finished
	};

public:
	FSimulation(uint32_t Seed, uint8_t SizeX, uint8_t SizeY);

	const FBall& AddBall(ETeam Team);
	
	EState Step();
	std::optional<TEvent> PopEvent();

private:
	static float GetDistanceSquared(const FGridCell From, const FGridCell To);
	static float GetDistanceSquared(const FBall& From, const FBall& To);

	FBall* FindNearestEnemy(const FBall& Source);
	bool IsWithinAttackRange(const FBall& Source, const FBall& Target) const;

	void MoveTo(FBall& Ball, const FBall& Target);
	void Attack(FBall& Source, FBall& Target);

	void AddLogEvent(const TEvent& Event);
	
private:
	std::mt19937 RandomGenerator;
	uint8_t SizeX;
	uint8_t SizeY;
	std::vector<FBall> Balls;
	std::queue<TEvent> EventLog;
};