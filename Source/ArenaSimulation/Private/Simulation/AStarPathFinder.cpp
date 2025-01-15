#include "AStarPathFinder.h"
#include <queue>
#include <unordered_map>


AStarPathFinder::AStarPathFinder(uint8_t InSizeX, uint8_t InSizeY)
	: SizeX(InSizeX)
	, SizeY(InSizeY)
{
}

void AStarPathFinder::SetObstacle(FSimulationCell Cell)
{
	Obstacles.insert(Cell);
}

void AStarPathFinder::ClearObstacle(FSimulationCell Cell)
{
	Obstacles.erase(Cell);
}

struct FOpenSetNode
{
	FSimulationCell Cell;
	int FScore;  // Represents our current best guess as to how cheap a path could be from start to finish if it goes through cell

	bool operator<(const FOpenSetNode& Other) const
	{
		return FScore > Other.FScore;
	}
};

std::vector<FSimulationCell> AStarPathFinder::FindPath(FSimulationCell From, FSimulationCell To)
{
	std::unordered_map<FSimulationCell, FSimulationCell> CameFrom;

	// Currently known cost of the cheapest path from start to n
	std::unordered_map<FSimulationCell, int> GScore;
	GScore[From] = 0;

	// The set of discovered nodes that may need to be re-expanded
	std::priority_queue<FOpenSetNode> OpenSet;
	OpenSet.emplace(From, 0);

	while (!OpenSet.empty())
	{
		FSimulationCell Current = OpenSet.top().Cell;
		if (Current == To)
		{
			std::vector<FSimulationCell> Result;
			//Result.push_back(Current);

			while (CameFrom.contains(Current))
			{
				Current = CameFrom[Current];
				Result.push_back(Current);
			}

			Result.pop_back();

			std::reverse(Result.begin(), Result.end());
			return Result;
		}

		OpenSet.pop();

		const std::vector<FSimulationCell> Neighbours = GetNeighbours(Current);
		for (const FSimulationCell Neighbour : Neighbours)
		{
			if (HasObstacle(Neighbour) && Neighbour != To)
				continue;

			int TentativeGScore = GScore[Current] + Cost(Current, Neighbour);

			const auto Iterator = GScore.find(Neighbour);
			if (Iterator == GScore.cend() || TentativeGScore < Iterator->second)
			{
				// This path to neighbor is better than any previous one. Record it!
				CameFrom[Neighbour] = Current;
				GScore[Neighbour] = TentativeGScore;

				const int FScore = TentativeGScore + Cost(From, Neighbour);
				OpenSet.emplace(Neighbour, FScore);
			}
		}
	}

	return {};
}

int AStarPathFinder::Cost(FSimulationCell From, FSimulationCell To)
{
	return std::abs(int(To.X) - int(From.X)) + std::abs(int(To.Y) - int(From.Y));
}

std::vector<FSimulationCell> AStarPathFinder::GetNeighbours(FSimulationCell Cell) const
{
	std::vector<FSimulationCell> Result;

	const auto AddCell = [&](FSimulationCell InCell, int XOffset, int YOffset)
	{
			if (IsValidCell(InCell, XOffset, YOffset))
				Result.emplace_back(static_cast<uint8_t>(InCell.X + XOffset), static_cast<uint8_t>(InCell.Y + YOffset));
	};

	AddCell(Cell, 1, 0);
	AddCell(Cell, 0, 1);
	AddCell(Cell, -1, 0);
	AddCell(Cell, 0, -1);

	return Result;
}

bool AStarPathFinder::HasObstacle(FSimulationCell Cell) const
{
	return Obstacles.contains(Cell);
}

bool AStarPathFinder::IsValidCell(FSimulationCell Cell, int XOffset, int YOffset) const
{
	const int X = Cell.X + XOffset;
	const int Y = Cell.Y + YOffset;

	return (X >= 0) && (X < SizeX) && (Y >= 0) && (Y < SizeY);
}
