# Free Frontier - Technical Implementation Plan

## Overview

This document outlines the technical architecture and implementation roadmap for Free Frontier, a 2D real-time strategy game built with Godot 4.5. The plan follows Godot best practices and community recommendations for large-scale strategy game development.

## 1. High-Level Technical Architecture

### A. Engine & Technology Stack
- **Engine**: Godot 4.5
- **Primary Language**: GDScript for all game logic
- **Performance Optimization**: C++ GDExtension for identified bottlenecks
- **Performance Target**: 60 FPS on mid-tier laptops
- **Physics**: Use Godot's "Idle" physics step for economy ticks

### B. Language Strategy

GDScript is used for all game code, providing:
- Rapid iteration and prototyping
- Native Godot integration without marshalling overhead
- Simpler debugging and hot-reload capabilities
- Lower barrier to contribution

When profiling identifies performance bottlenecks (typically in simulation-heavy systems with thousands of entities), those specific systems can be migrated to C++ GDExtension while maintaining the same API. This "profile first, optimize second" approach ensures effort is focused where it matters.

**Candidates for future C++ migration** (if profiling indicates need):
- Combat simulation with many simultaneous engagements
- Pathfinding for large fleet counts
- Economy tick processing with hundreds of colonies

### C. Project Folder Layout

```
free-frontier/
├── assets/                    # Raw art, audio, sprites
│   └── images/
│       └── ships/             # Ship sprites and construction images
├── scenes/                    # Godot scene files (.tscn)
│   ├── ui/
│   │   └── components/        # Reusable UI component scenes
│   ├── main_menu.tscn
│   ├── sector_map.tscn
│   ├── colony_view.tscn
│   ├── fleet_view.tscn
│   ├── blueprints.tscn
│   └── ship_construction.tscn
├── scripts/
│   ├── core/                  # Autoload singletons
│   │   ├── EventBus.gd        # Global signal bus
│   │   ├── TimeManager.gd     # Game clock and time scale
│   │   ├── DataManager.gd     # JSON config loading
│   │   ├── GameState.gd       # Global game state
│   │   └── GameLoop.gd        # Main simulation loop
│   ├── domain/
│   │   ├── models/            # Data classes
│   │   │   ├── Asteroid.gd
│   │   │   ├── Colony.gd
│   │   │   ├── Fleet.gd
│   │   │   ├── ShipClass.gd
│   │   │   ├── BuildingClass.gd
│   │   │   └── ...
│   │   └── systems/           # Simulation systems
│   │       ├── AsteroidSystem.gd
│   │       ├── EconomySystem.gd
│   │       └── ...
│   └── ui/
│       ├── screens/           # Screen controller scripts
│       │   ├── SectorMap.gd
│       │   ├── ColonyView.gd
│       │   ├── FleetView.gd
│       │   └── ...
│       ├── map/               # Map visualization
│       │   ├── AsteroidMarker.gd
│       │   └── ColonyMarker.gd
│       ├── widgets/           # Reusable UI widgets
│       │   ├── Minimap.gd
│       │   └── SectorDebugOverlay.gd
│       └── components/        # UI components
│           └── StatusBar.gd
├── config/                    # JSON static data
│   ├── game_config.json       # Global game settings
│   ├── ships.json             # Ship class definitions
│   ├── buildings.json         # Building class definitions
│   └── missiles.json          # Missile class definitions
├── addons/                    # Third-party plugins (GUT, etc.)
└── docs/                      # Project documentation
```

### D. Runtime Architecture

```
┌───────────────────────────────┐
│        UI & Presentation      │  ← Control nodes, scene scripts
├───────────────────────────────┤
│   View-Model glue (Signals)   │  ← EventBus, screen controllers
├───────────────────────────────┤
│  Game Simulation (GDScript)   │  ← Domain systems, pure logic
├───────────────────────────────┤
│     Data / Config (JSON)      │  ← Loaded at startup by DataManager
└───────────────────────────────┘
```

**Key Principles:**
- Simulation logic is decoupled from Nodes where possible for testability
- UI listens via signals and pulls read-only data each frame
- Single GameLoop singleton advances simulation in fixed time steps
- TimeManager is the sole authority for game time and pause state

### E. Key Singletons (Autoloads)

| Singleton | Purpose |
|-----------|---------|
| `EventBus` | Global signal bus for decoupled communication |
| `TimeManager` | Global clock, pause/fast-forward, tick dispatch |
| `DataManager` | Parses JSON into typed GDScript objects at startup |
| `GameState` | Holds current sector, asteroids, fleets, colonies |
| `GameLoop` | Advances simulation, triggers economy ticks |

