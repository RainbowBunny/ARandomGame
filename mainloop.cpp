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
    easy.resize(9);
    medium.resize(9);
    hard.resize(9);

    for (auto &x : easy) {
        fin >> x;
    } 

    for (auto &x : medium) {
        fin >> x;
    } 

    for (auto &x : hard) {
        fin >> x;
    } 

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
        // std::cout << SDL_GetError() << std::endl;
        // std::cout << "Finished rendering" << std::endl;
        break;
    }

    case GAME_WINNING: {
        break;
    }

    case GAME_LOSING: {
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
                game = Game(easy[0], easy[1], easy[2], easy[3], easy[4], easy[5], easy[6], easy[7], easy[8], renderer, gallery);
                updateGameState(PLAYING_THE_GAME);
                background.setBackgroundState(GAME_BACKGROUND);

            } else if (clickedButton == "medium") {

                game = Game(medium[0], medium[1], medium[2], medium[3], medium[4], medium[5], medium[6], medium[7], medium[8], renderer, gallery);
                updateGameState(PLAYING_THE_GAME);
                background.setBackgroundState(GAME_BACKGROUND);

            } else if (clickedButton == "hard") {

                game = Game(hard[0], hard[1], hard[2], hard[3], hard[4], hard[5], hard[6], hard[7], hard[8], renderer, gallery);
                updateGameState(PLAYING_THE_GAME);
                background.setBackgroundState(GAME_BACKGROUND);

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

        }
        break;
    }

    }
}