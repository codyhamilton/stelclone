#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_init.h>
#include "game.h"
#include "render.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SDL_Log("SDL_AppInit\n");
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    GameState *gameState = (GameState*)SDL_calloc(1, sizeof(GameState));

    if(!SDL_CreateWindowAndRenderer("Stelclone", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &gameState->window, &gameState->renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    //SDL_SetRenderLogicalPresentation(gameState->renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderVSync(gameState->renderer, 1);
    *appstate = gameState;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    GameState *gameState = (GameState*)appstate;
    switch(event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        default:
            break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    static const Uint64 target_frame_time = 12;
    static Uint64 last_time = 0;
    static Uint64 last_frame = 0;

    const Uint64 current_time = SDL_GetTicks();
    Uint64 delta_time = current_time - last_time;
    last_time = current_time;
    GameState *gameState = (GameState*)appstate;
    SDL_Delay(12);

    render(gameState);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    SDL_Log("SDL_AppQuit: %d\n", result);
    GameState *gameState = (GameState*)appstate;
    SDL_DestroyWindow(gameState->window);
    SDL_DestroyRenderer(gameState->renderer);
    SDL_free(gameState);
    SDL_Quit();
}