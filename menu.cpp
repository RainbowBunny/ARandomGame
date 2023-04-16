// Custom library
#include <menu.h>

// SDL2 library

// Standard library

Button::Button(std::string _buttonName, SDL_Rect position, SDL_Texture* normal, SDL_Texture* choosing) {
    buttonName = _buttonName;
    buttonRect = position;
    buttonNormal = normal;
    buttonChoosing = choosing;
}

bool Button::isChoosing(int mouseX, int mouseY) {
    return mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
        mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h;
}

void Button::renderButton(SDL_Renderer* &renderer, int mouseX, int mouseY) {
    if (isChoosing(mouseX, mouseY)) {
        SDL_RenderCopy(renderer, buttonChoosing, nullptr, &buttonRect);
    } else {
        SDL_RenderCopy(renderer, buttonNormal, nullptr, &buttonRect);
    }
}

Menu::Menu(std::vector <std::string> _buttonName, std::vector <SDL_Rect> _buttonPosition, 
    std::vector <SDL_Texture*> _imageNormal, std::vector <SDL_Texture*> _imageChoosing) {
    buttonList.resize((int)_buttonName.size());
    for (int i = 0; i < (int)_buttonName.size(); i++) {
        buttonList[i] = Button(_buttonName[i], _buttonPosition[i], _imageNormal[i], _imageChoosing[i]);
    }
}

void Menu::renderMenu(SDL_Renderer* &renderer, int mouseX, int mouseY) {
    for (int i = 0; i < (int)buttonList.size(); i++) {
        buttonList[i].renderButton(renderer, mouseX, mouseY);
    }
}

Menu loadMenuFromFile(std::string file, SDL_Renderer* &renderer, Gallery &gallery) {
    /*
        Data format:
        name x y w h content
        string name: name of the button
        x y w h - SDL_Rect, position of the menu button
        string content: the text string on the menu
    */
    std::ifstream fin(file);
    int numberOfButton;
    fin >> numberOfButton;
    std::vector <std::string> buttonName(numberOfButton);
    std::vector <SDL_Rect> buttonPosition(numberOfButton);
    std::vector <std::string> buttonContent(numberOfButton);

    for (int i = 0; i < numberOfButton; i++) {
        fin >> buttonName[i];
        fin >> buttonPosition[i].x >> buttonPosition[i].y >> buttonPosition[i].w >> buttonPosition[i].h;
        fin >> buttonContent[i];
    }

    for (int i = 0; i < numberOfButton; i++) {
        
    }
}