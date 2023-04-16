#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H
// Custom library

// SDL2 library
#include <SDL2/SDL.h>

// Standard library

const int FPS = 20;
const int FRAME_DELAY = 1000 / FPS;
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 650;
const int CELL_SIZE = 80;
const int MAXIMUM_BURNING_CELL = 28;

const SDL_Color CYAN_COLOR = {0, 255, 255};
const SDL_Color BLUE_COLOR = {0, 0, 255};
const SDL_Color ORANGE_COLOR = {255, 165, 0};
const SDL_Color YELLOW_COLOR = {255, 255, 0};
const SDL_Color LIME_COLOR = {0, 255, 0};
const SDL_Color PURPLE_COLOR = {128, 0, 128};
const SDL_Color RED_COLOR = {255, 0, 0};
const SDL_Color WHITE_COLOR = {255, 255, 255};
const SDL_Color BLACK_COLOR = {0, 0, 0};
const SDL_Color GREEN_COLOR = {0, 128, 0};

const SDL_Color DEFAULT_COLOR = BLACK_COLOR;

enum GameState {
    PLAYING = 0,
    LOSE,
    WIN, 
    QUIT
};

# endif