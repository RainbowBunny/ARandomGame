// Custom library
#include <utils.h>

// SDL2 library

// Standard library

void logSDLError(std::ostream &os, const std::string &msg, bool fatal) {
    /*
        This function is used to show the error to an ostream os, which can be std::cout,
        an outfile stream, etc...
    */
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer, 
    int screenWidth, int screenHeight, const char* windowTitle) {
    /*
        This function is used to create window and renderer for the game.
        Pass the window and renderer variable you want to create to this function.
        Remember these variable are passed by reference (&).
    */

    // Init the SDL library.
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError(std::cout, "SDL_Init", true);
    }

    // Creating the window of the game.
    window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    // Check if we could not create the window.
    if (window == nullptr) {
        logSDLError(std::cout, "Create Window", true);
    }

    // Create the renderer of the game, TODO: Check the meaning of -1 and the flag.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED 
                                            | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        logSDLError(std::cout, "Create Renderer", true);
    }

    // I am not sure what these line do, but remember, width need to come before height.
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, screenWidth, screenHeight);

    // Initialize the Image library
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG)) {
        logSDLError(std::cout, "IMG Init", true);
    }

    // Initialize the ttf library
    if (TTF_Init() == -1) {
        logSDLError(std::cout, "TTF Init", true);
    }
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
    /*
        This function must be used when we quit SDL.
        Pass your window and renderer to be destroyed and to free up memory.
    */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

void logError(std::ostream &os, const std::string &msg, bool fatal) {
    os << "Some thing gone wrong maybe? " << msg << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}