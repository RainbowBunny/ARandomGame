// Custom library
#include <gallery.h>
#include <utils.h>

// SDL2 library

// Standard library
#include <string>
#include <algorithm>

Gallery::Gallery(SDL_Renderer* _renderer) {
    renderer = _renderer;
    loadGamePictures();
}

Gallery::~Gallery() {
    std::vector <std::vector <SDL_Texture*> > tmp;
    std::swap(pictures, tmp);
}

std::vector <SDL_Texture*> Gallery::loadTextureFromImage(std::string path, 
    int numberOfFrame, std::string extension, SDL_BlendMode blendMode) {
    /*
        This function is used to load an animation series
        Input:
            Path (string): The path to the animation list
            numberOfFrame (int): Number of frame of the animation
        Output:
            animation (vector <SDL_Texture*>): The loaded animation of the object.
    */
    std::vector <SDL_Texture*> animation(numberOfFrame);
    for (int i = 0; i < numberOfFrame; i++) {
        SDL_Surface* loadedSurface = IMG_Load((path + "_" + std::to_string(i) + extension).c_str());
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
        if (loadedSurface == nullptr) {
            logSDLError(std::cout, "Can not load image: " + path, true);            
        }
        animation[i] = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }

    return animation;
}

void Gallery::loadGamePictures() {
    /*
    enum PictureID {
        CAT = 0,
        BURNING,
        PROTECTED,
        GRASS,
        GAME_WIN,
        GAME_LOSE,
        BUTTON, 
        BACK_GROUND,
        PIC_COUNT
    };
    */
    pictures.push_back(loadTextureFromImage("image/cat", 1, ".png", SDL_BLENDMODE_BLEND));
    pictures.push_back(loadTextureFromImage("image/fire_animation/fire", 162, ".jpg", SDL_BLENDMODE_NONE));
    pictures.push_back(loadTextureFromImage("image/water_animation/water", 180, ".jpg", SDL_BLENDMODE_NONE));
    pictures.push_back(loadTextureFromImage("image/grass_background", 1, ".jpg", SDL_BLENDMODE_NONE));
    pictures.push_back(loadTextureFromImage("image/game_winning", 1, ".jpg", SDL_BLENDMODE_NONE));
    pictures.push_back(loadTextureFromImage("image/game_losing", 1, ".jpg", SDL_BLENDMODE_NONE));
    pictures.push_back(loadTextureFromImage("image/button", 1, ".png", SDL_BLENDMODE_NONE));
    pictures.push_back(loadTextureFromImage("image/menu_background", 1, ".jpg", SDL_BLENDMODE_NONE));
}