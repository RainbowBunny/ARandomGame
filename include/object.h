#pragma once
#ifndef OBJECT_H
#define OBJECT_H
// Custom library
#include <utils.h>
#include <gallery.h>
#include <constants.h>

// SDL2 library
#include <SDL2/SDL.h>

// Standard library
#include <vector>
#include <chrono>
#include <random>

// Random device
class RandomGenerator {
private:
    std::mt19937_64 rng;
public:
    RandomGenerator() {}
    RandomGenerator(unsigned seed) { rng.seed(seed); }
    int randomInteger(int l, int r) { return rng() % (r - l + 1) + l; }
};

enum CellType {
    CAT_CELL = 0, // cell in which the cat is playing.
    BURNING_CELL, // cell which was destroyed by fire.
    PROTECTED_CELL, // cell which was chosen, fire can not get through.
    EMPTY_CELL, // cell in the playing screen but does not has anything
    CELL_OFF_BOARD // cell out of playing screen.
};


class Cell {
private:
    int frame;
    SDL_Texture* defaultState;
    CellType cellType;
    SDL_Rect cellRect;
public:
    Cell() {}
    Cell(int _left, int _top, int _width, int _height, 
        CellType _cellType, SDL_Texture* _defaultState);
    CellType getCellType() { return cellType; }
    bool isInsideCell(int posx, int posy);
    void setCellType(CellType _type);
    void updateDefaultState(SDL_Texture* _default);
    void drawCell(SDL_Renderer* &Renderer, Gallery &gallery, SDL_Texture* image);
};

class Board {
private:
    int width, height;
    SDL_Rect boardRect;
    std::vector <std::vector <Cell> > gameBoard;
public:
    Board() {}
    Board(int _boardWidth, int _boardHeight, int _gameBoardLeft, 
        int _gameBoardTop, int _width, int _height, SDL_Renderer* _renderer, Gallery &gallery);
    bool isInsideBoard(int x, int y);
    CellType getCellType(int x, int y) { return gameBoard[x][y].getCellType(); }
    void setCellType(int x, int y, CellType type) { gameBoard[x][y].setCellType(type); }
    void renderBoard(SDL_Renderer* &renderer, SDL_Color color, Gallery &gallery);
    void renderMouseSpecialCell(int mouseX, int mouseY, SDL_Renderer* &renderer, Gallery &gallery);
    bool nextStep();
    void getChosenCell(int mouseX, int mouseY, int &cellX, int &cellY);
    bool stalemate();
    int countBurning();
};

class Cat {
private:
    int x, y;
public: 
    Cat() {}
    void generatePosition(int width, int height, RandomGenerator &randomGenerator, Board board);
    int getX() { return x; };
    int getY() { return y; };
};

class Game {
private:
    int maximumBurningCell;
    RandomGenerator randomGenerator;
    Board board;
    std::vector <Cat> catList;
    GameState gameState = PLAYING_THE_GAME;
public:
    Game() {}
    Game(int _maximumBurningCell, int numberOfCat, int initialBurningCell, int _boardWidth, int _boardHeight, 
        int _gameBoardLeft, int _gameBoardTop, int _width, int _height, SDL_Renderer* _renderer, Gallery &gallery);
    void renderGame(SDL_Renderer* &renderer, SDL_Color color, Gallery &gallery);
    void nextStep();
    void handleUserInput(SDL_Event e);
    void updateGameState(GameState g) { gameState = g; }
    GameState getGameState() { return gameState; }
};

#endif