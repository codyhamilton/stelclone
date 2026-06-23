#ifndef ASTEROID_H_
#define ASTEROID_H_

#include "../data/game_types.h"
#include "../data/resources.h"
#include "../data/game_settings.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * Asteroid
 * 
 * An asteroid in the game.
 */
typedef struct {
    char name[8];
    uint32_t cell_index;
    Vector position;
    ResourceDeposit resource_deposits[GAME_RESOURCE_TYPES];
    int8_t speed_x;
    int8_t speed_y;
    uint8_t habitable_space; // in square units
    enum AsteroidSize size;
    enum AsteroidEffect effect;
} Asteroid;

/**
 * Asteroids
 * 
 * A list of asteroids in the game.
 */
typedef struct {
    uint8_t count;
    Asteroid items[GAME_MAX_ASTEROIDS];
} Asteroids;


Asteroids *asteroids(void);

/**
 * Get an asteroid by its index
 * 
 * @param index The index of the asteroid to get
 * @return The asteroid
 */
Asteroid *asteroid(uint8_t index);

/**
 * Destroy an asteroid
 * 
 * @param asteroid The asteroid to destroy
 */
void destroy_asteroid(uint8_t index);

#endif
