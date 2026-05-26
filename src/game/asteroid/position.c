#include "../asteroid.h"

bool is_valid_asteroid_position(Asteroids* asteroids, Vector p, int32_t min) {
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
