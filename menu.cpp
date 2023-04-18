// Custom library
#include <menu.h>

// SDL2 library

// Standard library
#include <unistd.h>

Button::Button(std::string _buttonName, SDL_Rect position, SDL_Texture* normal, SDL_Texture* choosing) {
    buttonName = _buttonName;
    buttonRect = position;
    buttonNormal = normal;
    buttonChoosing = choosing;
    textRect.x = buttonRect.x + 20; textRect.y = buttonRect.y + 20;
    textRect.w = buttonRect.w - 40; textRect.h = buttonRect.h - 40;
}

bool Button::isChoosing(int mouseX, int mouseY) {
    return mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
        mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h;
}

void Button::renderButton(SDL_Renderer* &renderer, Gallery& gallery, int mouseX, int mouseY) {
    SDL_RenderCopy(renderer, gallery.getFrame(BUTTON, frame), nullptr, &buttonRect);
    frame++;
    if (isChoosing(mouseX, mouseY)) {
        SDL_RenderCopy(renderer, buttonChoosing, nullptr, &textRect);
    } else {
        SDL_RenderCopy(renderer, buttonNormal, nullptr, &textRect);
    }
}

Menu::Menu(std::vector <std::string> _buttonName, std::vector <SDL_Rect> _buttonPosition, 
    std::vector <SDL_Texture*> _imageNormal, std::vector <SDL_Texture*> _imageChoosing) {
    buttonList.resize((int)_buttonName.size());
    for (int i = 0; i < (int)_buttonName.size(); i++) {
        buttonList[i] = Button(_buttonName[i], _buttonPosition[i], _imageNormal[i], _imageChoosing[i]);
    }
}

void Menu::renderMenu(SDL_Renderer* &renderer, Gallery& gallery, int mouseX, int mouseY) {
    for (int i = 0; i < (int)buttonList.size(); i++) {
        buttonList[i].renderButton(renderer, gallery, mouseX, mouseY);
    }
}

Uint8 castingIntToUint8(int target) { return target % (1 << 8); }

Menu loadMenuFromFile(std::string file, SDL_Renderer* &renderer, Gallery &gallery) {
    /*
        Data format:
        name x y w h content
        string name: name of the button
        x y w h - SDL_Rect, position of the menu button
        string content: the text string on the menu
    */
    std::ifstream fin(file);
    if (!fin.is_open()) {
        logError(std::cout, "Can not open file: " + file, true);
    }

    int numberOfButton;
    fin >> numberOfButton;
    std::vector <std::string> buttonName(numberOfButton);
    std::vector <SDL_Rect> buttonPosition(numberOfButton);
    std::vector <std::string> buttonContent(numberOfButton);
    std::vector <SDL_Color> normalButtonTextColor(numberOfButton);
    std::vector <SDL_Color> choosingButtonTextColor(numberOfButton);
    std::vector <SDL_Texture*> buttonNormal(numberOfButton);
    std::vector <SDL_Texture*> buttonChoosing(numberOfButton);

    for (int i = 0; i < numberOfButton; i++) {
        std::string tmp;
        getline(fin, tmp);
        getline(fin, buttonName[i]);
        fin >> buttonPosition[i].x >> buttonPosition[i].y >> buttonPosition[i].w >> buttonPosition[i].h;
        getline(fin, tmp);
        getline(fin, buttonContent[i]);

        int r1, b1, g1, r2, b2, g2;
        fin >> r1 >> b1 >> g1;
        normalButtonTextColor[i].r = castingIntToUint8(r1);
        normalButtonTextColor[i].g = castingIntToUint8(b1);
        normalButtonTextColor[i].b = castingIntToUint8(g1);
        fin >> r2 >> b2 >> g2;
        choosingButtonTextColor[i].r = castingIntToUint8(r2);
        choosingButtonTextColor[i].g = castingIntToUint8(b2);
        choosingButtonTextColor[i].b = castingIntToUint8(g2);

        std::cout << "Reading: " << buttonName[i] << " " << buttonContent[i] << std::endl;

        buttonNormal[i] = gallery.loadTextureFromText(buttonContent[i], normalButtonTextColor[i]);
        buttonChoosing[i] = gallery.loadTextureFromText(buttonContent[i], choosingButtonTextColor[i]);
    }

    std::cout << "Finished reading menu: " + file << std::endl;

    Menu current = Menu(buttonName, buttonPosition, buttonNormal, buttonChoosing);
    // for (int i = 0; i < numberOfButton; i++) {
    //     SDL_DestroyTexture(buttonNormal[i]);
    //     SDL_DestroyTexture(buttonChoosing[i]);
    // }

    return current;
}

std::string Menu::getPressedButton(int mouseX, int mouseY) {
    for (int i = 0; i < (int)buttonList.size(); i++) {
        if (buttonList[i].isChoosing(mouseX, mouseY)) {
            return buttonList[i].getButtonName();
        }
    }
    return "none";
}

Background::Background() {
    /*
    enum State {
        MAIN_MENU = 0,
        GAME_BACKGROUND,
        PAUSE_MENU,
        GAME_WINNING,
        GAME_LOSING,
        TOTALSTATE
    };
    */

    currentState = MAIN_MENU;

    backgroundID.push_back(BACKGROUND);
    backgroundID.push_back(GAME);
    backgroundID.push_back(GAME);
    backgroundID.push_back(GAME_WIN);
    backgroundID.push_back(GAME_LOSE);
}

void Background::renderBackground(SDL_Renderer* &renderer, Gallery &gallery) {
    SDL_RenderCopy(renderer, gallery.getFrame(backgroundID[currentState], frame), nullptr, nullptr);
    frame++;
}

void Textbox::renderTextBox(SDL_Renderer* &renderer, Gallery &gallery) {
    if (background != NONE) {
        std::cout << 1 << std::endl;
        std::cout << backgroundRect.x << " " << backgroundRect.y << " " << backgroundRect.w << " " << backgroundRect.h << std::endl;
        SDL_RenderCopy(renderer, gallery.getFrame(background, frame), nullptr, &backgroundRect);
        frame++;
    }
    
    if ((int)textString.size() > 0) {
        std::cout << 2 << std::endl;
        SDL_RenderCopy(renderer, gallery.loadTextureFromText(textString, textColor), nullptr, &textRect);
    }
}

Textbox createTextboxFromFile(std::ifstream &fin, PictureID id) {
    SDL_Rect _backgroundRect, _textRect;
    SDL_Color _color;
    fin >> _backgroundRect.x >> _backgroundRect.y >> _backgroundRect.w >> _backgroundRect.h;
    std::cout << _backgroundRect.x << " " << _backgroundRect.y << " " << _backgroundRect.w << " " << _backgroundRect.h << std::endl;
    fin >> _textRect.x >> _textRect.y >> _textRect.w >> _textRect.h;
    int r, g, b;
    fin >> r >> g >> b;
    _color.r = castingIntToUint8(r);
    _color.g = castingIntToUint8(g);
    _color.b = castingIntToUint8(b);
    return Textbox(id, _backgroundRect, _textRect, _color);
}