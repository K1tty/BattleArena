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

	SpawnBall(ETeam::Red);
	SpawnBall(ETeam::Blue);
}

void AArenaGameMode::TickSimulation()
{
	UE_LOG(LogSimulation, Log, TEXT("Tick"));

	if (Simulation->Step() == FSimulation::EState::Running)
	{
		while (auto Event = Simulation->PopEvent())
		{
			if (std::holds_alternative<FMoveEvent>(Event.value()))
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

void AArenaGameMode::SpawnBall(ETeam Team)
{
	const FBall& Ball = Simulation->AddBall(Team);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	const FVector SpawnLocation = Grid->GridToWorld(Ball.Position.X, Ball.Position.Y);
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	ABall* Actor = GetWorld()->SpawnActor<ABall>(BallClass, SpawnLocation, SpawnRotation, SpawnParams);
	Actor->SetColor(Team == ETeam::Red);
	Balls.Add(Ball.Id, Actor);
}

void AArenaGameMode::ExecuteMove(const FMoveEvent& Event)
{
	UE_LOG(LogSimulation, Log, TEXT("%u: Move (%i,%i) -> (%i,%i)"), Event.Source.Id, Event.From.X, Event.From.Y, Event.To.X, Event.To.Y);

	const FVector From = Grid->GridToWorld(Event.From.X, Event.From.Y);
	const FVector To = Grid->GridToWorld(Event.To.X, Event.To.Y);

	TObjectPtr<ABall> SourceBall = Balls[Event.Source.Id];
	SourceBall->PlayMoveToEffect(From, To);
}

void AArenaGameMode::ExecuteAttack(const FAttackEvent& Event)
{
	UE_LOG(LogSimulation, Log, TEXT("%u: Attack %u"), Event.Source.Id, Event.Target.Id);

	TObjectPtr<ABall> SourceBall = Balls[Event.Source.Id];
	SourceBall->PlayAttackEffect();

	TObjectPtr<ABall> TargetBall = Balls[Event.Target.Id];
	TargetBall->PlayDamageEffect();
}

void AArenaGameMode::ExecuteDeath(const FDeathEvent& Event)
{
	UE_LOG(LogSimulation, Log, TEXT("%u: Death"), Event.Source.Id);
	
	TObjectPtr<ABall> SourceBall = Balls[Event.Source.Id];
	SourceBall->PlayDeathEffect();
}