**Future Singletons** (to be implemented):
| Singleton | Purpose |
|-----------|---------|
| `FleetSystem` | Movement + navigation (A* on sector graph) |
| `CombatSystem` | Discrete combat rounds, damage resolution |
| `AISystem` | Corporation & alien AI planners |
| `SaveManager` | Binary save/load |

### F. Scene Guidelines

- **One "Controller" script** on the scene root; children stay presentation-only
- **Use Godot "groups"** for functional categories (e.g., "FleetSprites")
- **Node pooling** for transient objects (missiles, explosions)
- **Leverage scene inheritance** only one level deep

### G. Performance Optimization Strategy

1. **Profile First**: Use Godot's built-in profiler to identify actual bottlenecks
2. **GDScript Optimization**: Apply GDScript best practices (typed variables, avoid allocations in loops)
3. **C++ GDExtension**: For verified hot paths that cannot be optimized in GDScript
4. **Object Pooling**: Reuse objects for frequently spawned/despawned entities

## 2. Core Data Models

```gdscript
# ShipClass - immutable reference data from JSON
class_name ShipClass
extends RefCounted

var id: String
var name: String
var description: String
var stats: ShipStats
var build_cost: ResourceDelta
var crew: CrewRequirements
var consumption: ResourceDelta

# BuildingClass - immutable reference data from JSON  
class_name BuildingClass
extends RefCounted

var id: String
var name: String
var description: String
var grid_size: Vector2i
var category: String
var manufacturer: String
var power_usage: int
var power_output: int
var staff_required: int
var maintenance_cost: int
var build_cost: ResourceDelta
var stats: Dictionary
var armor: int
var max_health: int
var blueprint_required: bool
var lore: Dictionary

# Colony - mutable game state
class_name Colony
extends RefCounted

var id: String
var name: String
var body_id: String
var position: Vector2
var buildings: Dictionary  # id -> BuildingInstance
var storage: ResourceStorage

# ResourceDelta - resource change amounts
class_name ResourceDelta
extends RefCounted

var credits: float = 0.0
var power: float = 0.0
var cobaltite: float = 0.0
var vanadium: float = 0.0
var chromium: float = 0.0
var molybdenum: float = 0.0
var rhenium: float = 0.0
var osmium: float = 0.0
var rhodium: float = 0.0
var iridium: float = 0.0
```

**Data Loading Strategy:**
- Load once at boot via DataManager autoload
- Systems work with immutable reference data + mutable instance state
- All game stats defined in `config/*.json` files, not hardcoded

## 3. Implementation Roadmap

### Milestone 0 - Bootstrapping [COMPLETE]
**Goals:**
- Create repo scaffolding & folder layout
- Set up development environment

**Completed:**
- [x] Set up .editorconfig, Git LFS for binary assets
- [x] Create basic project structure following folder layout
- [x] Set up autoload singletons
- [x] Configure Godot project settings

### Milestone 1 - Simulation Skeleton [COMPLETE]
**Goals:**
- Establish core simulation framework
- Implement basic time management

**Completed:**
- [x] Implement GameLoop singleton with fixed time steps
- [x] Create TimeManager (pause / 1x / 2x / 4x / 8x)
- [x] Implement DataManager JSON loaders (ships, buildings, missiles)
- [x] Author minimal EconomySystem (tick → apply ResourceDelta)
- [x] Create GameState structure with colonies, fleets, asteroids
- [x] Implement EventBus for decoupled signal communication

### Milestone 2 - Sector Map View [IN PROGRESS]
**Goals:**
- Implement main game view
- Basic camera and navigation
- World generation

**Completed:**
- [x] Build SectorMap.tscn with viewport-based rendering
- [x] Implement camera pan/zoom controls
- [x] Draw asteroids from GameState with AsteroidMarker
- [x] Asteroid selection and hover effects
- [x] Minimap implementation
- [x] Procedural asteroid field generation with configurable parameters
- [x] Basic colony marker rendering

**Remaining:**
- [ ] Connect TimeControls buttons → TimeManager
- [ ] Background parallax effects
- [ ] Fleet markers and movement visualization
- [ ] Sensor range visualization

### Milestone 3 - Colonies & Resources
**Goals:**
- Colony management system
- Resource production and consumption

