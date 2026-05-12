# Free Frontier — Game Overview

## What It Is

Free Frontier is a 2D real-time strategy game. Players manage a corporate mining operation in a contested asteroid field: extracting rare resources, building colonies, commanding fleets, and navigating alien diplomacy — all while serving (and secretly undermining) a faceless mega-corporation.

## Setting

The year is 2784. Traditional nation-states have been replaced by five mega-corporations that control human space. The discovery of the **Frontier Sector** — a remote asteroid belt with mineral concentrations unlike anywhere else in known space — has triggered a new gold rush. Human corporations and alien species converge on this unregulated territory, competing for resources with no overarching authority to enforce the rules.

The player manages a specific grid of the Frontier Sector on behalf of their corporate employer. No habitable planets. No law. Everyone is armed and motivated.

## Player Role

You are a **regional manager** for one of the five mega-corporations. You have two layers of objectives running simultaneously:

**Corporate objectives (your employer's agenda):**
- Meet profit growth targets and annual production quotas
- Execute short-term executive demands (which often conflict with long-term goals)
- Fulfil your employment contract to earn your release from service

**Personal objectives (your escape plan):**
- Accumulate enough personal wealth to retire
- Legal routes: performance bonuses, market speculation, resource trading
- Gray and illegal routes: insider trading, corruption, worker exploitation, asset misappropriation

The game ends when you satisfy both your contract terms and your retirement threshold. Your final outcome (the quality of your retirement) depends on total wealth and the methods used to acquire it.

## Moral Axis

Three approaches with distinct risk/reward profiles:

| Path | Style | Outcome |
|---|---|---|
| **Ethical** | Fair treatment, honest practices | Slower but stable; better worker relations, lower risk |
| **Opportunistic** | Calculated risk, efficiency focus | Moderate pace, balanced risk/reward |
| **Ruthless** | Exploit everything, maximise short-term gains | Fastest profits; highest risk, worst relations |

## Core Gameplay Systems

### Sector Map
The primary view. A zoomable 2D map of the asteroid field showing fleet positions, colony locations, resource deposits, sensor ranges, and trade routes. Fleet orders are issued here.

### Colony Management
Establish colonies on asteroids by deploying Coloniser ships. Each colony is built on a grid-placement system with buildings in eight categories:

| Category | Function |
|---|---|
| **Habitation** | Worker housing, recreation, medical care |
| **Life Support** | Oxygen, water, food, emergency stores |
| **Power** | Solar arrays, fusion reactors, power storage |
| **Security** | Internal order, counter-infiltration |
| **Defense** | Anti-ship turrets, missile silos, shields |
| **Resource** | Mines, refineries, storage depots |
| **Fleet** | Hangars, shipyards, orbital docks |
| **Utility** | Sensors, comms, asteroid engines |

Buildings consume power and staff. Life-critical systems receive automatic resource allocation priority.

### Resource Economy
Three tiers of rare minerals drive the economy:

| Tier | Minerals | Availability |
|---|---|---|
| **Frontier** | Cobaltite, Vanadium, Chromium, Molybdenum | 50% chance per asteroid |
| **Exotic** | Rhenium (40%), Osmium (40%), Rhodium (20%), Iridium (20%) | Variable per asteroid |
| **Xenon** | Helium-42, Crystal Matrix | Fixed supply, ~10% of asteroids |

Resources deplete using a **half-life yield model** — each mining cycle (roughly one building-year of extraction) halves remaining density. Physical trading requires ships to be present at the trading location.

### Fleet Management
Build and command multiple ship classes from Fighters to Carriers. Ships are customised with weapon and utility modules. Fleets travel between asteroids, defend colonies, establish new footholds, and engage in combat.

### Missile Warfare
Missiles provide a cheap, low-maintenance area-denial capability around established colonies. A well-stocked missile silo can deter enemy colonisation across a wide perimeter without fleet commitment. Basic missiles are available from the start; advanced types (EMP, penetration, cluster, terrain denial) require blueprints obtained through exploration or trade.

### Alien Diplomacy
Five alien species compete in the sector:

| Species | Profile |
|---|---|
| **K'thari** | Hive-mind insectoids; rapid expansion, swarm tactics |
| **Voss** | Amphibious; methodical, strong defense, diplomatic |
| **Zyn** | Energy-based; fast but fragile, aggressive gathering |
| **Thrax** | Reptilian; powerful military tradition, heavy combat |
| **Quix** | Nomadic; mobile operations, trade-focused |

Relations can be hostile, neutral, or cooperative. Trade with, negotiate with, or fight them.

### Time Control
Real-time with selectable speeds:

| Speed | Rate | Use Case |
|---|---|---|
| **Paused** | Stopped | Issue orders, plan |
| **Slow** | 1 day = 10 sec | Detailed combat, critical moments |
| **Normal** | 1 day = 5 sec | General gameplay (default) |
| **Fast** | 1 day = 2 sec | Routine operations |
| **Fastest** | 1 day = 0.5 sec | Skip quiet periods |

Critical events (combat, colony alerts) trigger auto-pause.

## Corporate Factions

Five NPC mega-corporations, each with distinct strengths and building specialisations:

| Corporation | Founded | Specialty |
|---|---|---|
| **Nova Dynamics** | 2157 | Military hardware, weapons, security |
| **Stellar Resources** | 2189 | Resource extraction and processing |
| **Galactic Industries** | 2212 | Shipbuilding and manufacturing |
| **Terraforming Solutions** | 2245 | Life support and environmental systems |
| **Frontier Enterprises** | 2278 | Exploration tech, sensors, power |

## Win Conditions

**Victory**: Fulfil your employment contract AND accumulate sufficient personal wealth.
**Failure**: Unable to meet contract terms; all colonies lost; financial collapse.

Your retirement outcome (quality of life, reputation, security) is shaped by wealth accumulated and the moral path taken.

## Visual Style

8-bit retro aesthetic with a space industrial / corporate dystopian theme.

## Related Documentation

- [Technical Architecture](ARCHITECTURE.md)
- [Game Background & Setting](game/background.md)
- [Colonization & Buildings](game/colonization.md)
- [Resource Economy](game/economy.md)
- [Gameplay Systems](game/gameplay.md)
- [Ships & Fleets](game/ships.md)
- [Missiles](game/missiles.md)
