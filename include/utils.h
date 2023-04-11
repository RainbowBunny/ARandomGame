// Custom library

// SDL2 library
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Standard library
#include <iostream>

void logSDLError(std::ostream &os, const std::string &msg, bool fatal);

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer, int screenWidth, int screenHeight, const char* windowTitle);

void quitSDL(SDL_Window* window, SDL_Renderer* renderer);