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

class Button {
private:
    std::string buttonName;
    SDL_Rect buttonRect;
    SDL_Texture* buttonNormal;
    SDL_Texture* buttonChoosing;
public:
    Button() {};
    Button(std::string buttonName, SDL_Rect position, SDL_Texture* normal, SDL_Texture* choosing);
    bool isChoosing(int mouseX, int mouseY);
    void renderButton(SDL_Renderer* &renderer, int mouseX, int mouseY);
};

class Menu {
private:
    std::vector <Button> buttonList;
public:
    Menu() {}
    Menu(std::vector <std::string> _buttonName, std::vector <SDL_Rect> _buttonPosition, 
        std::vector <SDL_Texture*> _imageNormal, std::vector <SDL_Texture*> _imageChoosing);
    void renderMenu(SDL_Renderer* &renderer, int mouseX, int mouseY);
};

Menu loadMenuFromFile(std::string file);