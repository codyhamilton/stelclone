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
    FR_C1,
    FR_C2,
    FR_A1,
    FR_A2,
    EX_P1,
    EX_P2,
    EX_S1,
    EX_S2,
    RE_P, 
    RE_S, 
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
    SMALL,
    MEDIUM,
    LARGE
};

#endif