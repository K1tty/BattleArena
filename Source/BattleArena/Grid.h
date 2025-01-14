#pragma once
#include "Grid.generated.h"


UCLASS()
class AGrid: public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetGridSize(uint8 InSizeX, uint8 InSizeY);

	FVector GridToWorld(uint8 CellX, uint8 CellY);

private:
	const float CellSize = 100;

	uint8 SizeX = 0;
	uint8 SizeY = 0;
};