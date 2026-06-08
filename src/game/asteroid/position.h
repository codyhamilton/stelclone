#ifndef ASTEROID_POSITION_H
#define ASTEROID_POSITION_H

#include "../asteroid.h"

/**
 * A boundary definition, defining the bounding box and min distance
 */
typedef struct {
    Vector min; // Minimum position
    Vector max; // Maximum position
    int32_t min_distance; // Minimum distance between asteroids
} AsteroidPlacementBounds;

typedef struct {
  uint16_t start; // Asteroid index this cell starts at
  uint16_t count; // Number of items in the cell
} GridCell;

typedef struct {
  Asteroid *asteroids;
  GridCell *cells;
  uint32_t stride;
  uint32_t cell_count;
  uint16_t asteroid_count;
} SectorGrid;

/**
 * Recalculate the sector cell grid
 */
void calculate_sector_grid(Asteroids *asteroids);

/**
 * Find candicate position weithin a radius of a point
 */
bool find_position_within_bounds(Asteroids *asteroids, Vector *position, AsteroidPlacementBounds bounds, uint32_t *attempts);

/**
 * Find a valid candidate position within a radius of a point
 */
bool find_position_within_distance(Asteroids *asteroids, Vector *position, Vector center, int32_t min_distance, int32_t max_distance, uint32_t *attempts);

#endif
