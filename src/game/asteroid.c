#include "asteroid.h"
#include <math.h>
#include <stdlib.h>
#include "../util/log.h"
#include "../util/rand.h"

static Asteroids asteroids;

void updateAsteroids() { }

static void asteroids_free() {
    for(uint16_t i = 0; i < asteroids.count; i++) {
        free(asteroids.items[i].deposits.items);
    }
    free(asteroids.items);
}


/**
 * Get the list of asteroids
 * 
 * @return The list of asteroids
 */
Asteroids *asteroids_list() {
    return &asteroids;
}

/**
 * Get an asteroid by its id
 * 
 * @param id The id of the asteroid to get
 * @return The asteroid
 */
Asteroid *asteroid_get(uint16_t id) {
    for(uint8_t i = 0; i < asteroids.count; i++) {
        if(asteroids.items[i].id == id) {
            return &asteroids.items[i];
        }
    }
    return NULL;
}

/**
 * Check if a position is valid for an asteroid
 * 
 * This checks if the position is within the minimum distance between asteroids
 * it will check against all already placed asteroids
 *
 * @param p The position to check
 * @param min The minimum distance between asteroids
 * @return True if the position is valid, false otherwise
 */
static bool is_valid_asteroid_position(Vector p, int32_t min) {
    const int32_t min_x = p.x - min;
    const int32_t min_y = p.y - min;
    const int32_t max_x = p.x + min;
    const int32_t max_y = p.y + min;
    const int64_t min_squared = min * min;
    for(Asteroid *asteroid = asteroids.items; asteroid < asteroids.items + asteroids.count; asteroid++) {
        Vector a = asteroid->position;
        if(a.x >= min_x && a.x <= max_x && a.y >= min_y && a.y <= max_y) {
            const int64_t dx = a.x - p.x;
            const int64_t dy = a.y - p.y;
            if(dx * dx + dy * dy < min_squared) {
                return false;
            }
        }   
    }
    return true;
}

static bool place_home_asteroids(GameSettings *settings) {
    const int8_t max_placement_attempts = 100;
    const int32_t row_height = settings->sector_size.y / 2;
    // We're dividing the sector into two rows, sizing cells based on player count.
    // E.g. for 2 players, cols 1+1. 3 = 2+1. 4 = 2+2. 5 = 3+2. 6 = 3+3. etc.
    for(uint8_t i = 0; i < settings->player_count; i++) {
        // Calculate the strata position and colwidth for this player
        const uint8_t offset = i % 2; // 0 for even, 1 for odd
        const int32_t colwidth = settings->sector_size.x / ((settings->player_count + 1 - offset) / 2); // cell width
        const int32_t strata_y = (row_height / 2) + (offset * row_height); // cell y position
        const int32_t strata_x = (colwidth / 2) + (colwidth * (i - offset) / 2); // cell x position

        // Now we have centre points for the strata, we can randomly place home asteroids in their strata
        Asteroid *asteroid = &asteroids.items[asteroids.count++];
        const int32_t col_range = colwidth * 3 / 4;
        const int32_t row_range = row_height * 3 / 4;
        const int32_t start_x = strata_x - (col_range / 2);
        const int32_t start_y = strata_y - (row_range / 2);
        bool home_placed = false;
        // We'll retry up to max_placement_attempts times to place the home asteroid
        for(int attempts = 0; attempts < max_placement_attempts; attempts++) {
            asteroid->position.x = start_x + prng(col_range);
            asteroid->position.y = start_y + prng(row_range);
            if(is_valid_asteroid_position(asteroid->position, settings->asteroids.min_distance_between_asteroids)) {
                home_placed = true;
                break;
            }
        }
        if(!home_placed) {
            // Exit to outer retry loop to start from scratch or die
            return false;
        }

        // Success, set other properties
        asteroid->effect = AST_EFFECT_PLAYER_HOME;
        asteroid->size = AST_SIZE_MEDIUM;
        asteroid->speed.x = 0;
        asteroid->speed.y = 0;

        // Give each home asteroid at least N directly reachable asteroids
        for(uint8_t j = 0; j < settings->asteroids.min_home_reachable_asteroids; j++) {
            Asteroid *reachable_asteroid = &asteroids.items[asteroids.count];
            bool reachable_placed = false;
            for(int attempts = 0; attempts < max_placement_attempts; attempts++) {
                const int32_t distance = prng_range(settings->asteroids.min_distance_between_asteroids, settings->asteroids.max_home_reachable_distance);
                const float angle = prng(360) / 360.0f * M_PI * 2;
                const Vector position = (Vector){
                    .x = (int32_t)(asteroid->position.x + cos(angle) * distance),
                    .y = (int32_t)(asteroid->position.y + sin(angle) * distance),
                };
                if(is_valid_asteroid_position(position, settings->asteroids.min_distance_between_asteroids)) {
                    reachable_asteroid->position = position;
                    asteroids.count++;
                    reachable_placed = true;
                    break;
                }
            }
            if(!reachable_placed) {
                // Exit to outer retry loop to start from scratch or die
                return false;
            }
        }
    }
    return true;
}

/**
 * Generate the asteroids
 * 
 * @param settings The settings for the asteroid generation
 */
bool asteroids_generate(GameSettings *settings) {
    // Die if we have too few or too many players, or if we don't have enough asteroids to place all the home asteroids
    if(settings->player_count < 2 || settings->player_count > 8 || settings->player_count * 3 > settings->asteroids.asteroid_count) {
        LOG_DEBUG("Asteroids generation failed: Invalid player count: %d\n", settings->player_count);
        return false;
    }

    for(int attempts = 0; attempts < 10; attempts++) {
        if(asteroids.count > 0) {
            asteroids_free();
        }

        // Create the asteroids
        asteroids.items = (Asteroid*)calloc(settings->asteroids.asteroid_count, sizeof(Asteroid));
        asteroids.count = 0;

        if(!place_home_asteroids(settings)) {
            continue;
        };

        for(Asteroid *asteroid = asteroids.items + settings->player_count; asteroid != asteroids.items + asteroids.count; asteroid++) {
            // Apply effects
            if(prng(100) < settings->asteroids.effect_chance) {
                asteroid->effect = (enum AsteroidEffect)(prng_range(AST_EFFECT_START, AST_EFFECT_END));
            }

            // Set asteroid size
            asteroid->size = AST_SIZE_SMALL;
            const uint8_t size_index = prng(settings->asteroids.large.multiplier + settings->asteroids.medium.multiplier + settings->asteroids.small.multiplier);
            if(size_index < settings->asteroids.large.multiplier) {
                asteroid->size = AST_SIZE_LARGE;
            } else if(size_index < settings->asteroids.large.multiplier + settings->asteroids.medium.multiplier) {
                asteroid->size = AST_SIZE_MEDIUM;
            }
        }

    }
    return true;
}