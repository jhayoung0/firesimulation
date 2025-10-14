# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**firepjt_first** is an Unreal Engine 5.6 fire simulation/emergency training game with multiple game variants and multiplayer support. The project combines fire safety mechanics with different gameplay styles.

## Build and Development Commands

### Opening the Project
- Open `firepjt_first.uproject` in Unreal Engine 5.6
- Generate Visual Studio project files: Right-click `.uproject` → "Generate Visual Studio project files"
- Build from Visual Studio: Open `firepjt_first.sln` and build the solution

### Building
- **Editor build**: Build `firepjt_first` target in Visual Studio (Development Editor configuration)
- **Game build**: Build `firepjt_first` target (Development configuration)
- Unreal Build Tool handles compilation via `.Build.cs` files

### Testing
- Play-In-Editor (PIE): Use Unreal Editor's Play button
- Multiplayer testing: Set number of players in PIE settings (for lobby/networking features)
- Standalone game: Launch from Visual Studio or package from Unreal Editor

## Architecture

### Game Variants System

The project uses a **variant-based architecture** where different gameplay modes extend the base character/controller classes:

1. **Base Layer** (`Source/firepjt_first/`)
   - `firepjt_firstCharacter`: First-person base character with Enhanced Input
   - `firepjt_firstPlayerController`: Base player controller
   - `firepjt_firstGameMode`: Root game mode (stub implementation)

2. **Fire Simulation Variant** (Main gameplay)
   - `APeopleBase` (extends `firepjt_firstCharacter`): Core fire emergency character
     - Oxygen system with environmental hazards
     - Posture system (crawling reduces smoke inhalation via `IsCrawl` state)
     - Gear bonuses (mask, wet towel reduce oxygen depletion)
     - Interaction system for picking up objects (fire extinguisher, mask, towel, phone)
   - Fire-related actors: `AFireActor`, `AFireExtinguisher`, `AFireHose`
   - UI: `UMainUI` displays oxygen/health stats

3. **Shooter Variant** (`Source/firepjt_first/Variant_Shooter/`)
   - `AShooterCharacter`, `AShooterGameMode`, `AShooterPlayerController`
   - **AI System**: Uses StateTree for NPC behavior
     - `AShooterNPC`: Enemy characters with AI
     - `AShooterAIController`: AI controller with perception
     - Custom StateTree tasks/conditions in `ShooterStateTreeUtility.h`:
       - `FStateTreeLineOfSightToTargetCondition`: LOS checks
       - `FStateTreeSenseEnemiesTask`: AI perception processing
       - `FStateTreeShootAtTargetTask`: Combat behavior
       - `FStateTreeFaceActorTask`, `FStateTreeFaceLocationTask`: Facing logic
   - Weapon system: `AShooterWeapon`, `AShooterProjectile`, `AShooterWeaponHolder`
   - Team scoring system in `AShooterGameMode`

4. **Horror Variant** (`Source/firepjt_first/Variant_Horror/`)
   - `AHorrorCharacter`, `AHorrorGameMode`, `AHorrorPlayerController`
   - Simple first-person horror game mode

5. **Multiplayer/Lobby System** (`Source/firepjt_first/*/Cubee/`)
   - `ALobbyGameMode`: Manages player connections (max 2 players)
   - Tracks connected players via `PostLogin`/`Logout`
   - UI widgets: `UMainWidget`, `UJoinWidget`, `ULobbyWidget`, `UInfoWidget`
   - Located in `Cubee` subdirectories (Private/Public/Cubee)

### Content Structure

- `Content/CustomContents/House/`: Building/environment assets and UI blueprints
  - `UI/`: Main menu, lobby, and join UI widgets (WBP_MainMenu, WBP_Lobby, WBP_Join)
- `Content/CustomContents/Fireman/`: Firefighter character assets
- `Content/CustomContents/People/`: NPC/civilian assets
- `Content/CustomContents/Maps/`: Level maps

### Key Systems

