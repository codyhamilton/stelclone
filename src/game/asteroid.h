#ifndef ASTEROID_H_
#define ASTEROID_H_

#include "../data/game_types.h"
#include "../data/resources.h"

#include <stdint.h>

/**
 * Game asteroid settings
 * 
 * The settings for the asteroid generation.
 */
typedef struct GameAsteroidSettings {
    uint16_t asteroid_count;
    Vector sector_size;
    float asteroid_speed_modifier; // 0.0 all asteroids stationary, 1.0 all asteroids moving, 0.5 normal spread 
    float asteroid_size_modifier; // 0.0 all asteroids small, 1.0 all large, 0.5 normal spread 

    // Common deposits
    float common_desposit_chance;
    int common_deposit_quantity_base;
    int common_deposit_quantity_stddev;
    // Uncommon deposits
    float uncommon_desposit_chance;
    int uncommon_deposit_quantity_base;
    int uncommon_deposit_quantity_stddev;
    // Rare deposits
    float rare_desposit_chance;
    int rare_deposit_quantity_base;
    int rare_deposit_quantity_stddev;
    // Strategic deposits
    float strategic_desposit_chance;
    int strategic_deposit_quantity_base;
    int strategic_deposit_quantity_stddev;

} GameAsteroidSettings;


typedef struct {
    ResourceTypeDef *resource;
    float quantity;
} ResourceDeposit;

typedef struct {
    ResourceDeposit *items;
    uint8_t count;
} ResourceDepositList;

/**
 * Asteroid
 * 
 * An asteroid in the game.
 */
typedef struct {
    char name[16];
    ResourceDepositList deposits;
    Vector position;
    Vector speed;
    Vector acceleration;
    enum AsteroidSize size;
    uint16_t habitable_space; // in square units
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

void updateAsteroids();

Asteroids *asteroids_generate(GameAsteroidSettings *settings);

void asteroids_destroy(Asteroid *asteroid);

#endif