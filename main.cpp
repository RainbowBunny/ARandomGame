// Custom library
#include <constants.h>
#include <utils.h>
#include <object.h>
#include <gallery.h>

// SDL2 library
#include <SDL2/SDL.h>

// Standard library
#include <iostream>
#include <utility>

const char* gameTitle = "Game";

// Remember, do not forget argc, argv
int main(int argc, char **argv) {
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;

    initSDL(gWindow, gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT, gameTitle);

    // Creating game object
    Gallery gGallery(gRenderer);
    gGallery.loadGamePictures();

    Game game(28, 1, 1, 7, 7, 200, 20, gRenderer, gGallery);

    std::cout << "Initiated game" << std::endl;
    std::cout << SDL_GetError();
    
    // Event handler
    SDL_Event e;

    // Time frame to maintain a constant FPS
    Uint32 frameStart;
    int frameTime;

    // Main game loop.
    while (true) {
        // Get the number of ticks at the start of the loop
        frameStart = SDL_GetTicks();

        SDL_SetRenderDrawColor(gRenderer, WHITE_COLOR.r, WHITE_COLOR.g, WHITE_COLOR.b, WHITE_COLOR.a);
        SDL_RenderClear(gRenderer);

        // Getting user input
        while (SDL_PollEvent(&e) != 0) {
            game.handleUserInput(e);
        }

        if (game.getGameState() == QUIT) {
            break;
        } else if (game.getGameState() == WIN) {
            std::cout << "Winning" << std::endl;
            break;
        } else if (game.getGameState() == LOSE) {
            std::cout << "Losing" << std::endl;
            break;
        }

        game.renderGame(gRenderer, DEFAULT_COLOR, gGallery);
        
        SDL_RenderPresent(gRenderer);

        // Delay to maintain the current FPS

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    quitSDL(gWindow, gRenderer);
    return 0;
}