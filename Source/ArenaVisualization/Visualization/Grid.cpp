#include "Grid.h"


void AGrid::SetGridSize_Implementation(uint8 InSizeX, uint8 InSizeY)
{
	SizeX = InSizeX;
	SizeY = InSizeY;
}

FVector AGrid::GridToWorld(uint8 CellX, uint8 CellY)
{
	if (SizeX == 0 || SizeY == 0)
	{
		return FVector::ZeroVector;
	}

	const FVector GridStart = GetActorLocation() - FVector(SizeX * CellSize / 2, SizeY * CellSize / 2, 0);
	const FVector CellOffset = FVector(CellX * CellSize, CellY * CellSize, 0);
	const FVector HalfCellOffset = FVector(CellSize / 2, CellSize / 2, 0);

	return GridStart + CellOffset + HalfCellOffset;
}
