# Free Frontier — Technical Architecture

## Stack

| Layer | Technology |
|---|---|
| Language | TypeScript |
| UI framework | Svelte 5 |
| Bundler | Vite |
| Testing | Vitest |
| Canvas renderer | Pixi.js (deferred — added per-surface when HTML/CSS is insufficient) |
| Desktop packaging | Tauri or Electron (deferred until a desktop build is wanted) |

---

## Project Layout

```
free-frontier/
├── src/
│   ├── domain/
│   │   ├── models/          # TypeScript classes (pure data, no framework deps)
│   │   └── systems/         # Pure TypeScript simulation systems
│   ├── core/
│   │   ├── EventBus.ts      # Typed event bus
│   │   ├── TimeManager.ts   # Game clock, time scales, tick dispatch
│   │   ├── DataManager.ts   # JSON config loading and typed lookups
│   │   ├── GameState.ts     # Mutable game state, Svelte stores for reactive binding
│   │   └── GameLoop.ts      # rAF-driven simulation loop with fixed timestep
│   └── ui/
│       ├── App.svelte        # Root component, screen routing
│       ├── screens/          # One Svelte component per screen
│       └── components/       # Reusable Svelte components
├── config/                   # JSON static data (game truth — unchanged from Godot project)
│   ├── game_config.json
│   ├── ships.json
│   ├── buildings.json
│   └── missiles.json
├── assets/                   # Sprites, images
└── docs/
```

---

## Runtime Architecture

```
┌───────────────────────────────┐
│        UI & Presentation      │  ← Svelte components, screen controllers
├───────────────────────────────┤
│   View-Model Glue (Stores)    │  ← EventBus, Svelte stores in GameState
├───────────────────────────────┤
│  Game Simulation (TypeScript) │  ← Domain systems, pure logic
├───────────────────────────────┤
│     Data / Config (JSON)      │  ← Loaded at startup by DataManager
└───────────────────────────────┘
```

**Key principles:**
- Domain layer has zero framework dependencies — no Svelte, no Pixi, no browser APIs
- UI reads from Svelte stores reactively; never writes to GameState directly
- `GameLoop` advances simulation on a fixed timestep via `requestAnimationFrame`
- `TimeManager` is the sole authority for game time and pause state
- Pixi.js surfaces are isolated to specific Svelte components that mount a `<canvas>` — they do not cross into domain or core

---

## Core Modules

| Module | Purpose |
|---|---|
| `EventBus` | Typed event bus. All events defined in `EventMap` interface; no wildcard subscriptions |
| `TimeManager` | Game clock, pause/resume, time scale multiplier, tick dispatch |
| `DataManager` | Loads `config/*.json` at startup; exposes typed lookups for ShipClass, BuildingClass, MissileClass |
| `GameState` | Holds all mutable game state; exposes Svelte stores for UI-reactive fields |
| `GameLoop` | rAF loop with fixed simulation timestep; calls systems in order each tick |
| `SaveSystem` | JSON serialisation/deserialisation of GameState via model `toJSON()`/`fromJSON()` |

---

## Domain Layer

### Models (pure TypeScript classes)

```typescript
class Asteroid {
  id: string; position: Vector2; velocity: Vector2;
  resources: Record<string, number>; size: number;
  toJSON(): object; static fromJSON(data: object): Asteroid;
}

class Colony {
  id: string; name: string; bodyId: string; position: Vector2;
  buildings: Map<string, BuildingInstance>;
  storage: ResourceStorage;
}

class Fleet {
  id: string; name: string; ships: ShipInstance[];
  position: Vector2; status: FleetStatus;
  currentSector: string; destinationSector: string;
  orders: FleetOrder[];
  // fuel, maxFuel added in [NEW]-game-systems plan 03
}
```

### Systems (pure functions)

```typescript
// No side effects beyond mutating passed state and emitting events
EconomySystem.processTick(colonies: Colony[], buildingClasses: Map<string, BuildingClass>): void
AsteroidSystem.processTick(asteroids: Asteroid[]): void
AsteroidSystem.generateAsteroids(seed: number, config: WorldConfig): Asteroid[]
FleetSystem.processTick(fleets: Fleet[], asteroids: Asteroid[], secondsPerDay: number): void
// Added in [NEW]-game-systems:
CombatSystem.processTick(engagements: CombatEngagement[], gameState: GameState, rng: PRNG): void
AISystem.processTick(gameState: GameState, rng: PRNG): void
```

