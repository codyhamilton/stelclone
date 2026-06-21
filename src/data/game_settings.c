#include "game_settings.h"
#include <string.h>

static const GameSettings default_settings = {
    .sector_size = { 5000, 4000 },
    .sector_size_selection = GAME_SECTOR_SIZE_MEDIUM,
    .difficulty_selection = GAME_DIFFICULTY_DEFAULT,
    .player_count = 3,
    .colony = {
        .sensor_range = 200,
    },
    .asteroids = {
        .asteroid_count = 100,
        .effect_chance = 20,
        .asteroid_density_selection = GAME_ASTEROID_DENSITY_MEDIUM,
        .min_distance_between_asteroids = 50,
        .min_home_reachable_asteroids = 2,
        .max_home_reachable_distance = 400,
        .large = { 
            .speed_modifier = 1.0, 
            .multiplier = 10,
            .frontier = { .chance = 0.75, .quantity_base = 150, .quantity_stddev = 50 },
            .exotic = { .chance = 0.3, .quantity_base = 20, .quantity_stddev = 10 },
            .restricted = { .chance = 0.1, .quantity_base = 30, .quantity_stddev = 15 },
        },
        .medium = { 
            .speed_modifier = 1.0, 
            .multiplier = 10,
            .frontier = { .chance = 0.5, .quantity_base = 10, .quantity_stddev = 5 },
            .exotic = { .chance = 0.3, .quantity_base = 20, .quantity_stddev = 10 },
            .restricted = { .chance = 0.1, .quantity_base = 30, .quantity_stddev = 15 },
        },
        .small = { 
            .speed_modifier = 1.0, 
            .multiplier = 10,
            .frontier = { .chance = 0.5, .quantity_base = 10, .quantity_stddev = 5 },
            .exotic = { .chance = 0.3, .quantity_base = 20, .quantity_stddev = 10 },
            .restricted = { .chance = 0.2, .quantity_base = 30, .quantity_stddev = 15 },
        },
        .player = { 
            .speed_modifier = 0.0, 
            .multiplier = 10,
            .frontier = { .chance = 1.0, .quantity_base = 100, .quantity_stddev = 10 },
            .exotic = { .chance = 1.0, .quantity_base = 50, .quantity_stddev = 10 },
            .restricted = { .chance = 0.0, .quantity_base = 0, .quantity_stddev = 0 },
        },
    },
};

GameSettings game_settings(GameSettingsSelection selection) {
    GameSettings settings;
    memcpy(&settings, &default_settings, sizeof(GameSettings));
    settings.difficulty_selection = selection.difficulty;
    settings.sector_size_selection = selection.sector_size_selection;
    settings.asteroids.asteroid_density_selection = selection.asteroid_density_selection;
    switch(selection.difficulty) {
        case GAME_DIFFICULTY_EASY:
            settings.asteroids.player.frontier.quantity_base *= 2;
            settings.asteroids.player.exotic.quantity_base *= 2;
            break;
        case GAME_DIFFICULTY_HARD:
            settings.asteroids.player.frontier.quantity_base *= 0.75;
            settings.asteroids.player.exotic.quantity_base *= 0.75;
            break;
        case GAME_DIFFICULTY_BRUTAL:
            settings.asteroids.player.frontier.quantity_base *= 0.5;
            settings.asteroids.player.exotic.quantity_base *= 0.5;
            break;
        default:
            break;
    }
    switch(selection.sector_size_selection) {
        case GAME_SECTOR_SIZE_SMALL:
            settings.sector_size.x *= 0.5;
            settings.sector_size.y *= 0.5;
            break;
        case GAME_SECTOR_SIZE_LARGE:
            settings.sector_size.x *= 1.5;
            settings.sector_size.y *= 1.5;
            break;
        default:
            break;
    }
    switch(selection.asteroid_density_selection) {
        case GAME_ASTEROID_DENSITY_LOW:
            settings.asteroids.asteroid_count *= 0.5;
            break;
        case GAME_ASTEROID_DENSITY_HIGH:
            settings.asteroids.asteroid_count *= 2;
            break;
        default:
            break;
    }
    return settings;
}