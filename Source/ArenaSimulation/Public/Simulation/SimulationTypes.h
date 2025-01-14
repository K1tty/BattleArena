#pragma once


enum class ESimulationTeam
{
	Red,
	Blue
};

struct FSimulationCell
{
	uint8_t X;
	uint8_t Y;
};

using TBallId = uint32_t;