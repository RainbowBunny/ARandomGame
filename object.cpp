// Custom library
#include <object.h>

// SDL2 library

// Standard library

Cell::Cell(int _top, int _left, int _width, int _height, 
    CellType _cellType, SDL_Texture* _defaultState) {
    top = _top; left = _left;
    width = _width; height = _height;
    cellType = _cellType;
    defaultState = _defaultState;
    dstRect = {top, left, width, height};
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
    int _gameBoardTop, SDL_Renderer* _renderer, Gallery &gallery) {
    boardWidth = _boardWidth;
    boardHeight = _boardHeight;
    gameBoardTop = _gameBoardTop;
    gameBoardLeft = _gameBoardLeft;

    createBoard(gallery);
}

void Board::createBoard(Gallery &gallery) {
    /*
        Initialize the cell of the board.
    */
    gameBoard.assign(boardWidth, std::vector <Cell> (boardHeight));
    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            gameBoard[i][j] = Cell(gameBoardLeft + i * CELL_SIZE + 5, gameBoardTop + j * CELL_SIZE - 5, 
                CELL_SIZE - 5, CELL_SIZE - 5, EMPTY_CELL, gallery.getFrame(GRASS, 0));
        }
    }
}

void Board::renderBoard(SDL_Renderer* &renderer, SDL_Color color, Gallery &gallery) {
    /*
        This function is used to draw the board for user to play.
    */

    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            gameBoard[i][j].drawCell(renderer, gallery);
        }
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            SDL_RenderDrawLine(renderer, gameBoardLeft + (i + 0) * CELL_SIZE, gameBoardTop + (j + 0) * CELL_SIZE, 
                gameBoardLeft + (i + 0) * CELL_SIZE, gameBoardTop + (j + 1) * CELL_SIZE);
            SDL_RenderDrawLine(renderer, gameBoardLeft + (i + 0) * CELL_SIZE, gameBoardTop + (j + 0) * CELL_SIZE, 
                gameBoardLeft + (i + 1) * CELL_SIZE, gameBoardTop + (j + 0) * CELL_SIZE);
        }
    }
    
    SDL_RenderDrawLine(renderer, gameBoardLeft, gameBoardTop, 
        gameBoardLeft, gameBoardTop + CELL_SIZE);
    SDL_RenderDrawLine(renderer, gameBoardLeft, gameBoardTop, 
        gameBoardLeft + CELL_SIZE, gameBoardTop);

}

void Cat::generatePosition(int width, int height, RandomGenerator &randomGenerator) {
    /*
        TODO: Implement
        This function is used to generate a random position for cat.
        Condition: The cat must be over the side diagonal of the board.
        Example: X is where the cat can be if width = 3 and height = 3
        XX.
        X..
        ...
    */

    while (true) {
        int genX = randomGenerator.randomInteger(0, width - 1);
        int genY = randomGenerator.randomInteger(0, height - 1);
        if (genX + genY < (width + height) / 2) {
            x = genX;
            y = genY;
            break;
        }
    }
}

Game::Game(int _boardWidth, int _boardHeight, int _gameBoardLeft, 
    int _gameBoardTop, SDL_Renderer* _renderer, Gallery &gallery) {
    /*
        Game initialize
    */

    // Initializing the random generator
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    randomGenerator = RandomGenerator(seed);

    // Initializing the board of the game
    board = Board(_boardWidth, _boardHeight, _gameBoardLeft, 
        _gameBoardTop, _renderer, gallery);
    board.createBoard(gallery);
    
    // Creating the cat
    cat = Cat();
    cat.generatePosition(_boardWidth, _boardHeight, randomGenerator);
    board.setCellType(cat.getX(), cat.getY(), CAT_CELL);

    // Creating the fire
    while (true) {
        int randomX = randomGenerator.randomInteger(0, _boardWidth - 1);
        int randomY = randomGenerator.randomInteger(0, _boardHeight - 1);
        if (randomX + randomY > (_boardWidth + _boardHeight) / 2) {
            board.setCellType(randomX, randomY, BURNING_CELL);
            break;
        }
    }

}

void Game::renderGame(SDL_Renderer* &renderer, SDL_Color color, Gallery &gallery) {
    board.renderBoard(renderer, color, gallery);
}