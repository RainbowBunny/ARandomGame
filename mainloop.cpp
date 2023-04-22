#include <mainloop.h>

/* 
enum GameState {
    STARTING_SCREEN = 0,
    CHOOSING_DIFFICULTY,
    PLAYING_THE_GAME,
    PAUSING_THE_GAME,
    LOSING_THE_GAME,
    WINNING_THE_GAME,
    RESTARTING_SCREEN,
    QUITTING_THE_GAME
};

*/

MainLoop::MainLoop(SDL_Renderer* renderer, Gallery &gallery) {
    mainMenu = loadMenuFromFile("data/start_menu.txt", renderer, gallery);
    pauseMenu = loadMenuFromFile("data/pause_menu.txt", renderer, gallery);
    difficultMenu = loadMenuFromFile("data/difficulty_menu.txt", renderer, gallery);
    confirmMenu = loadMenuFromFile("data/confirm_menu.txt", renderer, gallery);
    restartMenu = loadMenuFromFile("data/restart_menu.txt", renderer, gallery);

    std::ifstream fin("data/level_data.txt");
    for (int i = 0; i < TOTAL_LEVEL; i++) {
        levelData[i].resize(9);
        for (auto &x : levelData[i]) {
            fin >> x;
        }
    }

    std::string file[] = {"data/easy.txt", "data/medium.txt", "data/hard.txt"};
    for (int i = 0; i < TOTAL_LEVEL; i++) {
        std::ifstream inStream(file[i]);
        int cat, fire;
        inStream >> cat;
        catPosition[i].resize(cat);
        for (auto &x : catPosition[i]) {
            inStream >> x.first >> x.second;
        }

        inStream >> fire;
        firePosition[i].resize(fire);
        for (auto &x : firePosition[i]) {
            inStream >> x.first >> x.second;
        }
    }

    std::ifstream textboxLoader("data/textbox_data.txt");
    if (!textboxLoader.is_open()) {
        logError(std::cout, "Textbox file loading fail", true);
    }
    difficulty = createTextboxFromFile(textboxLoader, BUTTON);
    boardSize = createTextboxFromFile(textboxLoader, BUTTON);
    currentBurningCell = createTextboxFromFile(textboxLoader, BUTTON);
    burningCell = createTextboxFromFile(textboxLoader, NONE);

    background = Background();

    gameState = STARTING_SCREEN;
}

void MainLoop::renderGame(SDL_Renderer* &renderer, Gallery &gallery, int mouseX, int mouseY) {
    // std::cout << "Mainloop rendering..." << std::endl;
    background.renderBackground(renderer, gallery);
    switch (gameState) {
    case STARTING_SCREEN: {
        mainMenu.renderMenu(renderer, gallery, mouseX, mouseY);
        break;
    }
    case CHOOSING_DIFFICULTY: {
        difficultMenu.renderMenu(renderer, gallery, mouseX, mouseY);
        break;
    }

    case PLAYING_THE_GAME: {
        // std::cout << "Rendering the game: ";
        game.renderGame(renderer, BLACK_COLOR, gallery);
        difficulty.renderTextBox(renderer, gallery);
        boardSize.renderTextBox(renderer, gallery);
        currentBurningCell.renderTextBox(renderer, gallery);
        burningCell.renderTextBox(renderer, gallery);
        // std::cout << SDL_GetError() << std::endl;
        // std::cout << "Finished rendering" << std::endl;
        break;
    }

    case PAUSING_THE_GAME: {
        game.renderGame(renderer, BLACK_COLOR, gallery);
        difficulty.renderTextBox(renderer, gallery);
        boardSize.renderTextBox(renderer, gallery);
        currentBurningCell.renderTextBox(renderer, gallery);
        burningCell.renderTextBox(renderer, gallery);
        pauseMenu.renderMenu(renderer, gallery, mouseX, mouseY);
        break;
    }

    case WINNING_THE_GAME: {
        game.renderGame(renderer, BLACK_COLOR, gallery);
        difficulty.renderTextBox(renderer, gallery);
        boardSize.renderTextBox(renderer, gallery);
        currentBurningCell.renderTextBox(renderer, gallery);
        burningCell.renderTextBox(renderer, gallery);
        background.renderBackground(renderer, gallery);
        break;
    }

    case LOSING_THE_GAME: {
        game.renderGame(renderer, BLACK_COLOR, gallery);
        difficulty.renderTextBox(renderer, gallery);
        boardSize.renderTextBox(renderer, gallery);
        currentBurningCell.renderTextBox(renderer, gallery);
        burningCell.renderTextBox(renderer, gallery);
        background.renderBackground(renderer, gallery);
        break;
    }

    case RESTARTING_SCREEN: {
        restartMenu.renderMenu(renderer, gallery, mouseX, mouseY);
        break;
    }

    }
}