**Tasks:**
- [x] ColonyView.tscn with building grid visualization
- [x] Building placement UI with click-to-place functionality (1x1 buildings)
- [ ] Building construction queue
- [x] Resource storage and overflow handling (basic)
- [x] Colony status indicators (power, life support, population)
- [x] Building categories and variant selection
- [ ] Blueprint requirement enforcement (partially implemented)

### Milestone 4 - Fleets & Pathfinding
**Goals:**
- Fleet management and movement
- Navigation system

**Tasks:**
- [ ] FleetSystem with A* pathfinding
- [ ] FleetView.tscn + Formation display
- [ ] Fleet orders and waypoint system
- [ ] Ship construction from shipyard buildings
- [ ] Fuel/maintenance drain per movement
- [ ] Fleet status monitoring

### Milestone 5 - Combat Prototype
**Goals:**
- Basic combat mechanics
- Damage and destruction

**Tasks:**
- [ ] Combat system with damage resolution
- [ ] Missile mechanics using missiles.json stats
- [ ] Minimal battle UI overlay
- [ ] Combat AI for basic engagements
- [ ] Ship destruction and salvage
- [ ] Building damage and repair

### Milestone 6 - AI & Economy Balancing
**Goals:**
- Intelligent AI opponents
- Dynamic economy

**Tasks:**
- [ ] Corporate AI planners (Goal-Oriented Action Planning or utility AI)
- [ ] Alien species AI with distinct behaviors
- [ ] Dynamic market prices influenced by supply/demand
- [ ] AI fleet management and expansion
- [ ] Economic simulation balancing

### Milestone 7 - Persistence, Polish & UX
**Goals:**
- Save/load system
- User experience improvements

**Tasks:**
- [ ] Binary save/load of GameState
- [ ] NotificationSystem for game events
- [ ] SFX, music integration
- [ ] Settings menu and configuration
- [ ] Tutorial system

### Milestone 8 - Content & Optimization
**Goals:**
- Complete content creation
- Performance optimization

**Tasks:**
- [ ] Finalize ship/building/missile catalogs in JSON
- [ ] Profile and optimize identified bottlenecks
- [ ] Consider C++ GDExtension for hot paths if needed
- [ ] Final balancing and polish
- [ ] Release packaging

## 4. Development Practices

### Code Quality
1. **Use static typing** in all GDScript for performance and clarity
2. **Use Godot's built-in profiler** regularly; watch draw-call counts & allocations
3. **Follow GDScript style guide** (snake_case functions, PascalCase classes)
4. **Weekly playable builds** to catch regressions early

### Testing Strategy
- **GUT Framework**: Use Godot Unit Test addon for automated testing
- **Unit Tests**: Simulation systems (Economy, Combat)
- **Integration Tests**: Save/load, state management
- **Manual Testing**: UI workflows, visual verification

### Version Control
- **Feature branches** for each milestone
- **Conventional commits** for clear history
- **Automated testing** on commits where applicable

## 5. Risk Matrix & Mitigations

| Risk | Impact | Probability | Mitigation |
|------|--------|-------------|------------|
| Performance with large fleets | High | Medium | Profile early, use object pooling, consider C++ for hot paths |
| Save file bloat | Medium | Low | Store IDs not definitions; compress with zstd |
| Scope creep | High | High | Lock feature list per milestone; backlog additions |
| Complex AI behavior | Medium | Medium | Start with simple utility AI; iterate based on playtesting |

## 6. Success Criteria

### Technical Metrics
- **Performance**: 60 FPS with 100+ ships on screen
- **Memory**: < 2GB RAM usage
- **Load Times**: < 5 seconds for save games
- **Determinism**: Identical results from same save file

### Quality Metrics
- **Test Coverage**: Critical simulation systems covered by GUT tests
- **Bug Density**: < 1 critical bug per milestone
- **User Experience**: Intuitive controls, clear feedback

## 7. Next Steps

1. Complete Milestone 2 (Sector Map time controls, fleet visualization)
2. Begin Milestone 3 (Colony management UI)
3. Set up GUT testing framework for simulation systems

## References

- [Godot Architecture Best Practices](https://github.com/abmarnie/godot-architecture-organization-advice)
- [GDScript Style Guide](https://docs.godotengine.org/en/stable/tutorials/scripting/gdscript/gdscript_styleguide.html)
- [Godot 4 Documentation](https://docs.godotengine.org/en/stable/)
- [GUT - Godot Unit Test](https://github.com/bitwes/Gut)

---

**Document Version**: 2.0  
**Last Updated**: January 2026
