// Custom library
#include <object.h>

// SDL2 library

// Standard library

Cell::Cell(int _left, int _top, int _width, int _height, 
    CellType _cellType, SDL_Texture* _defaultState) {
    cellType = _cellType;
    defaultState = _defaultState;
    cellRect = {_left, _top, _width, _height};
}

bool Cell::isInsideCell(int posx, int posy) {
    return posx >= cellRect.x and posx <= cellRect.x + cellRect.w and posy >= cellRect.y and posy <= cellRect.y + cellRect.h;  
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
        SDL_RenderCopy(renderer, image, nullptr, &cellRect);
        return;
    }

    switch (cellType) {
    case CAT_CELL: {
        SDL_RenderCopy(renderer, gallery.getFrame(GRASS, frame), nullptr, &cellRect);
        SDL_RenderCopy(renderer, gallery.getFrame(CAT, frame), nullptr, &cellRect);
        frame++;
        break;
    }

    case BURNING_CELL: {
        SDL_RenderCopy(renderer, gallery.getFrame(BURNING, frame), nullptr, &cellRect);
        frame++;
        break;
    }

    case PROTECTED_CELL: {
        SDL_RenderCopy(renderer, gallery.getFrame(PROTECTED, frame), nullptr, &cellRect);
        frame++;
        break;
    }

    case EMPTY_CELL: {
        SDL_RenderCopy(renderer, defaultState, nullptr, &cellRect);
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
            if (gameBoard[i][j].isInsideCell(mouseX, mouseY) == true && gameBoard[i][j].getCellType() != BURNING_CELL && gameBoard[i][j].getCellType() != PROTECTED_CELL) {
                gameBoard[i][j].drawCell(renderer, gallery, gallery.getFrame(PROTECTED, 0));
            }
        }
    }
}

bool Board::nextStep() {
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
                    return false;
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

    return true;
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

bool Board::stalemate() {
    /*
        Check if the fire has been contained
    */
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
                    return false;
                }

                if (gameBoard[nx][ny].getCellType() == EMPTY_CELL) {
                    return false;
                }
            }
        }
    }
    return true;
}

void Cat::generatePosition(int width, int height, RandomGenerator &randomGenerator, Board board) {
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
        if (genX + genY < (width + height) / 2 - 1 && board.getCellType(genX, genY) != CAT_CELL) {
            x = genX;
            y = genY;
            break;
        }
    }
}

Game::Game(int _maximumBurningCell, int numberOfCat, int initialBurningCell, int _boardWidth, int _boardHeight, 
    int _gameBoardLeft, int _gameBoardTop, SDL_Renderer* _renderer, Gallery &gallery) {
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
    catList.resize(numberOfCat);
    for (int i = 0; i < numberOfCat; i++) {
        catList[i].generatePosition(_boardWidth, _boardHeight, randomGenerator, board);
        board.setCellType(catList[i].getX(), catList[i].getY(), CAT_CELL);
    }
    

    // Creating the fire
    while (true) {
        int randomX = randomGenerator.randomInteger(0, _boardWidth - 1);
        int randomY = randomGenerator.randomInteger(_boardHeight - 1, _boardHeight - 1);
        if (randomX + randomY > (_boardWidth + _boardHeight) / 2) {
            std::cout << randomX << ' ' << randomY << std::endl;
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
    /*
        Rendering the next stage of the game
    */
    if (!board.nextStep()) {
        updateGameState(LOSE);
        return;
    }
    int postCount = board.countBurning();
    if (postCount > MAXIMUM_BURNING_CELL) {
        updateGameState(LOSE);
        return;
    }

    if (board.stalemate()) {
        updateGameState(WIN);
        return;
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

    if (board.getCellType(cellX, cellY) == PROTECTED_CELL) {
        return;
    } 

    board.setCellType(cellX, cellY, PROTECTED_CELL);
    nextStep();
}