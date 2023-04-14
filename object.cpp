// Custom library
#include <object.h>

// SDL2 library

// Standard library

Cell::Cell(int _left, int _top, int _width, int _height, 
    CellType _cellType, SDL_Texture* _defaultState) {
    left = _left; top = _top;
    width = _width; height = _height;
    cellType = _cellType;
    defaultState = _defaultState;
    dstRect = {left, top, width, height};
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

void Cell::drawCell(SDL_Renderer* &renderer, Gallery &gallery, SDL_Texture* image) {
    // remember to break when using switch case.

    if (image != nullptr) {
        SDL_RenderCopy(renderer, image, nullptr, &dstRect);
        return;
    }

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

bool Board::isInsideBoard(int x, int y) {
    return 0 <= x && 0 <= y && x < boardWidth && y < boardHeight;   
}

void Board::createBoard(Gallery &gallery) {
    /*
        Initialize the cell of the board.
    */
    gameBoard.assign(boardWidth, std::vector <Cell> (boardHeight));
    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            gameBoard[i][j] = Cell(gameBoardLeft + i * CELL_SIZE + 1, gameBoardTop + j * CELL_SIZE + 1, 
                CELL_SIZE - 1, CELL_SIZE - 1, EMPTY_CELL, gallery.getFrame(GRASS, 0));
        }
    }
}

void Board::renderBoard(SDL_Renderer* &renderer, SDL_Color color, Gallery &gallery) {
    /*
        This function is used to draw the board for user to play.
    */

    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            gameBoard[i][j].drawCell(renderer, gallery, nullptr);
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

void Board::renderMouseSpecialCell(int mouseX, int mouseY, SDL_Renderer* &renderer, Gallery &gallery) {
    /*
        Render special animation when mouse touch some cell.
    */
    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            if (gameBoard[i][j].isInsideCell(mouseX, mouseY) == true && gameBoard[i][j].getCellType() != BURNING_CELL) {
                gameBoard[i][j].drawCell(renderer, gallery, gallery.getFrame(PROTECTED, 0));
            }
        }
    }
}

void Board::nextStep() {
    /*
        The next step of the game.
        Fire will burn adjency cell which has not been protected.
    */
    std::vector <std::pair <int, int> > updateBurningCell;
    
    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            if (gameBoard[i][j].getCellType() != BURNING_CELL) {
                continue;
            }
            for (int k = 0; k < 4; k++) {
                int nx = i + dx[k], ny = j + dy[k];
                if (!isInsideBoard(nx, ny)) {
                    continue;
                }
                
                if (gameBoard[nx][ny].getCellType() == CAT_CELL) {
                    std::cout << "Game over" << std::endl;
                    return;
                }

                if (gameBoard[nx][ny].getCellType() == EMPTY_CELL) {
                    updateBurningCell.emplace_back(nx, ny);
                }
            }
        }
    }

    for (auto cell : updateBurningCell) {
        gameBoard[cell.first][cell.second].setCellType(BURNING_CELL);
    }
}

void Board::getChosenCell(int mouseX, int mouseY, int &cellX, int &cellY) {
    /*
        Input: Position of mouse: mouseX, mouseY
        Output: Modified the value of cellX and cellY to the chosen cell of the board
        Return -1 -1 if no cell was chosen.
    */
    cellX = -1; cellY = -1;
    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) { 
            if (gameBoard[i][j].isInsideCell(mouseX, mouseY)) {
                cellX = i; cellY = j;
                return;
            }
        }
    }
}

int Board::countBurning() {
    int ans = 0;
    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            ans += (gameBoard[i][j].getCellType() == BURNING_CELL);
        }
    }
    return ans;
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
            std::cout << x << ' ' << y << std::endl;
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
    /*
        Function to render all event in game.
    */
    board.renderBoard(renderer, color, gallery);
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    board.renderMouseSpecialCell(mouseX, mouseY, renderer, gallery);
}

void Game::nextStep() {
    int preCount = board.countBurning();
    board.nextStep();
    int postCount = board.countBurning();
    if (preCount == postCount) {
        updateGameState(WIN);
        return;
    }
    if (postCount > MAXIMUM_BURNING_CELL) {
        updateGameState(LOSE);
    }
}

void Game::handleUserInput(SDL_Event e) {
    /*
        Function to handle all user input:
        - Quit game.
        - Choose a cell.
    */
    if (e.type == SDL_QUIT) {
        updateGameState(QUIT);
        return;
    }
    if (e.type != SDL_MOUSEBUTTONDOWN) {
        return;
    }

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    int cellX, cellY;
    board.getChosenCell(mouseX, mouseY, cellX, cellY);
    if (cellX == -1 && cellY == -1) {
        return;
    }

    board.setCellType(cellX, cellY, PROTECTED_CELL);
    nextStep();
}