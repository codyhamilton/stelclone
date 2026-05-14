#include <stdint.h>

#ifndef GAME_TYPES_H_
#define GAME_TYPES_H_

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
enum ResourceType {
    RES_FR_C1,
    RES_FR_C2,
    RES_FR_A1,
    RES_FR_A2,
    RES_EX_P1,
    RES_EX_P2,
    RES_EX_S1,
    RES_EX_S2,
    RES_RE_P, 
    RES_RE_S, 
};

/**
 * Asteroid size
 * 
 * The relative size of the asteroid.
 * Will affect resources, habitable space and acceleration
 *
 * Smaller have more strategic/rare, larger more common resources
 */
enum AsteroidSize {
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

enum AsteroidEffect {
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