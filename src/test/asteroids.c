#include "../data/game_settings.h"
#include "../game/asteroid.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char* buffer;
    int x_max; // Internal available width for drawing
    int y_max; // Internal available height for drawing
    int stride; // The number of characters in a buffer row
    int rows; // The total number of rows in the buffer
} Grid;

/**
 * Fill a grid with the given width and height
 * 
 * @param width The width of the grid
 * @param height The height of the grid
 * @return The filled grid
 */
Grid create_grid( int width, int height) {
    const int stride = width + 3; // 2 borders + 1 newline
    const int rows = height + 2; // 2 borders
    char* buffer = (char*)malloc(stride * rows * sizeof(char) + 1); // +1 for the null terminator

    if(!buffer) {
        fprintf(stderr, "Failed to allocate memory for the grid\n");
        exit(1);
    }
    
    // Fill with spaces first
    memset(buffer, ' ', stride * rows * sizeof(char));

    // Top and bottom borders at row 0 and row rows-1 (not row height: that is the
    // last *content* row when rows == height + 2).
    for(int b = 0; b < 2; b++) {
        const int i = (b == 0) ? 0 : rows - 1;
        const int offset = i * stride;
        buffer[offset] = '+';
        buffer[offset + stride - 2] = '+';
        buffer[offset + stride - 1] = '\n';
        memset(buffer + offset + 1, '-', width * sizeof(char));
    }

    // Inner lines: one full-height band of playable rows between the borders
    for(int i = 1; i <= height; i++) {
        const int offset = i * stride;
        buffer[offset] = '|';
        buffer[offset + stride - 2] = '|';
        buffer[offset + stride - 1] = '\n';
    }

    // Null-terminate the buffer
    buffer[stride * rows] = '\0';

    return (Grid){
        .buffer = buffer,
        .x_max = width,
        .y_max = height,
        .stride = stride,
        .rows = rows
    };
}

typedef struct {
    enum AsteroidEffect effect;
    enum AsteroidSize size;
} DrawFilter;

void draw_asteroids(Asteroids *asteroids, int count, Grid grid, Vector sector_size, DrawFilter *filter) {
    for(Asteroid *asteroid = asteroids->items; asteroid != asteroids->items + count; asteroid++) {
        if(filter && filter->effect && asteroid->effect != filter->effect) {
            continue;
        }
        if(filter && filter->size && asteroid->size != filter->size) {
            continue;
        }

        char symbol;
        if(asteroid->effect == AST_EFFECT_PLAYER_HOME) {
            symbol = 'H';
        } else {
            switch(asteroid->size) {
                case AST_SIZE_SMALL:
                    symbol = '*';
                    break;
                case AST_SIZE_MEDIUM:
                    symbol = 'o';
                    break;
                case AST_SIZE_LARGE:
                    symbol = 'O';
                    break;
                default:
                    symbol = '?';
                    break;
            }
        }

        const int x = grid.x_max * asteroid->position.x / sector_size.x;
        const int y = grid.y_max * asteroid->position.y / sector_size.y;

        const int offset = ((y + 1) * grid.stride) + x + 1;

        if(offset < 0 || offset >= grid.stride * grid.rows) {
            printf("Asteroid out of bounds: x: %d, y: %d, offset: %d\n", x, y, offset);
            continue;
        }

        if(grid.buffer[offset] == ' ') {
            grid.buffer[offset] = symbol;
        }
    }
}

const int width = 80;
const int height = 30;

Asteroids *execute_generator(GameSettings settings) {
    bool result = asteroids_generate(&settings);
    if(!result) {
        printf("Test failed: Failed to generate asteroids\n");
        return NULL;
    }
    return asteroids_list();
}

