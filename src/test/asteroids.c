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

void draw_asteroids(Asteroids *asteroids, int count, Grid grid, Vector sector_size) {
    for(Asteroid *asteroid = asteroids->items; asteroid != asteroids->items + count; asteroid++) {
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
        grid.buffer[((y + 1) * grid.stride) + x + 1] = symbol;
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

void test_player_deposition(int iterations, int player_count) {
    printf("Testing %d players deposition with %d iterations...\n", player_count, iterations);
    Grid grid = create_grid(width, height);

    GameSettings settings = game_settings((GameSettingsSelection){
        .difficulty = GAME_DIFFICULTY_DEFAULT,
        .sector_size_selection = GAME_SECTOR_SIZE_MEDIUM,
        .asteroid_density_selection = GAME_ASTEROID_DENSITY_MEDIUM,
    });

    settings.player_count = player_count;

    for(int i = 0; i < iterations; i++) {
        Asteroids *asteroids = execute_generator(settings);
        draw_asteroids(asteroids, settings.player_count, grid, settings.sector_size);
    }

    printf("%s", grid.buffer);
    printf("Test passed: %d players deposition with %d iterations\n", player_count, iterations);
    printf("\n\n");
    free(grid.buffer);
}

void test_full_deposition(GameSettings settings, int player_count) {
    printf("Testing %d players deposition...\n", player_count);
    Grid grid = create_grid(width, height);

    settings.player_count = player_count;

    Asteroids *asteroids = execute_generator(settings);
    draw_asteroids(asteroids, asteroids->count, grid, settings.sector_size);

    printf("%s", grid.buffer);
    printf("Test passed: %d players deposition\n", player_count);
    printf("\n\n");
    free(grid.buffer);
}
/**
* Test asteroid generation
*/
int main(int argc, char *argv[]) {
    srand(time(NULL));

    for(int i = 2; i <= 8; i++) {
        test_player_deposition(1, i);
        test_player_deposition(200, i);

        GameSettings settings = game_settings((GameSettingsSelection){
            .difficulty = GAME_DIFFICULTY_DEFAULT,
            .sector_size_selection = GAME_SECTOR_SIZE_MEDIUM,
            .asteroid_density_selection = GAME_ASTEROID_DENSITY_MEDIUM,
        });

        test_full_deposition(settings, i);
    }

    return 0;
}
