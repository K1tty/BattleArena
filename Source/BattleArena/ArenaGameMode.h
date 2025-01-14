#pragma once
#include "GameFramework/GameModeBase.h"
#include "Ball.h"
#include "Grid.h"
#include "Simulation.h"
#include "ArenaGameMode.generated.h"


UCLASS()
class AArenaGameMode: public AGameModeBase
{
	GENERATED_BODY()

public:
	AArenaGameMode();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:
	void InitSimulation();
	void TickSimulation();

	void SpawnGrid(uint8 SizeX, uint8 SizeY);
	void SpawnBall(ETeam Team);

	void ExecuteMove(const FMoveEvent& Event);
	void ExecuteAttack(const FAttackEvent& Event);
	void ExecuteDeath(const FDeathEvent& Event);
	
private:
	TUniquePtr<FSimulation> Simulation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGrid> GridClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABall> BallClass;

	UPROPERTY()
	TObjectPtr<AGrid> Grid;

	UPROPERTY()
	TMap<int, ABall*> Balls;
};