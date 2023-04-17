#pragma once
// Custom library
#include <utils.h>
#include <gallery.h>

// SDL2 library
#include <SDL2/SDL.h>

// Standard library
#include <fstream>
#include <vector>
#include <string>

enum State {
    MAIN_MENU = 0,
    GAME_BACKGROUND,
    PAUSE_MENU,
    GAME_WINNING,
    GAME_LOSING,
    TOTALSTATE
};

class Button {
private:
    std::string buttonName;
    SDL_Rect buttonRect;
    SDL_Rect textRect;
    SDL_Texture* buttonNormal;
    SDL_Texture* buttonChoosing;
public:
    Button() {};
    Button(std::string buttonName, SDL_Rect position, SDL_Texture* normal, SDL_Texture* choosing);
    std::string getButtonName() { return buttonName; }
    bool isChoosing(int mouseX, int mouseY);
    void renderButton(SDL_Renderer* &renderer, Gallery& gallery, int mouseX, int mouseY);
};

class Menu {
private:
    std::vector <Button> buttonList;
public:
    Menu() {}
    Menu(std::vector <std::string> _buttonName, std::vector <SDL_Rect> _buttonPosition,
        std::vector <SDL_Texture*> _imageNormal, std::vector <SDL_Texture*> _imageChoosing);
    void renderMenu(SDL_Renderer* &renderer, Gallery& gallery, int mouseX, int mouseY);
    std::string getPressedButton(int mouseX, int mouseY);
};

Menu loadMenuFromFile(std::string file, SDL_Renderer* &renderer, Gallery &gallery);

class Background {
private:
    int frame;
    State currentState;
    std::vector <PictureID> backgroundID;
public:
    Background();
    void setBackgroundState(State state) { currentState = state; frame = 0; }
    void renderBackground(SDL_Renderer* &renderer, Gallery &gallery);
};