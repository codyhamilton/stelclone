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
    int16_t x;
    int16_t y;
} Vector;


/**
 * Asteroid size
 * 
 * The relative size of the asteroid.
 * Will affect resources, habitable space and acceleration
 *
 * Smaller have more strategic/rare, larger more common resources
 */
enum AsteroidSize : uint8_t {
    AST_SIZE_SMALL,
    AST_SIZE_MEDIUM,
    AST_SIZE_LARGE,
};

/*
 * Asteroid effect
 * 
 * An effect, if any, applied to the asteroid.
 * Will affect the asteroid's resources, habitable space and acceleration.
 *
 * Note when adding new effects, you must update the AST_EFFECT_START and AST_EFFECT_END macros.
 */
enum AsteroidEffect : uint8_t {
    AST_EFFECT_NONE,
    AST_EFFECT_PLAYER_HOME,
    AST_EFFECT_IRRADIATED,
    AST_EFFECT_RICH_ICE,
    AST_EFFECT_RICH_MINERALS,
    AST_EFFECT_RICH_GAS,
    AST_EFFECT_GEOTHERMAL_ENERGY,
    AST_EFFECT_UNSTABLE,
    AST_EFFECT_VAST_PLAINS,
    AST_EFFECT_DEEP_RESOURCES,
    AST_EFFECT_ABANDONED_MINES,
    AST_EFFECT_VOLCANIC_ACTIVITY,
};
#define AST_EFFECT_START AST_EFFECT_IRRADIATED
#define AST_EFFECT_END AST_EFFECT_VOLCANIC_ACTIVITY

#endif