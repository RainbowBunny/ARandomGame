// Custom library

// SDL2 library
#include <SDL2/SDL.h>

// Standard library
#include <vector>

enum CellType {
    CAT_CELL = 0, // cell in which the cat is playing.
    BURNING_CELL, // cell which was destroyed by fire.
    PROTECTED_CELL, // cell which was chosen, fire can not get through.
    EMPTY_CELL, // cell in the playing screen but does not has anything
    CELL_OFF_BOARD // cell out of playing screen.
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
    SDL_Renderer* renderer;
public:
    Board(int _boardWidth, int _boardHeight, int _gameBoardLeft, 
            int _gameBoardTop, SDL_Renderer* _renderer);
    void createBoard();
    void renderBoard();
};