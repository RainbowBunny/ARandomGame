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

    background = Background();

    gameState = STARTING_SCREEN;
}

void MainLoop::renderGame(SDL_Renderer* &renderer, Gallery &gallery, int mouseX, int mouseY) {
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
        game.renderGame(renderer, BLACK_COLOR, gallery);
    }

    case GAME_WINNING: {

    }

    case GAME_LOSING: {

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
                logError(std::cout, "Maybe there are undefined behavior somewhere, clicked button in mainmenu: " + clickedButton, false);
            }
            break;
        }

        case CHOOSING_DIFFICULTY: {
            std::string clickedButton = difficultMenu.getPressedButton(mouseX, mouseY);
            if (clickedButton == "none") {
                logError(std::cout, "Clicking random thing in difficulty menu", false);
            } else if (clickedButton == "easy") {

                game = Game(28, 2, 1, 7, 7, 40, 40, renderer, gallery);
                updateGameState(PLAYING_THE_GAME);
                background.setBackgroundState(GAME_BACKGROUND);

            } else if (clickedButton == "medium") {

                game = Game(28, 2, 1, 7, 7, 40, 40, renderer, gallery);
                updateGameState(PLAYING_THE_GAME);
                background.setBackgroundState(GAME_BACKGROUND);

            } else if (clickedButton == "hard") {

                game = Game(28, 2, 1, 7, 7, 40, 40, renderer, gallery);
                updateGameState(PLAYING_THE_GAME);
                background.setBackgroundState(GAME_BACKGROUND);

            } else {
                logError(std::cout, "Maybe there are undefined behavior somewhere, clicked button in diffmenu: " + clickedButton, false);
            }
            break;
        }

        case PLAYING_THE_GAME: {
            game.handleUserInput(e);
            if (game.getGameState() == WINNING_THE_GAME) {
                updateGameState(WINNING_THE_GAME);
                background.setBackgroundState(GAME_WINNING);
            } else if (game.getGameState() == LOSING_THE_GAME) {
                updateGameState(LOSING_THE_GAME);
                background.setBackgroundState(GAME_LOSING);
            }
        }
        
        case GAME_WINNING: {

        }

        case GAME_LOSING: {

        }

        }
        break;
    }

    }
}