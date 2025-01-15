## Overview
This coding challenge aims to separate the deterministic battle simulation packed into a separate simulation library from the Unreal Engine simulation viewer, which can be used to replay the battle outcome using the simulation library.

#### Unreal Engine Project
The Unreal Engine project is split into 2 logical modules:
* `ArenaSimulation` - simulation implementation
* `ArenaVisualization` - simulation viewer

The simulation outcome can be viewed directly on the project default map or in the UE log under the `LogSimulation` category.
Grid size, balls amount, attack radius/cooldown, and visualization tick time are hardcoded to not waste time on configuration that never be used by anyone. But they can still be changed by tweaking code constants.

#### Simulation Library Project
The `RawSimulation` directory contains a Visual Studio solution as a proof of concept for separated simulation development.
It's a Windows console application with a simulation outcome logged to standard output.

## Pre-Requisites
* Visual Studio 2022
* Unreal Engine 5.4.4

## Known Issues
* Grid size value should be even due to the limitation of standard UE checkered grid rendering material usage
* Rarely balls can intersect with a grid due to some bug
