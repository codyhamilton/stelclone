#include <SDL3/SDL.h>
#ifndef GAME_H_
#define GAME_H_

enum ResourceTypeQuality {
    COMMON,
    UNCOMMON,
    RARE,
    STRATEGIC
};

typedef struct {
    Uint8 id;
    char name[10];
    enum ResourceTypeQuality quality;
} ResourceType;

typedef struct {
    Uint16 id;
    SDL_Texture *texture;
    SDL_FPoint position;
    SDL_FPoint speed;
} Asteroid;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_FPoint pos;
    SDL_FPoint speed;
} GameState;



void update(GameState *gameState);

#endif