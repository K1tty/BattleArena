#include <iostream>
#include <format>
#include "Simulation/Simulation.h"

template<class... Ts>
struct TOverloadedLambda: Ts... { using Ts::operator()...; };


int main()
{
  FSimulation Simulation(42, 20, 20);
  
  while (Simulation.Step() != ESimulationState::Finished)
  {
    while (auto OptionalEvent = Simulation.PopEvent())
    {
      std::visit(TOverloadedLambda{
        [](const FSpawnEvent& Event) { std::cout << std::format("{}: Spawn ({},{})", Event.SourceId, Event.Position.X, Event.Position.Y) << std::endl; },
        [](const FMoveEvent& Event) { std::cout << std::format("{}: Move ({},{}) -> ({},{})", Event.SourceId, Event.From.X, Event.From.Y, Event.To.X, Event.To.Y) << std::endl; },
        [](const FAttackEvent& Event) { std::cout << std::format("{}: Attack {}", Event.SourceId, Event.TargetId) << std::endl; },
        [](const FDeathEvent& Event) {std::cout << std::format("{}: Death", Event.SourceId) << std::endl; }
      }, OptionalEvent.value());
    }
  }

  return 0;
}