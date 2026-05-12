# Colonization and Base Building

## Overview

Colonies are established on asteroids throughout the Frontier Sector. Each colony consists of buildings placed on a grid, with buildings consuming power, requiring staff, and providing various functions essential for survival and expansion.

All building statistics are defined in `config/buildings.json`. This document describes the building system conceptually; refer to the JSON config for exact values.

## Building Categories

Buildings are organized into eight categories, each serving distinct functions:

### 1. Habitation
Provides living space and quality of life for colony workers.

| Building Type | Purpose |
|---------------|---------|
| **Habitat** | Worker housing with varying comfort levels |
| **Leisure Complex** | Recreation facilities affecting morale |
| **Medical Bay** | Healthcare facilities for worker health |

**Manufacturer Focus**: Terraforming Solutions

### 2. Life Support
Critical systems for maintaining breathable atmosphere and sustenance.

| Building Type | Purpose |
|---------------|---------|
| **Oxygen Processor** | Atmospheric processing and air production |
| **Water Processor** | Water recycling and purification |
| **Food Production** | Food generation (aeroponics, hydroponics, biosynth) |
| **Emergency Stores** | Backup supplies for crisis situations |

**Manufacturer Focus**: Terraforming Solutions

### 3. Power
Energy generation and storage for colony operations.

| Building Type | Purpose |
|---------------|---------|
| **Solar Array** | Basic renewable power generation |
| **Fusion Reactor** | High-output power (risk if destroyed) |
| **Power Storage** | Backup power reserves |

**Manufacturer Focus**: Frontier Enterprises

### 4. Security
Internal order and protection against infiltration.

| Building Type | Purpose |
|---------------|---------|
| **Security Station** | Maintaining order, counter-infiltration |

**Manufacturer Focus**: Nova Dynamics

### 5. Defense
Military installations for protecting against external threats.

| Building Type | Purpose |
|---------------|---------|
| **Turret** | Anti-ship weapons (point defense to plasma cannon) |
| **Anti-Missile System** | Missile interception (CIWS, laser grid) |
| **Missile Silo** | Long-range offensive capability |
| **Shield Generator** | Energy shields for colony protection |

**Manufacturer Focus**: Nova Dynamics

### 6. Resource
Mining and storage of valuable minerals.

| Building Type | Purpose |
|---------------|---------|
| **Mine** | Resource extraction from asteroid |
| **Refinery** | Processing raw ore into usable materials |
| **Storage Depot** | Resource storage capacity |

**Manufacturer Focus**: Stellar Resources

### 7. Fleet
Ship construction, storage, and fleet coordination.

| Building Type | Purpose |
|---------------|---------|
| **Hangar** | Ship storage and launch facilities |
| **Shipyard** | Ship construction |
| **Fleet Command** | Fleet coordination and tactical bonuses |
| **Orbital Dock** | Ship repair and maintenance |

**Manufacturer Focus**: Galactic Industries

### 8. Utility
Support systems for detection, communication, and mobility.

| Building Type | Purpose |
|---------------|---------|
| **Sensor Array** | Detection range and scanning |
| **Communications Hub** | Comms range and coordination |
| **Asteroid Engines** | Asteroid mobility (maneuvering, main drive) |
| **Satellite Control** | Satellite network management |

**Manufacturer Focus**: Mixed (Frontier Enterprises primary)

## Building Variants

Each building type has multiple variants offering different tradeoffs:

### Basic Variant
- Cheapest build cost
- Common materials only (cobaltite, vanadium)
- Higher power draw per output
- Lower structural integrity
- No blueprint required

### Efficient Variant
- Moderate cost
- Lower power consumption
- Better upkeep-to-output ratio
- Decent durability
- May require blueprint

### Advanced Variant
- Expensive build cost
- Requires exotic resources (rhenium, osmium)
- Highest output per grid space
- Hardened structure
- Blueprint required

### Hardened Variant (select buildings)
- Sacrifices efficiency for maximum structural integrity
- Designed for high-threat environments
- Higher armor and health values
- Blueprint required

## Grid System

Buildings occupy space on the colony grid:

- Grid size varies by building (e.g., 1x1 for point defense turrets, 4x4 for shipyards)
- Larger buildings typically have higher output but require more space
- Strategic placement affects operational efficiency
- Grid size is specified in `config/buildings.json` as `[width, height]`

## Corporate Manufacturers

Each corporation specializes in certain building types:

| Corporation | Specialty | Primary Buildings |
|-------------|-----------|-------------------|
| Nova Dynamics | Military hardware | Defense, Security |
| Stellar Resources | Resource extraction | Mines, Refineries, Storage |
| Galactic Industries | Shipbuilding | Shipyards, Hangars, Docks |
| Terraforming Solutions | Life support | Habitats, Life Support, Medical |
| Frontier Enterprises | Exploration tech | Sensors, Solar Arrays, Communications |

## Blueprint System

### Blueprint Requirements
- Basic building variants are available from the start
- Advanced variants require purchasing blueprints
- Blueprints are one-time purchases, usable indefinitely
- Some blueprints require specific corporate standing

### Blueprint Acquisition
- Purchase from the galactic federation marketplace
- Each corporation offers their specialized blueprints
- Blueprint costs are defined in `config/buildings.json`

## Building Management

### Power and Staffing
All buildings require:
- **Power**: Supplied by solar arrays, fusion reactors, or storage
- **Staff**: Workers assigned to operate the building
- **Maintenance**: Ongoing credit cost

Automatic priority system for resource allocation:
1. Life-critical systems (oxygen, water, food) - highest priority
2. Defense systems - second priority
3. Industrial facilities - lowest priority

Players can adjust priority levels within these tiers.

### Building States
- **Operational**: Fully functioning, consuming power and staff
- **Offline**: Disabled, maintenance reduced by 80%
- **Under Construction**: Being built, not yet functional
- **Damaged**: Reduced efficiency until repaired

### Structural Integrity
Each building has:
- **Max Health**: Total structural hit points
- **Armor**: Damage reduction from attacks
- **Current Health**: Reduced by damage, restored by repairs

Damage sources:
- Combat damage from enemy weapons
- Environmental wear over time
- Power surges from grid failures

### Repair System
- Requires dedicated repair staff
- Repair speed varies by building type
- Cost per hit point varies by building
- Player can set repair priorities and allocate resources

## Worker Management

### Automatic Employment
- Set maximum worker limits based on colony capacity
- Workers gradually arrive based on:
  - Colony living conditions (habitat comfort)
  - Wage competitiveness vs. galactic average
  - Current worker satisfaction
  - Available housing
- Workers can voluntarily leave if conditions deteriorate

### Worker Costs
- Regular wages (affects efficiency and satisfaction)
- Workers arrive/leave on corporate freighters at scheduled times
- Termination costs for fired workers
- Repatriation costs for deceased workers
- "Widows fund" payments for 10 years after worker death
