// Copyright Epic Games, Inc. All Rights Reserved.
#include "BattleArena.h"
#include "Simulation.h"


void FBattleArenaModule::StartupModule()
{
	FSimulation Simulation(42, 100, 100);
	while (Simulation.Step() ==	FSimulation::EState::Running)
	{
		while (auto Event = Simulation.PopEvent())
		{
			if (std::holds_alternative<FMoveEvent>(Event.value()))
			{
				const FMoveEvent& e = std::get<FMoveEvent>(Event.value());
				UE_LOG(LogTemp, Log, TEXT("%u: Move (%i,%i) -> (%i,%i)"), e.Source.Id, e.From.X, e.From.Y, e.To.X, e.To.Y);
			}
			else if (std::holds_alternative<FAttackEvent>(Event.value()))
			{
				const FAttackEvent& e = std::get<FAttackEvent>(Event.value());
				UE_LOG(LogTemp, Log, TEXT("%u: Attack %u"), e.Source.Id, e.Target.Id);
			}
			else if (std::holds_alternative<FDeathEvent>(Event.value()))
			{
				const FDeathEvent& e = std::get<FDeathEvent>(Event.value());
				UE_LOG(LogTemp, Log, TEXT("%u: Death"), e.Source.Id);
			}
		}
	}
}

IMPLEMENT_PRIMARY_GAME_MODULE(FBattleArenaModule, BattleArena, "BattleArena");
