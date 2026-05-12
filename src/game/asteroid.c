#include "asteroid.h"
#include <stdlib.h>

static Asteroids asteroids;

void updateAsteroids() {
}

static void asteroids_free() {
    for(uint8_t i = 0; i < asteroids.count; i++) {
        free(asteroids.items[i].deposits.items);
    }
    free(asteroids.items);
}

Asteroids *asteroids_generate(GameAsteroidSettings *settings) {
    if(asteroids.count > 0) {
        asteroids_free();
    }
    // Create the asteroids
    asteroids.items = (Asteroid*)calloc(settings->asteroid_count, sizeof(Asteroid));
    asteroids.count = settings->asteroid_count;

    // Create the deposits
    for(uint8_t i = 0; i < asteroids.count; i++) {
        const uint8_t deposit_count = rand() % 10 + 1;
        asteroids.items[i].deposits.items = (ResourceDeposit*)calloc(deposit_count, sizeof(ResourceDeposit));
        asteroids.items[i].deposits.count = 0;
    }
    return &asteroids;
}