void test_player_deposition(GameSettings settings, int iterations) {
    printf("Testing %d players deposition with %d iterations...\n", settings.player_count, iterations);
    Grid grid = create_grid(width, height);

    DrawFilter filter = {
        .effect = AST_EFFECT_PLAYER_HOME
    };

    int placement_retries = 0;
    int placement_failures = 0;
    int max_placement_retries = 0;
    int max_placement_failures = 0;
    for(int i = 0; i < iterations; i++) {
        Asteroids *asteroids = execute_generator(settings);
        AsteroidGenerationStats stats = asteroid_generation_stats();
        placement_retries += stats.placement_retries;
        placement_failures += stats.placement_failures;
        if(stats.placement_retries > max_placement_retries) {
            max_placement_retries = stats.placement_retries;
        }
        if(stats.placement_failures > max_placement_failures) {
            max_placement_failures = stats.placement_failures;
        }
        draw_asteroids(asteroids, asteroids->count, grid, settings.sector_size, &filter);
    }

    printf("%s", grid.buffer);
    printf("Placement retries: avg %d, max %d\n", (int)(placement_retries / iterations), max_placement_retries);
    printf("Placement failures: avg %d, max %d\n", (int)(placement_failures / iterations), max_placement_failures);
    printf("Test passed: %d players deposition with %d iterations\n", settings.player_count, iterations);
    printf("\n\n");
    free(grid.buffer);
}

void test_full_deposition(GameSettings settings) {
    char sector_size_name[8];
    char asteroid_density_name[8];
    int grid_width = width;
    int grid_height = height;
    switch(settings.sector_size_selection) {
        case GAME_SECTOR_SIZE_SMALL:
            strcpy(sector_size_name, "Small");
            grid_width *= 0.5;
            grid_height *= 0.5;
            break;
        case GAME_SECTOR_SIZE_MEDIUM:
            strcpy(sector_size_name, "Medium");
            break;
        case GAME_SECTOR_SIZE_LARGE:
            strcpy(sector_size_name, "Large");
            grid_width *= 1.5;
            grid_height *= 1.5;
            break;
        default:
            strcpy(sector_size_name, "Unknown");
            break;
    }
    switch(settings.asteroids.asteroid_density_selection) {
        case GAME_ASTEROID_DENSITY_LOW:
            strcpy(asteroid_density_name, "Low");
            break;
        case GAME_ASTEROID_DENSITY_MEDIUM:
            strcpy(asteroid_density_name, "Medium");
            break;
        case GAME_ASTEROID_DENSITY_HIGH:
            strcpy(asteroid_density_name, "High");
            break;
        default:
            strcpy(asteroid_density_name, "Unknown");
            break;
    }
    Grid grid = create_grid(grid_width, grid_height);

    Asteroids *asteroids = execute_generator(settings);
    AsteroidGenerationStats stats = asteroid_generation_stats();
    if(stats.placement_retries > 0) {
        printf("Placement retries: %d\n", stats.placement_retries);
    }
    if(stats.placement_failures > 0) {
        printf("Placement failures: %d\n", stats.placement_failures);
    }
    printf("Neighbours: ");
    for(int i = 0; i < settings.player_count; i++) {
        printf("[%d] %d, ", i, stats.home_asteroid_neighbour_counts[i].count);
    }
    printf("\n");
    draw_asteroids(asteroids, asteroids->count, grid, settings.sector_size, NULL);

    printf("%s", grid.buffer);
    printf("Test passed: Full asteroid deposition with %s sector size and %s asteroid density\n", sector_size_name, asteroid_density_name);
    printf("\n\n");

    free(grid.buffer);
}
/**
* Test asteroid generation
*/
int main(int argc, char *argv[]) {
    srand(time(NULL));

    GameSettingsSelection selection = {
        .difficulty = GAME_DIFFICULTY_DEFAULT,
        .sector_size_selection = GAME_SECTOR_SIZE_MEDIUM,
        .asteroid_density_selection = GAME_ASTEROID_DENSITY_MEDIUM,
    };
    for(int i = 3; i <= 8; i++) {
        GameSettings settings = game_settings(selection);
        settings.player_count = i;
        test_player_deposition(settings, 200);
    }

    for(int i = GAME_SECTOR_SIZE_SMALL; i <= GAME_SECTOR_SIZE_LARGE; i++) {
        for(int j = GAME_ASTEROID_DENSITY_LOW; j <= GAME_ASTEROID_DENSITY_HIGH; j++) {
            GameSettings settings = game_settings((GameSettingsSelection){
                .difficulty = GAME_DIFFICULTY_DEFAULT,
                .sector_size_selection = i,
                .asteroid_density_selection = j,
            });
            test_full_deposition(settings);
        }
    }

    return 0;
}
