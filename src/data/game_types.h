#include <stdint.h>

#ifndef GAME_TYPES_H_
#define GAME_TYPES_H_

#define GAME_MAX_RESOURCES 10 // Maximum resources per asteroid
#define GAME_RESOURCE_TYPES 10 // Maximum resource types in game

/**
* Defines shared game types used throughout the game.
*/


/**
 * Vector
 * 
 * A 2D vector.
 */
typedef struct {
    int32_t x;
    int32_t y;
} Vector;

/**
 * Resource type

 * C: Common
 * A: Alternative
 * P: Primary
 * S: Secondary
 */
enum ResourceType : uint8_t {
    RES_FR_C1 = 0,
    RES_FR_C2 = 1,
    RES_FR_A1 = 2,
    RES_FR_A2 = 3,
    RES_EX_P1 = 100,
    RES_EX_P2 = 101,
    RES_EX_S1 = 102,
    RES_EX_S2 = 103,
    RES_RE_P = 200, 
    RES_RE_S = 201, 
};

/**
 * Asteroid size
 * 
 * The relative size of the asteroid.
 * Will affect resources, habitable space and acceleration
 *
 * Smaller have more strategic/rare, larger more common resources
 */
enum AsteroidSize : uint8_t {
    AST_SIZE_SMALL = 0,
    AST_SIZE_MEDIUM = 1,
    AST_SIZE_LARGE = 2,
};

/*
 * Asteroid effect
 * 
 * The effect of the asteroid.
 * Will affect the asteroid's resources, habitable space and acceleration.
 *
 * Note when adding new effects, you must update the AST_EFFECT_START and AST_EFFECT_END macros.
 */

enum AsteroidEffect : uint8_t {
    AST_EFFECT_NONE = 0,
    AST_EFFECT_PLAYER_HOME = 1,
    AST_EFFECT_IRRADIATED = 2,
    AST_EFFECT_RICH_ICE = 3,
    AST_EFFECT_RICH_MINERALS = 4,
    AST_EFFECT_RICH_GAS = 5,
    AST_EFFECT_GEOTHERMAL_ENERGY = 6,
    AST_EFFECT_UNSTABLE = 7,
    AST_EFFECT_VAST_PLAINS = 8,
    AST_EFFECT_DEEP_RESOURCES = 9,
    AST_EFFECT_ABANDONED_MINES = 10,
    AST_EFFECT_VOLCANIC_ACTIVITY = 11,
};
#define AST_EFFECT_START AST_EFFECT_IRRADIATED
#define AST_EFFECT_END AST_EFFECT_VOLCANIC_ACTIVITY

#endif