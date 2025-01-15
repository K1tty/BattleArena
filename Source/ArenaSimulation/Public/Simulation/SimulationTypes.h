#pragma once
#include <stdint.h>
#include <functional>


using TBallId = uint32_t;

enum class ESimulationTeam
{
	Red,
	Blue
};

struct FSimulationCell
{
	uint8_t X;
	uint8_t Y;

	bool operator==(const FSimulationCell& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	bool operator<(const FSimulationCell& Other) const
	{
		return (X != Other.X) ? X < Other.X : Y < Other.Y;
	}
};

namespace std
{
	template<>
	struct hash<FSimulationCell>
	{
		size_t operator()(const FSimulationCell& Cell) const
		{
			return std::hash<uint8_t>()(Cell.X) ^ (std::hash<uint8_t>()(Cell.Y) << 1);
		}
	};
}