void MainLoop::handleUserInput(SDL_Event e, SDL_Renderer* &renderer, Gallery gallery) {
    switch (e.type) {
    case SDL_QUIT : {
        updateGameState(QUITTING_THE_GAME);
        break;
    }
    case SDL_MOUSEBUTTONDOWN: {
        int mouseX = e.button.x, mouseY = e.button.y;
        switch (gameState) {
        case STARTING_SCREEN: {
            std::string clickedButton = mainMenu.getPressedButton(mouseX, mouseY);
            if (clickedButton == "none") {
                logError(std::cout, "Clicking random thing in main menu", false);
            } else if (clickedButton == "play") {
                updateGameState(CHOOSING_DIFFICULTY);
                background.setBackgroundState(MAIN_MENU);
            } else if (clickedButton == "option") {
                logError(std::cout, "Not implemented error", false);
            } else if (clickedButton == "quit") {
                updateGameState(QUITTING_THE_GAME);
            } else {
                logError(std::cout, "Maybe there are undefined behavior somewhere, clicked button in main menu: " + clickedButton, false);
            }
            break;
        }

        case CHOOSING_DIFFICULTY: {
            std::string clickedButton = difficultMenu.getPressedButton(mouseX, mouseY);
            if (clickedButton == "none") {
                logError(std::cout, "Clicking random thing in difficulty menu", false);
            } else if (clickedButton == "easy") {
                game = Game(levelData[EASY], renderer, gallery, catPosition[EASY], firePosition[EASY]);

                updateGameState(PLAYING_THE_GAME);
                background.setBackgroundState(GAME_BACKGROUND);
                difficulty.updateText("Easy");
                boardSize.updateText(std::to_string(game.getBoardHeight()) + "x" + std::to_string(game.getBoardWidth()));

                currentBurningCell.updateText(std::to_string(game.getCurrentBurningCell()) + "/" + std::to_string(game.getMaximumBurningCell()));
            } else if (clickedButton == "medium") {
                game = Game(levelData[MEDIUM], renderer, gallery, catPosition[MEDIUM], firePosition[MEDIUM]);

                updateGameState(PLAYING_THE_GAME);
                background.setBackgroundState(GAME_BACKGROUND);
                difficulty.updateText("Medium");
                boardSize.updateText(std::to_string(game.getBoardHeight()) + "x" + std::to_string(game.getBoardWidth()));

                currentBurningCell.updateText(std::to_string(game.getCurrentBurningCell()) + "/" + std::to_string(game.getMaximumBurningCell()));
            } else if (clickedButton == "hard") {
                game = Game(levelData[HARD], renderer, gallery, catPosition[HARD], firePosition[HARD]);

                updateGameState(PLAYING_THE_GAME);
                background.setBackgroundState(GAME_BACKGROUND);
                difficulty.updateText("Hard");
                boardSize.updateText(std::to_string(game.getBoardHeight()) + "x" + std::to_string(game.getBoardWidth()));
                
                currentBurningCell.updateText(std::to_string(game.getCurrentBurningCell()) + "/" + std::to_string(game.getMaximumBurningCell()));
            } else {
                logError(std::cout, "Maybe there are undefined behavior somewhere, clicked button in diff menu: " + clickedButton, false);
            }
            break;
        }

        case PLAYING_THE_GAME: {
            // std::cout << "Handling the input" << std::endl;
            game.handleUserInput(e);
            if (game.getGameState() == WINNING_THE_GAME) {
                // std::cout << 1 << std::endl;
                updateGameState(WINNING_THE_GAME);
                background.setBackgroundState(GAME_WINNING);
            } else if (game.getGameState() == LOSING_THE_GAME) {
                // std::cout << 2 << std::endl;
                updateGameState(LOSING_THE_GAME);
                background.setBackgroundState(GAME_LOSING);
            }
            currentBurningCell.updateText(std::to_string(game.getCurrentBurningCell()) + "/" + std::to_string(game.getMaximumBurningCell()));
            // std::cout << "Finished" << std::endl;
            break;
        }
        
        case WINNING_THE_GAME: {
            // std::cout << 1 << std::endl;
            updateGameState(RESTARTING_SCREEN);
            background.setBackgroundState(MAIN_MENU);
            break;
        }

        case LOSING_THE_GAME: {
            // std::cout << 2 << std::endl;
            updateGameState(RESTARTING_SCREEN);
            background.setBackgroundState(MAIN_MENU);
            break;
        }

        case RESTARTING_SCREEN: {
            std::string clickedButton = restartMenu.getPressedButton(mouseX, mouseY);
            if (clickedButton == "restart") {
                updateGameState(CHOOSING_DIFFICULTY);
            } else if (clickedButton == "main menu") {
                updateGameState(STARTING_SCREEN);
            } else {
                logError(std::cout, "Maybe there are undefined behavior somewhere, clicked button in restart menu: " + clickedButton, false);
            }
            break;
        }

        case PAUSING_THE_GAME: {
            std::string clickedButton = pauseMenu.getPressedButton(mouseX, mouseY);
            if (clickedButton == "resume") {
                updateGameState(PLAYING_THE_GAME);
            } else if (clickedButton == "restart") {
                updateGameState(CHOOSING_DIFFICULTY);
            } else if (clickedButton == "quit") {
                updateGameState(STARTING_SCREEN);
            } else {
                logError(std::cout, "Maybe there are undefined behavior somewhere, clicked button in restart menu: " + clickedButton, false);
            }
            break;
        }

        default: {
            break;
        }
            

        }
        break;
    }

    case SDL_KEYDOWN: {
        switch (e.key.keysym.sym) {
        case SDLK_ESCAPE: {
            if (gameState == PLAYING_THE_GAME) {
                updateGameState(PAUSING_THE_GAME);
            } else if (gameState == PAUSING_THE_GAME) {
                updateGameState(PLAYING_THE_GAME);
            }
            break;
        }

        default: {
            break;
        }

        }
    }

    default: {
        break;
    }

    }
}