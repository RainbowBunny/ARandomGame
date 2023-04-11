// Custom library
#include <constants.h>
#include <utils.h>

// SDL2 library
#include <SDL2/SDL.h>

// Standard library
#include <iostream>

// Remember, do not forget argc, argv
int main(int argc, char **argv) {
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;

    initSDL(gWindow, gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT, gameTitle);
    
    // Event handler
    SDL_Event e;

    // Time frame to maintain a constant FPS
    Uint32 frameStart;
    int frameTime;

    // Main game loop.
    bool playing = true;
    while (playing) {
        // Get the number of ticks at the start of the loop
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                playing = false;
            }
        }

        if (!playing) {
            break;
        }

        // Delay to maintain the current FPS
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }        
    }

    quitSDL(gWindow, gRenderer);
    return 0;
}