#### Enhanced Input System
- All character variants use Enhanced Input (Unreal 5's new input system)
- Input actions defined in Blueprint and bound in C++ via `UEnhancedInputComponent`
- Character movement, looking, jumping, crawling, and interactions

#### Interaction System (`APeopleBase`)
- Ray-cast detection checks actors within `CanInteractDist` (300 units)
- Attach/detach actors to specific attachment components:
  - `compActor`: General items
  - `compActorMask`: Protective mask
  - `compActorTowel`: Wet towel
  - `compActorPeople`: Rescued NPCs
- Animation playback on interaction (phone, towel, people)

#### Oxygen/Survival System
- `currOxygen` depletes based on: `BaseRate * Posture * Gear`
- **Posture modifier**: Crawling (`IsCrawl`) reduces smoke inhalation
- **Gear modifiers**: `HasMask` and `HasWetTowel` reduce depletion rate
- Camera position changes when crawling (defined in `CamerRotationCrawl`)

#### StateTree AI (Shooter Variant)
- AI behavior defined via StateTree assets (data-driven)
- Custom tasks/conditions in `ShooterStateTreeUtility.h` handle:
  - Enemy sensing and target acquisition
  - Line-of-sight validation
  - Combat actions (shooting, facing)
- NPCs use `AShooterAIController` with AI Perception Component

### Module Dependencies

**Build.cs dependencies**:
- Core modules: `Core`, `CoreUObject`, `Engine`, `InputCore`, `EnhancedInput`
- AI: `AIModule`, `StateTreeModule`, `GameplayStateTreeModule`
- UI: `UMG`, `Slate`, `SlateCore`
- Networking: `Sockets`, `Networking`
- VFX: `Niagara` (likely for fire effects)

**Enabled plugins** (`.uproject`):
- `ModelingToolsEditorMode`: Level editing tools
- `StateTree`, `GameplayStateTree`: AI behavior system
- `NiagaraFluids`: Fluid simulation for fire/smoke

## Networking Notes

- Lobby system suggests multiplayer support (LAN or listen server)
- `ALobbyGameMode::StartGame()` initiates gameplay after player join
- Network code uses `Sockets` and `Networking` modules
- Max 2 players currently configured in lobby

## Development Workflow

1. **Adding new fire simulation features**: Extend `APeopleBase` or related actors
2. **Creating new game variants**: Follow the pattern of Shooter/Horror variants (Character → GameMode → PlayerController)
3. **Modifying AI**: Edit StateTree assets in Content or extend `ShooterStateTreeUtility` tasks
4. **UI changes**: Widgets are primarily Blueprint-based (`WBP_*` files) with C++ backing classes
5. **Networking features**: Modify `ALobbyGameMode` and related lobby widgets

## Current Development Focus (Cubee Branch)

The **Cubee** branch is actively developing multiplayer/lobby functionality. Primary files being worked on:

### Core Networking Files (Active Development)

**`firepjt_firstPlayerController` (Base Controller)**
- Location: `Source/firepjt_first/firepjt_firstPlayerController.h/cpp`
- Manages input mapping contexts and mobile controls
- **Map-based initialization logic** in `BeginPlay()`:
  - Detects current map name via `GetWorld()->GetName()`
  - **House map**: Game mode (mouse hidden, `FInputModeGameOnly`)
  - **Lobby map**: UI mode (mouse visible, `FInputModeUIOnly`, spawns `ULobbyWidget`)
- Spawns `LobbyWidget` for local controllers in lobby
- Uses `DefaultMappingContexts` and `MobileExcludedMappingContexts` arrays

**`ALobbyGameMode`**
- Location: `Source/firepjt_first/Public/Cubee/LobbyGameMode.h`, `Private/Cubee/LobbyGameMode.cpp`
- Manages player connections in lobby
- **`PostLogin()`**: Adds new players to `ConnectedPlayers` array
- **`Logout()`**: Removes disconnecting players from array
- **`StartGame()`**:
  - Checks if `ConnectedPlayers.Num() >= MaxPlayers` (currently 2)
  - Calls `GetWorld()->ServerTravel("HouseMap?listen")` to start game
- Blueprint callable for UI integration

**`ULobbyWidget`**
- Location: `Source/firepjt_first/Public/Cubee/LobbyWidget.h`, `Private/Cubee/LobbyWidget.cpp`
- Main lobby UI with bound widgets: `Btn_Start`, `Btn_Firefighter`, `Btn_Citizen`, `Txt_PlayerCount`
- **`OnStartClicked()`**: Gets `ALobbyGameMode` and calls `StartGame()`
- **`OnFirefighterClicked()`/`OnCitizenClicked()`**: Role selection (currently stubs)
- **`UpdatePlayerCount(int32 Current, int32 Max)`**: Updates player count text display
- Paired with Blueprint `WBP_Lobby` in `Content/CustomContents/House/UI/`

### Supporting Lobby Widgets (Cubee)

**`UMainWidget`** (Main Menu)
- Location: `Source/firepjt_first/Public/Cubee/MainWidget.h`
- Entry point UI with buttons: `Btn_Host`, `Btn_Join`, `Btn_Exit`
- **`OnHostClicked()`**: Creates host and travels to lobby (`CreateHost()`)
- **`OnJoinClicked()`**: Shows `UJoinWidget` for IP entry
- Contains child widget `UJoinWidget` (bound via meta BindWidget)
- Paired with `WBP_MainMenu` Blueprint

**`UJoinWidget`** (Join Server UI)
- Location: `Source/firepjt_first/Public/Cubee/JoinWidget.h`
- Widgets: `EdtTxt_Url` (editable text for IP), `Btn_Back`
- **`JoinURL(const FText& inText, ETextCommit::Type inCommitMethod)`**: Handles URL submission to join server
- **`OnBackPressed()`**: Returns to main menu
- Paired with `WBP_Join` Blueprint

**`UInfoWidget`** (Info Display System)
- Location: `Source/firepjt_first/Public/Cubee/InfoWidget.h`
- Data-driven info display using `UInfoDataAsset` (DataAsset containing `TArray<FInfoTextEntry>`)
- **`FInfoTextEntry`**: Struct with `Title` and `Description` FText fields
- **`SetInfoDataAsset(UInfoDataAsset* NewDataAsset)`**: Assigns character-specific info data
- **`ViewInfoWidget(int32 Idx)`**: Displays info at index from DataAsset with fade-in animation
- **`RemoveInfoWidget()`**: Hides the widget
- Designed for displaying character role descriptions or tutorial info
- Paired with Blueprint widget

### Lobby Development Notes

- **Map flow**: MainMenu → Lobby (host/join) → HouseMap (game start)
- **Networking**: Uses listen server model (`ServerTravel` with `?listen` parameter)
- **Player roles**: Firefighter/Citizen selection UI exists but logic not implemented
- **UI binding**: C++ widgets use `meta = (BindWidget)` for Blueprint binding
- **Current branch**: `Cubee` (see git status)
- **Recent work**: Lobby player count tracking, role selection buttons, server travel logic

### Working with Cubee Files

When modifying lobby/multiplayer features:
1. **Controller changes**: Edit `firepjt_firstPlayerController` for input/UI initialization
2. **GameMode logic**: Edit `ALobbyGameMode` for connection/session management
3. **UI behavior**: Edit `ULobbyWidget`, `UMainWidget`, `UJoinWidget` for menu logic
4. **Blueprint UI**: Modify corresponding `WBP_*` files in `Content/CustomContents/House/UI/`
5. **Testing**: Use PIE with 2+ players, test host/join flow and server travel

## File Organization Conventions

- Public headers: `Source/firepjt_first/Public/`
- Implementation: `Source/firepjt_first/Private/`
- Variant-specific code in subdirectories: `Variant_Shooter/`, `Variant_Horror/`
- **Networking/multiplayer in `Cubee/` subdirectories** (Active development area)
- Blueprint assets: `Content/CustomContents/`
