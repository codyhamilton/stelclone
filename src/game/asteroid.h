#ifndef ASTEROID_H_
#define ASTEROID_H_

#include "../data/game_types.h"
#include "../data/resources.h"
#include "../data/game_settings.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    ResourceTypeDef *resource;
    float quantity;
} ResourceDeposit;

typedef struct {
    ResourceDeposit *items;
    uint8_t count;
} ResourceDeposits;

/**
 * Asteroid
 * 
 * An asteroid in the game.
 */
typedef struct {
    char name[16];
    ResourceDeposits deposits;
    Vector position;
    Vector speed;
    Vector acceleration;
    enum AsteroidSize size;
    enum AsteroidEffect effect;
    uint16_t habitable_space; // in square units
    uint16_t id;
} Asteroid;

/**
 * Asteroids
 * 
 * A list of asteroids in the game.
 */
typedef struct {
    Asteroid *items;
    uint16_t count;
} Asteroids;


/**
 * Home asteroid neighbour count
 * 
 * How many asteroids this home planet is closest to
 */
typedef struct {
    Asteroid *home_asteroid;
    int32_t count;
} HomeAsteroidNeighbourCount;

/**
 * Asteroid generation stats
 * 
 * Statistics for the asteroid generation process.
 */
typedef struct {
    HomeAsteroidNeighbourCount home_asteroid_neighbour_counts[8];
    int placement_retries;
    int placement_failures;
} AsteroidGenerationStats;

AsteroidGenerationStats asteroid_generation_stats(void);

/**
 * Generate the asteroids
 * 
 * @param settings The settings for the asteroid generation
 */
bool asteroids_generate(GameSettings *settings);

Asteroids *asteroids_list();

/**
 * Get an asteroid by its id
 * 
 * @param id The id of the asteroid to get
 * @return The asteroid
 */
Asteroid *asteroid_get(uint16_t id);

/**
 * Destroy an asteroid
 * 
 * @param asteroid The asteroid to destroy
 */
void asteroid_destroy(Asteroid *asteroid);

#endif