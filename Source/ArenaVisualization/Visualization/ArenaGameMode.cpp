#include "ArenaGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogSimulation, Log, All);


AArenaGameMode::AArenaGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void AArenaGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickSimulation();
}

void AArenaGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitSimulation();
}

void AArenaGameMode::InitSimulation()
{
	const uint32 Seed = GFrameNumber;
	const uint8 GridSize = 20;

	Simulation = MakeUnique<FSimulation>(Seed, GridSize, GridSize);

	SpawnGrid(GridSize, GridSize);
}

void AArenaGameMode::TickSimulation()
{
	UE_LOG(LogSimulation, Log, TEXT("Tick"));

	if (Simulation->Step() == FSimulation::EState::Running)
	{
		while (auto Event = Simulation->PopEvent())
		{
			if (std::holds_alternative<FSpawnEvent>(Event.value()))
			{
				const FSpawnEvent& e = std::get<FSpawnEvent>(Event.value());
				ExecuteSpawn(e);
			}
			else if (std::holds_alternative<FMoveEvent>(Event.value()))
			{
				const FMoveEvent& e = std::get<FMoveEvent>(Event.value());
				ExecuteMove(e);
			}
			else if (std::holds_alternative<FAttackEvent>(Event.value()))
			{
				const FAttackEvent& e = std::get<FAttackEvent>(Event.value());
				ExecuteAttack(e);
				
			}
			else if (std::holds_alternative<FDeathEvent>(Event.value()))
			{
				const FDeathEvent& e = std::get<FDeathEvent>(Event.value());
				ExecuteDeath(e);
			}
		}
	}
}

void AArenaGameMode::SpawnGrid(uint8 SizeX, uint8 SizeY)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	const FVector SpawnLocation = FVector::ZeroVector;
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	Grid = GetWorld()->SpawnActor<AGrid>(GridClass, SpawnLocation, SpawnRotation, SpawnParams);
	Grid->SetGridSize(SizeX, SizeY);
}

void AArenaGameMode::ExecuteSpawn(const FSpawnEvent& Event)
{
	UE_LOG(LogSimulation, Log, TEXT("%u: Spawn"), Event.SourceId);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	const FVector SpawnLocation = Grid->GridToWorld(Event.Position.X, Event.Position.Y);
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	ABall* Actor = GetWorld()->SpawnActor<ABall>(BallClass, SpawnLocation, SpawnRotation, SpawnParams);
	Actor->SetColor(Event.Team == ETeam::Red);
	Actor->SetHealthPercent(Event.Health);

	Balls.Add(Event.SourceId, Actor);
}

void AArenaGameMode::ExecuteMove(const FMoveEvent& Event)
{
	UE_LOG(LogSimulation, Log, TEXT("%u: Move (%i,%i) -> (%i,%i)"), Event.SourceId, Event.From.X, Event.From.Y, Event.To.X, Event.To.Y);

	const FVector From = Grid->GridToWorld(Event.From.X, Event.From.Y);
	const FVector To = Grid->GridToWorld(Event.To.X, Event.To.Y);

	TObjectPtr<ABall> SourceBall = Balls[Event.SourceId];
	SourceBall->PlayMoveToEffect(From, To);
}

void AArenaGameMode::ExecuteAttack(const FAttackEvent& Event)
{
	UE_LOG(LogSimulation, Log, TEXT("%u: Attack %u"), Event.SourceId, Event.TargetId);

	TObjectPtr<ABall> TargetBall = Balls[Event.TargetId];
	TObjectPtr<ABall> SourceBall = Balls[Event.SourceId];

	SourceBall->PlayAttackEffect(TargetBall);
	TargetBall->PlayDamageEffect(Event.TargetHealth);
}

void AArenaGameMode::ExecuteDeath(const FDeathEvent& Event)
{
	UE_LOG(LogSimulation, Log, TEXT("%u: Death"), Event.SourceId);
	
	TObjectPtr<ABall> SourceBall = Balls[Event.SourceId];
	SourceBall->PlayDeathEffect();
}
