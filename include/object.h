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

enum CellType {
    CAT_CELL = 0, // cell in which the cat is playing.
    BURNING_CELL, // cell which was destroyed by fire.
    PROTECTED_CELL, // cell which was chosen, fire can not get through.
    EMPTY_CELL, // cell in the playing screen but does not has anything
    CELL_OFF_BOARD // cell out of playing screen.
};

// Random device

class RandomGenerator {
private:
    std::mt19937_64 rng;
public:
    RandomGenerator() {}
    RandomGenerator(unsigned seed) { rng.seed(seed); } 
    int randomInteger(int l, int r) { return rng() % (r - l + 1) + l; }
};

class Cell {
private:
    int left, top, width, height, frame;
    SDL_Texture* defaultState;
    CellType cellType;
    SDL_Rect dstRect;
public:
    Cell() {}
    Cell(int _left, int _top, int _width, int _height, 
        CellType _cellType, SDL_Texture* _defaultState);
    bool isInsideCell(int posx, int posy);
    void setCellType(CellType _type);
    void updateDefaultState(SDL_Texture* _default);
    void drawCell(SDL_Renderer* &Renderer, Gallery &gallery);
};

class Board {
private:
    int boardWidth, boardHeight, gameBoardLeft, gameBoardTop;
    std::vector <std::vector <Cell> > gameBoard;
public:
    Board() {}
    Board(int _boardWidth, int _boardHeight, int _gameBoardLeft, 
        int _gameBoardTop, SDL_Renderer* _renderer, Gallery &gallery);
    void setCellType(int x, int y, CellType type) { gameBoard[x][y].setCellType(type); };
    void createBoard(Gallery &gallery);
    void renderBoard(SDL_Renderer* &renderer, SDL_Color color, Gallery &gallery);
};

class Cat {
private:
    int x, y;
public: 
    Cat() {}
    void generatePosition(int width, int height, RandomGenerator &randomGenerator);
    int getX() { return x; };
    int getY() { return y; };
};

class Game {
private:
    Board board;
    Cat cat;
    RandomGenerator randomGenerator;
public:
    Game() {}
    Game(int _boardWidth, int _boardHeight, int _gameBoardLeft, int _gameBoardTop, SDL_Renderer* _renderer, Gallery &gallery);
    void renderGame(SDL_Renderer* &renderer, SDL_Color color, Gallery &gallery);
};

#endif