---

## Event Patterns

All cross-module communication goes through `EventBus`. Event names use past-tense verbs:

```typescript
interface EventMap {
  colony_added: { colony: Colony };
  player_resources_changed: { colonyId: string; storage: ResourceStorage };
  fleet_arrived: { fleetId: string; asteroidId: string };
  combat_started: { attackerId: string; defenderId: string };
}

EventBus.emit('colony_added', { colony });
EventBus.on('fleet_arrived', ({ fleetId, asteroidId }) => { ... });
```

UI components subscribe to `GameState` Svelte stores for per-tick reactive updates; `EventBus` is for discrete events that UI needs to respond to once.

---

## Data Loading

All game statistics live in `config/*.json`. Nothing is hardcoded.

`DataManager` loads all JSON at startup as static Vite imports (synchronous, bundled):

```typescript
DataManager.getShipClass(id: string): ShipClass
DataManager.getBuildingClass(id: string): BuildingClass
DataManager.getMissileClass(id: string): MissileClass
```

Systems operate with immutable reference data (`ShipClass`) alongside mutable instance state (`ShipInstance`).

---

## Economy Tick System

`GameLoop` drives simulation. On each day tick (`advanceDayTick()`):

1. `TimeManager` advances game time at the current time scale
2. `EconomySystem.processTick()` iterates colonies: applies production/consumption, updates `ResourceStorage`
3. `AsteroidSystem.processTick()` updates resource densities (half-life depletion)
4. `FleetSystem.processTick()` advances fleet positions along orders
5. `CombatSystem.processTick()` resolves any active engagements *(added in [NEW]-game-systems plan 04)*
6. `AISystem.processTick()` runs all faction planners after simulation *(added in [NEW]-game-systems plan 05)*
7. `GameState` stores update → Svelte UI re-renders reactively

**Time scales** (game days per real second):

| Speed | Days/sec |
|---|---|
| Paused | 0 |
| Slow | 0.1 |
| Normal | 0.2 |
| Fast | 0.5 |
| Fastest | 2.0 |

---

## Screen Architecture

All screens are Svelte components pre-instantiated under `App.svelte`, shown/hidden by screen state. No mounting/unmounting on navigation.

| Screen | Component | Primary Data Source |
|---|---|---|
| Main Menu | `MainMenu.svelte` | — |
| Sector Map | `SectorMap.svelte` | `GameState.asteroids`, `.fleets`, `.colonies` stores |
| Colony View | `ColonyView.svelte` | `GameState.colonies` store |
| Fleet View | `FleetView.svelte` | `GameState.fleets` store |
| Ship Construction | `ShipConstruction.svelte` | `GameState`, `DataManager` |
| Blueprints | `Blueprints.svelte` | `GameState.blueprints` store |

---

## Naming Conventions

| Type | Convention | Example |
|---|---|---|
| Functions, variables | `camelCase` | `calculateOutput` |
| Types, classes, components | `PascalCase` | `BuildingClass`, `ColonyView.svelte` |
| Constants | `UPPER_SNAKE_CASE` | `MAX_FLEET_SIZE` |
| Private class members | `_underscore_prefix` | `_cachedOutput` |
| Entity IDs | `category_type_variant` | `power_solar_array_standard` |
| Event names | Past-tense verbs | `colony_added`, `resources_changed` |
| File names | `PascalCase.ts` for classes, `camelCase.ts` for modules | `EconomySystem.ts`, `eventBus.ts` |

---

## Testing

| Type | Tool | Scope |
|---|---|---|
| Unit | Vitest | Domain systems: Economy, Combat, Fleet |
| Integration | Vitest | Save/load round-trip, state management |
| Manual | Browser dev server | UI workflows, visual verification |

Domain tests run in Node (no browser, no DOM). Import aliases (`$domain`) make test imports clean.

---

## Performance Targets

| Metric | Target |
|---|---|
| Simulation | 200 asteroids, 20 fleets, 10 colonies at normal time scale without frame drops |
| Memory | < 500 MB RAM for a mid-game session |
| Save game load | < 2 seconds |
| Simulation determinism | Identical output from the same save file |

---

## Related Documentation

- [Game Overview](OVERVIEW.md)
- [Game Design Docs](game/)
- [Stack Migration (completed)](plans/01-stack-migration/)
- [Game Systems Plan](plans/[NEW]-game-systems/) *(folder renamed to `02-game-systems/` when program completes)*
