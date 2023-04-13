// Custom library
#include <object.h>
#include <utils.h>
#include <gallery.h>
#include <constants.h>

// SDL2 library

// Standard library

Cell::Cell(int _top, int _left, int _width, int _height, 
                CellType _cellType, SDL_Texture* _defaultState) {
    top = _top; left = _left;
    width = _width; height = _height;
    cellType = _cellType;
    defaultState = _defaultState;
}

bool Cell::isInsideCell(int posx, int posy) {
    return posx >= left and posx <= left + width and posy >= top and posy <= top + height;  
}

void Cell::setCellType(CellType _type) {
    cellType = _type;
}

void Cell::updateDefaultState(SDL_Texture* _default) {
    defaultState = _default;
}

void Cell::drawCell(SDL_Renderer* &renderer, Gallery &gallery) {
    // remember to break when using switch case.
    switch (cellType) {
    case CAT_CELL: {
        SDL_RenderCopy(renderer, gallery.getFrame(CAT, frame), nullptr, &dstRect);
        frame++;
        break;
    }
        
    
    case BURNING_CELL: {
        SDL_RenderCopy(renderer, gallery.getFrame(BURNING, frame), nullptr, &dstRect);
        frame++;
        break;
    }
        
    
    case PROTECTED_CELL: {
        SDL_RenderCopy(renderer, gallery.getFrame(PROTECTED, frame), nullptr, &dstRect);
        frame++;
        break;
    }
        

    case EMPTY_CELL: {
        SDL_RenderCopy(renderer, defaultState, nullptr, &dstRect);
        break;
    }
        
    
    case CELL_OFF_BOARD: {
        logError(std::cout, "Why are you drawing cell off board????", false);
        break;
    }
        

    default: {
        break;
    }
        
    }
}

Board::Board(int _boardWidth, int _boardHeight, int _gameBoardLeft, 
                int _gameBoardTop, SDL_Renderer* _renderer) {
    boardWidth = _boardWidth;
    boardHeight = _boardHeight;
    gameBoardTop = _gameBoardTop;
    gameBoardLeft = _gameBoardLeft;
    renderer = _renderer;

    createBoard();
}

void Board::createBoard() {
    gameBoard.assign(boardWidth, std::vector <Cell> (boardHeight));
    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            gameBoard[i][j] = Cell(gameBoardTop + i * CELL_SIZE, gameBoardLeft + j * CELL_SIZE, CELL_SIZE, CELL_SIZE, EMPTY_CELL, )
        }
    }
}

void Board::renderBoard() {

}