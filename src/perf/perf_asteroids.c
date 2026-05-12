#include "../game/asteroid.h"

int main(int argc, char *argv[]) {
    GameAsteroidSettings settings = {
        .asteroid_count = 200,
        .sector_size = { 100, 100 },
        .asteroid_speed_modifier = 1.0,
        .asteroid_size_modifier = 1.0,
        .common_desposit_chance = 0.5,
        .common_deposit_quantity_base = 10,
        .common_deposit_quantity_stddev = 5,
        .uncommon_desposit_chance = 0.3,
        .uncommon_deposit_quantity_base = 20,
        .uncommon_deposit_quantity_stddev = 10,
        .rare_desposit_chance = 0.1,
        .rare_deposit_quantity_base = 30,
        .rare_deposit_quantity_stddev = 15,
        .strategic_desposit_chance = 0.05,
        .strategic_deposit_quantity_base = 40,
        .strategic_deposit_quantity_stddev = 20,
    };
    for(int i = 0; i < 1000000; i++) {
        asteroids_generate(&settings);
    }
}