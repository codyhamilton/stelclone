#include <SDL3/SDL.h>
#ifndef GAME_H_
#define GAME_H_

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_FPoint pos;
    SDL_FPoint speed;
} GameState;

void update(GameState *gameState);

#endif