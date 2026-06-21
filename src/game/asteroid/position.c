#include "position.h"
#include <stdlib.h>
#include <math.h>
#include "../../util/rand.h"

static SectorGrid sector_grid = {0};

/**
 * Recalculate the sector cell grid
 *
 * Places each of the asteroids into cells based on position
 */
void calculate_sector_grid(Asteroids *asteroids) {
  const int32_t SECTOR_CELL_SIZE = 1000; // Size of each cell in the grid
  // Initial allocation
  if(sector_grid.asteroid_count == 0) {
    sector_grid.asteroids = (Asteroid*)calloc(asteroids->count, sizeof(&asteroids));
    sector_grid.asteroid_count = asteroids->count;
    // Resize allocation if asteroid count rises
  } else if(sector_grid.asteroid_count < asteroids->count) {
    sector_grid.asteroids = (Asteroid*)realloc(sector_grid.asteroids, asteroids->count * sizeof(Asteroid));
    sector_grid.asteroid_count = asteroids->count;
  }

  // Clear the grid
  for(GridCell *cell = sector_grid.cells; cell < sector_grid.cells + sector_grid.cell_count; cell++) {
    cell->count = 0;
  }


  // Place each asteroid into a cell
  for(Asteroid *asteroid = asteroids->items; asteroid < asteroids->items + asteroids->count; asteroid++) {
    const int32_t x = asteroid->position.x / SECTOR_CELL_SIZE;
    const int32_t y = asteroid->position.y / SECTOR_CELL_SIZE;
  }
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
static bool is_valid_asteroid_position(Asteroids* asteroids, Vector p, int32_t min) {
    const int32_t min_x = p.x - min;
    const int32_t min_y = p.y - min;
    const int32_t max_x = p.x + min;
    const int32_t max_y = p.y + min;
    const int64_t min_squared = min * min;
    for(Asteroid *asteroid = asteroids->items; asteroid < asteroids->items + asteroids->count; asteroid++) {
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

/**
 * Find a valid candidate position for an asteroid within bounds
 */
bool find_position_within_bounds(Asteroids *asteroids, Vector *position, AsteroidPlacementBounds bounds, uint32_t *attempts) {
    while(attempts > 0) {
        position->x = prng(bounds.max.x - bounds.min.x) + bounds.min.x;
        position->y = prng(bounds.max.y - bounds.min.y) + bounds.min.y;
        if(is_valid_asteroid_position(asteroids, *position, bounds.min_distance)) {
            return true;
        }
        attempts--;
    }
    position->x = 0;
    position->y = 0;
    return false;
}

/**
 * Find a valid candidate position within a radius of a point
 */
bool find_position_within_distance(Asteroids *asteroids, Vector *position, Vector center, int32_t min_distance, int32_t max_distance, uint32_t *attempts) {
    while(attempts > 0) {
        const int32_t distance = prng_range(min_distance, max_distance);
        const float angle = prng(360) / 360.0f * M_PI * 2;
        position->x = (int32_t)(center.x + cos(angle) * distance);
        position->y = (int32_t)(center.y + sin(angle) * distance);
        if(is_valid_asteroid_position(asteroids, *position, min_distance)) {
            return true;
        }
        attempts--;
    }
    position->x = 0;
    position->y = 0;
    return false;
}
