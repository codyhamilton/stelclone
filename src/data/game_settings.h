#ifndef GAME_SETTINGS_H_
#define GAME_SETTINGS_H_

#include "game_types.h"
#include <stdint.h>

#define GAME_MAX_ASTEROIDS UINT8_MAX // Maximum number of asteroids in game at any time
#define GAME_MAX_PLAYERS 8 // Maximum number of players in game at any time

enum GameSettingsDifficulty {
    GAME_DIFFICULTY_EASY,
    GAME_DIFFICULTY_DEFAULT,
    GAME_DIFFICULTY_HARD,
    GAME_DIFFICULTY_BRUTAL,
    GAME_DIFFICULTY_CUSTOM,
};

enum GameSettingsSectorSize {
    GAME_SECTOR_SIZE_SMALL,
    GAME_SECTOR_SIZE_MEDIUM,
    GAME_SECTOR_SIZE_LARGE,
    GAME_SECTOR_SIZE_CUSTOM,
};

enum GameSettingsAsteroidDensity {
    GAME_ASTEROID_DENSITY_CUSTOM,
    GAME_ASTEROID_DENSITY_LOW,
    GAME_ASTEROID_DENSITY_MEDIUM,
    GAME_ASTEROID_DENSITY_HIGH,
};

/**
 * Game asteroid settings
 * 
 * The settings for the asteroid generation.
 */
typedef struct {
    float chance;
    float quantity_base;
    float quantity_stddev;
} GameAsteroidDepositSettings;

typedef struct {
    float speed_modifier;
    GameAsteroidDepositSettings frontier;
    GameAsteroidDepositSettings exotic;
    GameAsteroidDepositSettings restricted;
    uint8_t multiplier;
} GameAsteroidSizeSettings;

typedef struct GameAsteroidSettings {
    GameAsteroidSizeSettings large;
    GameAsteroidSizeSettings medium;
    GameAsteroidSizeSettings small;
    GameAsteroidSizeSettings player; // Player initial home Asteroids
    enum GameSettingsAsteroidDensity asteroid_density_selection;
    uint16_t asteroid_count; // Number of asteroids
    uint8_t effect_chance; // 0-100 Chance of an asteroid having an effect
    uint16_t min_distance_between_asteroids; // Minimum distance between asteroids
    uint8_t min_home_reachable_asteroids; // Minimum number of reachable asteroids for a home asteroid
    uint16_t max_home_reachable_distance; // Maximum distance between a home asteroid and a reachable asteroid
} GameAsteroidSettings;

typedef struct {
    uint16_t sensor_range;
} GameColonySettings;

typedef struct {
    GameColonySettings colony;
    GameAsteroidSettings asteroids;
    Vector sector_size;
    enum GameSettingsSectorSize sector_size_selection;
    enum GameSettingsDifficulty difficulty_selection;
    uint8_t player_count;
} GameSettings;

typedef struct {
    enum GameSettingsDifficulty difficulty;
    enum GameSettingsSectorSize sector_size_selection;
    enum GameSettingsAsteroidDensity asteroid_density_selection;
} GameSettingsSelection;

GameSettings game_settings(GameSettingsSelection selection);

#endif // GAME_SETTINGS_H_