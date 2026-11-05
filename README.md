# C++ Learning

My homework from a mentored, 8-module C++ fundamentals course (April – August 2026).
The course went from the basics up to object-oriented design. Several later tasks are modelled on the UAV domain: drones, batteries, radio and fiber links, and drone swarms.

## Topics covered

- Build pipeline: preprocessing, compilation and linking errors
- Control flow, functions, enums
- Structs, vectors, memory layout and alignment
- Pointers, references and argument passing
- Classes, constructors and destructors, `const` correctness
- Inheritance, virtual functions, abstract classes, multiple inheritance
- Smart pointers (`std::unique_ptr`)

## Projects

| Folder | Task | Key concepts |
|---|---|---|
| [`08/`](08/drone_hive_assignment.cpp) | **Drone-Hive Control System.** A ground station commands several drone hives through a "queen" drone. Each hive holds one drone type (recon, FPV or bomber), and every type attacks differently: FPV drones are expended on the target, bombers carry a limited number of bombs, and recon drones move to the target instead of attacking. Recon hives can run a "free hunt" to search for target coordinates. | inheritance, polymorphism, `dynamic_cast`, `std::unique_ptr`, ownership |
| [`07/`](07/droneFleet.cpp) | **Drone Fleet.** Radio, fiber, FPV and bomber drones built as a class hierarchy (with multiple inheritance), registered with a master drone and controlled by an operator. | inheritance, virtual functions, virtual destructors |
| [`06/`](06/drones.cpp) | **Drone Control Classes.** Drones with a LiPo battery (4S / 5S / 6S, with low-voltage detection) and a communication module: radio (ELRS / CRSF) or fiber (MAVLink / UDP). | classes, constructors and destructors, `const` correctness |
| [`04/`](04/car_target.cpp) | **Car Target Tracking.** A car moves across a 1000×1000 field to find and hit a target at unknown coordinates, and the program logs every step of the route. | structs, vectors, navigation logic |
| [`car/`](car/car.cpp) | **Car Movement Optimization.** Finds the minimum number of steps for a car to cover a given distance when its speed can change by at most 1 per step. | loops, algorithmic thinking |
| [`03/`](03/filament_estimation.cpp) | **Filament Estimation.** Estimates how much 3D-printer filament a model needs from its geometry. | functions, math |
| [`02/`](02/tile_calc.cpp) | **Tile Calculator.** Calculates how many tiles are needed to cover a room. | functions, basic I/O |

## Build and run

Each task is a single file, so no build system is needed:

```bash
clang++ -std=c++17 -Wall 08/drone_hive_assignment.cpp -o drone_hive
./drone_hive
```

`g++` works the same way. Every file compiles under C++17.

## Next steps

- Deepen modern C++: move semantics, templates, the STL, RAII
- Move towards embedded C++ for microcontrollers and UAV software
