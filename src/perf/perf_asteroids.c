#include "../game/asteroid.h"
#include "../data/game_settings.h"

int main(int argc, char *argv[]) {
    GameSettings settings = game_settings((GameSettingsSelection){
        .difficulty = GAME_DIFFICULTY_DEFAULT,
        .sector_size_selection = GAME_SECTOR_SIZE_MEDIUM,
        .asteroid_density_selection = GAME_ASTEROID_DENSITY_MEDIUM,
    });
    for(int i = 0; i < 1000000; i++) {
        asteroids_generate(&settings);
    }
}