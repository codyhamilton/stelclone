#include "asteroid.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../util/log.h"
#include "../util/rand.h"

static const float ASTEROID_BACKFILL_PERCENTAGE = 0.2f; // 20% of asteroids are reserved to backfill under-populated home regions

static Asteroids asteroids;
static AsteroidGenerationStats stats;

void updateAsteroids() { }

static void asteroids_free() {
    if(!asteroids.items) {
        return;
    }
    for(uint16_t i = 0; i < asteroids.count; i++) {
        free(asteroids.items[i].deposits.items);
    }
    free(asteroids.items);
    asteroids.items = NULL;
    asteroids.count = 0;
}

/**
 * Get the list of asteroids
 * 
 * @return The list of asteroids
 */
Asteroids *asteroids_list() {
    return &asteroids;
}

AsteroidGenerationStats asteroid_generation_stats() {
    return stats;
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

typedef struct {
    Vector min; // Minimum position
    Vector max; // Maximum position
    int32_t min_distance; // Minimum distance between asteroids
} AsteroidPlacementBounds;

static const int max_placement_attempts = 100;

static bool find_position_within_bounds(Vector *position, AsteroidPlacementBounds bounds) {
    for(int attempts = 0; attempts < max_placement_attempts; attempts++) {
        position->x = prng(bounds.max.x - bounds.min.x) + bounds.min.x;
        position->y = prng(bounds.max.y - bounds.min.y) + bounds.min.y;
        if(is_valid_asteroid_position(*position, bounds.min_distance)) {
            return true;
        }
        stats.placement_retries++;
    }
    position->x = 0;
    position->y = 0;
    return false;
}

static bool find_position_within_distance(Vector *position, Vector center, int32_t min_distance, int32_t max_distance) {
    for(int attempts = 0; attempts < max_placement_attempts; attempts++) {
        const int32_t distance = prng_range(min_distance, max_distance);
        const float angle = prng(360) / 360.0f * M_PI * 2;
        position->x = (int32_t)(center.x + cos(angle) * distance);
        position->y = (int32_t)(center.y + sin(angle) * distance);
        if(is_valid_asteroid_position(*position, min_distance)) {
            return true;
        }
        stats.placement_retries++;
    }
    position->x = 0;
    position->y = 0;
    return false;
}

/**
 * Place the home asteroids
 * 
 * This places the home asteroids in the sector, and gives each home asteroid at least N directly reachable asteroids
 *
 * @param settings The settings for the asteroid generation
 * @return True if the home asteroids were placed successfully, false otherwise
 */
static bool place_home_asteroids(GameSettings *settings) {
    static const float strata_buffer_percentage = 0.2f;

    // We're dividing the sector into two rows, sizing cells based on player count.
    // E.g. for 2 players, cols 1+1. 3 = 2+1. 4 = 2+2. 5 = 3+2. 6 = 3+3. etc.
    const int32_t row_height = settings->sector_size.y / 2;
    for(uint8_t i = 0; i < settings->player_count; i++) {
        // Calculate the strata position and colwidth for this player
        const uint8_t offset = i % 2; // 0 for even, 1 for odd
        const uint8_t cells = (settings->player_count + 1 - offset) / 2;
        const int32_t cell_width = settings->sector_size.x / cells; // cell width

        const int32_t x_buffer = cell_width * strata_buffer_percentage;
        const int32_t y_buffer = row_height * strata_buffer_percentage;

        const Vector start = (Vector){
            .x = cell_width * (i - offset) / 2 + x_buffer,
            .y = offset * row_height + y_buffer
        };
        const Vector end = (Vector){
            .x = start.x + cell_width - (2 * x_buffer),
            .y = start.y + row_height - (2 * y_buffer)
        };
        const AsteroidPlacementBounds bounds = (AsteroidPlacementBounds){
            .min = start,
            .max = end,
            .min_distance = settings->asteroids.min_distance_between_asteroids
        };

        // Now we have centre points for the strata, we can randomly place home asteroids in their strata
        Asteroid *asteroid = &asteroids.items[asteroids.count];
        if(find_position_within_bounds(&asteroid->position, bounds)) {
            asteroid->effect = AST_EFFECT_PLAYER_HOME;
            asteroids.count++;
        } else {
            LOG_DEBUG("Home asteroid not placed after %d attempts\n", max_placement_attempts);
            return false;
        }
    }
    return true;
}

static bool place_reachable_asteroids(GameSettings *settings, uint8_t player_count) {
    for(uint8_t i = 0; i < player_count; i++) {
        Asteroid *home_asteroid = &asteroids.items[i];
        // Give each home asteroid at least N directly reachable asteroids
        for(uint8_t j = 0; j < settings->asteroids.min_home_reachable_asteroids; j++) {
            Asteroid *reachable_asteroid = &asteroids.items[asteroids.count];
            if(find_position_within_distance(&reachable_asteroid->position, home_asteroid->position, settings->asteroids.min_distance_between_asteroids, settings->asteroids.max_home_reachable_distance)) {
                asteroids.count++;
            } else {
                LOG_DEBUG("Reachable asteroid not placed after %d attempts\n", max_placement_attempts);
                return false;
            }
        }
    }
    return true;
}

static bool place_random_asteroids(AsteroidPlacementBounds bounds, uint16_t count) {
    int16_t target_index = asteroids.count + count;
    for(Asteroid *asteroid = asteroids.items + asteroids.count; asteroid != asteroids.items + target_index; asteroid++) {
        if(!find_position_within_bounds(&asteroid->position, bounds)) {
            return false;
        }
        asteroids.count++;
    }
    return true;
}

static int16_t calculate_nearest_home_asteroid_index(Vector position) {
    int32_t min_distance = INT32_MAX;
    int min_index = -1;
    for(int j = 0; j < asteroids.count && asteroids.items[j].effect == AST_EFFECT_PLAYER_HOME; j++) {
        const Asteroid *candidate_asteroid = &asteroids.items[j];
        const int32_t dx = position.x - candidate_asteroid->position.x;
        const int32_t dy = position.y - candidate_asteroid->position.y;
        const int32_t distance = dx * dx + dy * dy;
        if(distance < min_distance) {
            min_distance = distance;
            min_index = j;
        }
    }
    return min_index;

}

static void calculate_home_asteroid_neighbour_counts(HomeAsteroidNeighbourCount *counts, uint8_t player_count) {
    for(uint8_t i = 0; i < player_count; i++) {
        counts[i].home_asteroid = &asteroids.items[i];
        counts[i].count = 0;
    }

    for(Asteroid *asteroid = asteroids.items; asteroid != asteroids.items + asteroids.count; asteroid++) {
        if(asteroid->effect == AST_EFFECT_PLAYER_HOME) {
            continue;
        }
        const int16_t nearest_home_asteroid_index = calculate_nearest_home_asteroid_index(asteroid->position);
        if(nearest_home_asteroid_index >= 0) {
            counts[nearest_home_asteroid_index].count++;
        }
    }
}

/**
 * Compare two home asteroid neighbour counts
 *
 * Sort from smallest to largest
 * 
 * @param a The first home asteroid neighbour count
 * @param b The second home asteroid neighbour count
 * @return -1 if a is less than b, 1 if a is greater than b, 0 if they are equal
 */
static int compare_counts(const void *a, const void *b) {
  const HomeAsteroidNeighbourCount *count_a = (const HomeAsteroidNeighbourCount *)a;
  const HomeAsteroidNeighbourCount *count_b = (const HomeAsteroidNeighbourCount *)b;
  return count_a->count - count_b->count;
}

/**
 * Backfill the home regions
 * 
 * This places the remaining asteroids in a way that balances out home regions (somewhat)
 *
 * @param counts neighbour counts for each home asteroid
 * @param player_count The number of players
 * @return True if the home regions were backfilled successfully, false otherwise
 */
static bool backfill_home_regions(HomeAsteroidNeighbourCount *counts, GameSettings *settings) {
  qsort(counts, settings->player_count, sizeof(HomeAsteroidNeighbourCount), compare_counts);

  const AsteroidPlacementBounds bounds = (AsteroidPlacementBounds){
    .min = (Vector){ .x = 0, .y = 0 },
    .max = settings->sector_size,
    .min_distance = settings->asteroids.min_distance_between_asteroids
  };

  for(int i = asteroids.count; i < settings->asteroids.asteroid_count; i++) {
    Asteroid *asteroid = &asteroids.items[i];
    Vector position = (Vector){0, 0};
    // We round robin through home asteroids with the smallest count until all asteroids have been placed
    for(int j = 0; j < settings->player_count && counts[j].count <= counts[0].count; j++) {
        for(int k = 0; k < 200; k++) {
            if(!find_position_within_bounds(&position, bounds)) {
                return false;
            }
            if(&asteroids.items[calculate_nearest_home_asteroid_index(position)] == counts[j].home_asteroid) {
                asteroid->position = position;
                asteroids.count++;
                counts[j].count++;
                break;
            }
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
    LOG_DEBUG("Generating asteroids for %d players\n", settings->player_count);
    static const int max_attempts = 10;

    // Die if we have too few or too many players, or if we don't have enough asteroids to place all the home asteroids
    if(settings->player_count < 2 || settings->player_count > 8 || settings->player_count * 3 > settings->asteroids.asteroid_count) {
        LOG_DEBUG("Asteroids generation failed: Invalid player count: %d\n", settings->player_count);
        return false;
    }

    if(asteroids.items) {
        asteroids_free();
    }
    stats = (AsteroidGenerationStats){0};

    const AsteroidPlacementBounds bounds = (AsteroidPlacementBounds){
        .min = (Vector){ .x = 0, .y = 0 },
        .max = settings->sector_size,
        .min_distance = settings->asteroids.min_distance_between_asteroids
    };

    // Reset initial state
    asteroids.items = (Asteroid*)calloc(settings->asteroids.asteroid_count, sizeof(Asteroid));
    if(!asteroids.items) {
        LOG_DEBUG("Asteroids generation failed: Out of memory\n");
        return false;
    }

    // Outer retry loop for placement of asteroids
    for(int attempts = 0; attempts < max_attempts; attempts++) {
        asteroids.count = 0;
        if(!place_home_asteroids(settings)) {
            stats.placement_failures++;
            continue;
        }
        if(!place_reachable_asteroids(settings, settings->player_count)) {
            stats.placement_failures++;
            continue;
        }

        // Place (1 - ASTEROID_BACKFILL_PERCENTAGE) of the remaining asteroids randomly
        const uint16_t remaining_asteroids = settings->asteroids.asteroid_count - asteroids.count;
        if(!place_random_asteroids(bounds, (uint16_t)(remaining_asteroids * (1.0f - ASTEROID_BACKFILL_PERCENTAGE)))) {
            stats.placement_failures++;
            continue;
        }

        // Now we will do some targeted positioning, using remaining asteroids to balance out home regions
        // In this step we will ignore failures and just place as many as we can
        calculate_home_asteroid_neighbour_counts(stats.home_asteroid_neighbour_counts, settings->player_count);
        if(!backfill_home_regions(stats.home_asteroid_neighbour_counts, settings)) {
            stats.placement_failures++;
            continue;
        }

        break;
    }
    if(asteroids.count == 0) {
        LOG_DEBUG("Home asteroids not placed after %d attempts\n", max_attempts);
        asteroids_free();
        return false;
    }

    // Apply effects, sizes and remaining properties to placed asteroids
    for(Asteroid *asteroid = asteroids.items; asteroid != asteroids.items + settings->asteroids.asteroid_count; asteroid++) {
        // Apply effects
        if(asteroid->effect == AST_EFFECT_PLAYER_HOME) {
            asteroid->size = AST_SIZE_MEDIUM;
            asteroid->habitable_space = 100;
            asteroid->speed = (Vector){
                .x = 0,
                .y = 0,
            };
            asteroid->acceleration = (Vector){
                .x = 0,
                .y = 0,
            };
        } else {
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
