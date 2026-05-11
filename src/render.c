#include "render.h"
#include "SDL3/SDL_blendmode.h"
#include "SDL3/SDL_render.h"
#include <SDL3/SDL.h>
#include <stdint.h>

static const float GRID_SCALE = 0.8f;
static const unsigned int SECTOR_SIZE = 14; // 2px reserved for lines
static const unsigned int SECTOR_BORDER_SIZE = 1;
static const SDL_Color SECTOR_BORDER_COLOR = {64, 64, 64, 255};
static const SDL_Color SECTOR_SHADOW_COLOR = {32, 32, 32, 255}; // light gray with alpha

static SDL_Texture *grid_texture = NULL;

/**
 * Draws a the main sector map grid to a texture
 *
 * The grid is sector_columns x sector_columns sectors. The grid lines have 1px shadow
 * @param renderer The renderer to draw on
 * @param width The width of the grid
 * @param height The height of the grid
 */
SDL_Texture *create_grid_texture(SDL_Renderer *renderer, unsigned int grid_width) {
    const SDL_FRect grid_rect = {0, 0, grid_width, grid_width};

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, grid_width, grid_width);
    SDL_SetRenderTarget(renderer, texture);

    // Draw a black background for the grid
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &grid_rect);

    // Draw stars with random alpha on the grid
    for (int x = 0; x < grid_width; x++) {
        const int x = SDL_rand(grid_width);
        const int y = SDL_rand(grid_width);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_rand(255));
        SDL_RenderPoint(renderer, x, y);
    }

    // Draw the shadow lines (1px offset, gray)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, SECTOR_SHADOW_COLOR.r, SECTOR_SHADOW_COLOR.g, SECTOR_SHADOW_COLOR.b, SECTOR_SHADOW_COLOR.a);
    for (int x = 1; x <= grid_width + 1; x += SECTOR_SIZE) {
        SDL_RenderLine(renderer, 0, x, grid_width, x);
        SDL_RenderLine(renderer, x, 0, x, grid_width);
    }

    // Then draw the main lines over it, white
    SDL_SetRenderDrawColor(renderer, SECTOR_BORDER_COLOR.r, SECTOR_BORDER_COLOR.g, SECTOR_BORDER_COLOR.b, SECTOR_BORDER_COLOR.a);
    for (int x = 0; x <= grid_width; x += SECTOR_SIZE) {
        SDL_RenderLine(renderer, 0, x, grid_width, x);
        SDL_RenderLine(renderer, x, 0, x, grid_width);
    }

    SDL_SetRenderTarget(renderer, NULL);
    return texture;
}

void render_grid(GameState *gameState) {
    static const unsigned int sector_columns = 20;
    static const unsigned int grid_width = sector_columns * SECTOR_SIZE;

    if(grid_texture == NULL) {
        grid_texture = create_grid_texture(gameState->renderer, grid_width);
    }

    // Get the window size
    int window_width, window_height;
    SDL_GetWindowSize(gameState->window, &window_width, &window_height);

    //
    float scale = ((float)window_height * GRID_SCALE) / (float)grid_width;
    SDL_FRect dstrect = {0, 0, grid_width * scale, grid_width * scale};
    SDL_RenderTexture(gameState->renderer, grid_texture, NULL, &dstrect);
}

void render(GameState *gameState) {
    // Clear the screen
    SDL_SetRenderDrawColor(gameState->renderer, 0, 0, 0, 255);
    SDL_RenderClear(gameState->renderer);

    // Draw
    render_grid(gameState);
    SDL_RenderPresent(gameState->renderer);
}
