// Custom library
#include <object.h>

// SDL2 library

// Standard library

Cell::Cell(int _left, int _top, int _width, int _height, 
    CellType _cellType, SDL_Texture* _defaultState) {
    cellType = _cellType;
    defaultState = _defaultState;
    cellRect = {_left, _top, _width, _height};
    frame = 0;
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
    // std::cout << SDL_GetError() << std::endl;
    // remember to break when using switch case.
    if (image != nullptr) {
        SDL_RenderCopy(renderer, image, nullptr, &cellRect);
        return;
    }

    switch (cellType) {
    case CAT_CELL: {
        // SDL_RenderCopy(renderer, gallery.getFrame(GRASS, frame), nullptr, &cellRect);
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
        // SDL_RenderCopy(renderer, defaultState, nullptr, &cellRect);
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
    int _gameBoardTop, int _width, int _height, SDL_Renderer* _renderer, Gallery &gallery) {
    boardRect = {_gameBoardTop, _gameBoardLeft, _boardWidth, _boardHeight};
    width = _width;
    height = _height;
    
    gameBoard.assign(_width, std::vector <Cell> (_height));
    for (int i = 0; i < _width; i++) {
        for (int j = 0; j < _height; j++) {
            int x = 0;
            gameBoard[i][j] = Cell(boardRect.x + i * (_boardWidth / width) + 1, boardRect.y + j * (_boardHeight / height) + 1, 
                (_boardWidth / width) - 1, (_boardHeight / height) - 1, EMPTY_CELL, gallery.getFrame(GRASS, x));
        }
    }
}

bool Board::isInsideBoard(int x, int y) {
    return 0 <= x && 0 <= y && x < width && y < height;   
}

void Board::renderBoard(SDL_Renderer* &renderer, SDL_Color color, Gallery &gallery) {
    /*
        This function is used to draw the board for user to play.
    */
    // std::cout << 1 << std::endl;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            gameBoard[i][j].drawCell(renderer, gallery, nullptr);
        }
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int i = 0; i <= width; i++) {
        for (int j = 0; j <= height; j++) {
            if (j + 1 <= width) {
                SDL_RenderDrawLine(renderer, boardRect.x + (i + 0) * (boardRect.w / width), boardRect.y + (j + 0) * (boardRect.h / height), 
                    boardRect.x + (i + 0) * (boardRect.w / width), boardRect.y + (j + 1) * (boardRect.h / height));
            }
            
            
            if (i + 1 <= height) {
                SDL_RenderDrawLine(renderer, boardRect.x + (i + 0) * (boardRect.w / width), boardRect.y + (j + 0) * (boardRect.h / height), 
                    boardRect.x + (i + 1) * (boardRect.w / width), boardRect.y + (j + 0) * (boardRect.h / height));
            }
        }
    }
    // std::cout << 2 << std::endl;
}

void Board::renderMouseSpecialCell(int mouseX, int mouseY, SDL_Renderer* &renderer, Gallery &gallery) {
    /*
        Render special animation when mouse touch some cell.
    */
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (gameBoard[i][j].isInsideCell(mouseX, mouseY) == true && gameBoard[i][j].getCellType() != BURNING_CELL && gameBoard[i][j].getCellType() != PROTECTED_CELL && 
                gameBoard[i][j].getCellType() != CAT_CELL) {
                int x = 0;
                gameBoard[i][j].drawCell(renderer, gallery, gallery.getFrame(PROTECTED, x));
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

    // std::cout << "Beginning next step" << std::endl;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
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

    // std::cout << "Board size: " << width << " " << height << " " << gameBoard[0].size() << " " << gameBoard.size();
    cellX = -1; cellY = -1;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) { 
            if (gameBoard[i][j].isInsideCell(mouseX, mouseY)) {
                cellX = i; cellY = j;
                return;
            }
        }
    }
}

int Board::countBurning() {
    int ans = 0;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
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

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
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
    int _gameBoardLeft, int _gameBoardTop, int _width, int _height, SDL_Renderer* _renderer, Gallery &gallery) {
    /*
        Game initialize
    */

    maximumBurningCell = _maximumBurningCell;
    // std::cout << "MaximumBurningCell: " << maximumBurningCell << std::endl;

    // Initializing the random generator
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    randomGenerator = RandomGenerator(seed);

    // Initializing the board of the game
    board = Board(_boardWidth, _boardHeight, _gameBoardLeft, 
        _gameBoardTop, _width, _height, _renderer, gallery);
    
    // Creating the cat
    catList.resize(numberOfCat);
    for (int i = 0; i < numberOfCat; i++) {
        catList[i].generatePosition(_width, _height, randomGenerator, board);
        board.setCellType(catList[i].getX(), catList[i].getY(), CAT_CELL);
    }
    

    // Creating the fire
    while (true) {
        int randomX = randomGenerator.randomInteger(0, _width - 1);
        int randomY = randomGenerator.randomInteger(0, _height - 1);
        if (randomX + randomY > (_width + _height) / 2) {
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
        // std::cout << "Cat has been burned!" << std::endl;
        updateGameState(LOSING_THE_GAME);
        return;
    }
    int postCount = board.countBurning();
    if (postCount > maximumBurningCell) {
        // std::cout << "Post count: " << postCount << std::endl;
        updateGameState(LOSING_THE_GAME);
        return;
    }

    if (board.stalemate()) {
        updateGameState(WINNING_THE_GAME);
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
        updateGameState(QUITTING_THE_GAME);
        return;
    }
    if (e.type != SDL_MOUSEBUTTONDOWN) {
        return;
    }

    int mouseX = e.button.x, mouseY = e.button.y;
    int cellX, cellY;
    board.getChosenCell(mouseX, mouseY, cellX, cellY);
    // std::cout << "Still handling" << std::endl;
    if (cellX == -1 && cellY == -1) {
        return;
    }

    // std::cout << cellX << " " << cellY << std::endl;

    if (board.getCellType(cellX, cellY) == EMPTY_CELL) {
        board.setCellType(cellX, cellY, PROTECTED_CELL);
        nextStep();
    } 

    // std::cout << "Done!" << std::endl;

    return;
}