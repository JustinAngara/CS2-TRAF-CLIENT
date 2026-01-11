# Asphyxia CS2 Base Documentation

## Table of Contents
1. [Overview](#overview)
2. [Project Structure](#project-structure)
3. [Features](#features)
4. [Getting Started](#getting-started)
5. [Configuration](#configuration)
6. [Core Components](#core-components)
7. [Feature Implementation](#feature-implementation)
8. [Building the Project](#building-the-project)
9. [Best Practices](#best-practices)
10. [Troubleshooting](#troubleshooting)
11. [License](#license)

## Overview
Asphyxia CS2 Base is a C++ base for creating game modifications for Counter-Strike 2. It provides a foundation for developing cheats with features like ESP, chams, and aimbot. The project is built using C++20 and leverages various libraries including ImGui for the user interface.

## Project Structure
```
asphyxia-cs2/
├── cstrike/                 # Main source code
│   ├── core/               # Core functionality
│   ├── features/           # Game features implementation
│   │   ├── legitbot/       # Aimbot functionality
│   │   ├── misc/           # Miscellaneous features
│   │   └── visuals/        # Visual features (ESP, chams)
│   └── sdk/                # Game SDK interfaces and structures
├── dependencies/           # Third-party dependencies
├── resources/              # Resource files
└── Injector/               # DLL injector
```

## Features
- **ESP**: Player information overlay (name, health, armor, etc.)
- **Chams**: Player model coloring through the wall
- **Aimbot**: Target acquisition and aiming assistance
- **Menu System**: Interactive configuration interface
- **Configuration System**: Save and load settings

## Getting Started

### Prerequisites
- Visual Studio 2022 or later
- Windows 10/11 SDK
- DirectX SDK (if not included with Visual Studio)
- CMake (for building dependencies)

### Cloning the Repository
```bash
git clone https://github.com/JustinAngara/CS-TRAF-CLIENT.git
cd CS-TRAF-CLIENT
```

## Configuration
The configuration system allows you to customize various aspects of the cheat. Configuration files are stored in `%userprofile%\documents\.crown` by default.

### Main Configuration Options
- **Visuals**: Toggle ESP, chams, and other visual features
- **Aimbot**: Configure targeting, smoothing, and keybinds
- **Misc**: Various utility settings

## Core Components

### Core System
- `core.h/cpp`: Core functionality and utilities
- `config.h/cpp`: Configuration management
- `hooks.h/cpp`: Function hooking system
- `interfaces.h/cpp`: Game interface management
- `menu.h/cpp`: ImGui-based menu system
- `variables.h`: Global variables and settings

### Feature System
Features are organized into separate modules:
- **Legitbot**: Aim assistance functionality
- **Visuals**: ESP, chams, and other visual enhancements
- **Misc**: Various utility features

## API Reference

### Core Classes

#### `CCSPlayerController`
Main controller class for player actions and state.

```cpp
// Get the local player controller
CCSPlayerController* pLocalController = CCSPlayerController::GetLocalPlayerController();

// Get player information
const char* playerName = pLocalController->GetPlayerName();
int health = pLocalController->GetPawnHealth();
int armor = pLocalController->GetPawnArmor();
bool hasHelmet = pLocalController->IsPawnHasHelmet();
bool hasDefuser = pLocalController->IsPawnHasDefuser();

// Get the pawn (player character) associated with this controller
C_CSPlayerPawn* pPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(pLocalController->GetPawnHandle());
```

#### `C_CSPlayerPawn`
Represents the player's in-game character.

```cpp
// Get player pawn from controller
C_CSPlayerPawn* pPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(pController->GetPawnHandle());

// Check if player is an enemy
bool isEnemy = pLocalPawn->IsOtherEnemy(pEnemyPawn);

// Get player position and view angles
Vector_t position = pPawn->GetSceneOrigin();
Vector_t eyePosition = pPawn->GetEyePosition();
QAngle_t punchAngles = pPawn->GetAimPuchAngle();

// Check player state
bool isScoped = pPawn->IsScoped();
bool isDefusing = pPawn->IsDefusing();
int shotsFired = pPawn->GetShotsFired();
```

#### `CGameEntitySystem`
Manages all game entities.

```cpp
// Get entity by index
C_BaseEntity* pEntity = I::GameResourceService->pGameEntitySystem->Get<C_BaseEntity>(entityIndex);

// Get highest entity index
int highestIndex = I::GameResourceService->pGameEntitySystem->GetHighestEntityIndex();
```

#### `IEngineClient`
Provides engine functionality.

```cpp
// Check game state
bool isInGame = I::Engine->IsInGame();
bool isConnected = I::Engine->IsConnected();

// Get local player index
int localPlayerIndex = I::Engine->GetLocalPlayer();

// Get current map name
const char* mapName = I::Engine->GetLevelName();
```

### Common Patterns

#### Iterating Through Players
```cpp
// Get local player first
CCSPlayerController* pLocalController = CCSPlayerController::GetLocalPlayerController();
if (!pLocalController) return;

C_CSPlayerPawn* pLocalPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(pLocalController->GetPawnHandle());
if (!pLocalPawn) return;

// Iterate through all possible player indices
for (int i = 1; i <= I::Engine->GetMaxClients(); i++) {
    CCSPlayerController* pController = I::GameResourceService->pGameEntitySystem->Get<CCSPlayerController>(i);
    if (!pController || pController == pLocalController) continue;
    
    C_CSPlayerPawn* pPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(pController->GetPawnHandle());
    if (!pPawn || !pController->IsPawnAlive()) continue;
    
    // Now you can access player data
    const char* name = pController->GetPlayerName();
    Vector_t position = pPawn->GetSceneOrigin();
    bool isEnemy = pLocalPawn->IsOtherEnemy(pPawn);
    
    // Your code here...
}
```

#### Getting Active Weapon
```cpp
if (auto pPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(pController->GetPawnHandle())) {
    if (auto pWeaponServices = pPawn->GetWeaponServices()) {
        if (auto pWeapon = I::GameResourceService->pGameEntitySystem->Get<C_CSWeaponBaseGun>(pWeaponServices->GetActiveWeapon())) {
            // Access weapon properties
            int clip1 = pWeapon->GetClip1();
            // ...
        }
    }
}
```

## Feature Implementation

### Adding a New Feature
1. Create a new header/source pair in the appropriate feature directory
2. Implement your feature by hooking into the appropriate game functions
3. Add configuration options in `core/config.h`
4. Add menu controls in `core/menu.cpp`

### Example: Advanced ESP Feature
```cpp
// features/visuals/esp.h
#pragma once

namespace F {
    namespace VISUALS {
        namespace ESP {
            void Run();
        }
    }
}

// features/visuals/esp.cpp
#include "../features.h"

void F::VISUALS::ESP::Run() {
    if (!C_GET(bool, Vars.bESP))
        return;
        
    // ESP implementation here
}
```

## Building the Project

### Dependencies
- [imgui](https://github.com/ocornut/imgui) - For the menu system
- [minhook](https://github.com/TsudaKageyu/minhook) - For function hooking
- [freetype](https://freetype.org/) - For font rendering

### Build Steps
1. Open the solution file `asphyxia.sln` in Visual Studio
2. Select the appropriate build configuration (Debug/Release)
3. Build the solution (F7)
4. The output will be in the `build` directory

## Best Practices
1. **Code Organization**: Keep related functionality in appropriate namespaces
2. **Memory Safety**: Always check for null pointers and validate data
3. **Performance**: Be mindful of performance when implementing features
4. **Stealth**: Avoid detection by using proper hooking techniques
5. **Documentation**: Document your code and features

## Troubleshooting

### Common Issues
1. **Build Failures**:
   - Ensure all dependencies are properly linked
   - Check that you have the correct Windows SDK version installed

2. **Injection Issues**:
   - Run the injector as administrator
   - Make sure CS2 is running in windowed or borderless windowed mode

3. **Crashes**:
   - Check the debug output for error messages
   - Verify that all game interfaces are properly initialized

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing
Contributions are welcome! Please feel free to submit a Pull Request.

## Disclaimer
This project is for educational purposes only. The authors are not responsible for any misuse of this software.
