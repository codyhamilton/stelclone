#ifndef ASTEROID_GENERATE_H
#define ASTEROID_GENERATE_H

#include "../../data/game_settings.h"
#include <stdbool.h>

/**
 * Asteroid generation result
 * 
 * Statistics for the asteroid generation process.
 */
typedef struct {
    uint8_t home_asteroid_neighbour_counts[GAME_MAX_PLAYERS];
    uint16_t placement_retries;
    uint16_t placement_failures;
} AsteroidGenerationResult;

/**
 * Generate the asteroids
 * 
 * @param settings The settings for the asteroid generation
 */
bool generate_asteroids(GameSettings *settings, AsteroidGenerationResult *result);

#endif