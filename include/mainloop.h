#pragma once
// Custom library
#include <constants.h>
#include <utils.h>
#include <object.h>
#include <gallery.h>
#include <menu.h>

// SDL2 library

// Standard library

class MainLoop {
private:
    Menu mainMenu, difficultMenu, pauseMenu, confirmMenu, restartMenu;
    Background background;
    Textbox difficulty, boardSize, currentBurningCell, burningCell;
    Game game;

    GameState gameState;
    std::vector <int> easy, medium, hard;
public:
    MainLoop(SDL_Renderer* renderer, Gallery &gallery);
    void updateGameState(GameState state) { gameState = state; }
    void renderGame(SDL_Renderer* &renderer, Gallery &gallery, int mouseX, int mouseY);
    void handleUserInput(SDL_Event e, SDL_Renderer* &renderer, Gallery gallery);
    GameState getGameState() { return gameState; }
};