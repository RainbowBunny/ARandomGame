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
    int numberOfFrame, std::string extension) {
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
        if (loadedSurface == nullptr) {
            logSDLError(std::cout, "Can not load image: " + path, true);            
        }
        animation[i] = SDL_CreateTextureFromSurface(renderer, loadedSurface);
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
        PIC_COUNT
    };
    */
    pictures.push_back(loadTextureFromImage("image/cat", 1, ".png"));
    pictures.push_back(loadTextureFromImage("image/burning", 1, ".png"));
    pictures.push_back(loadTextureFromImage("image/protected", 1, ".png"));
    pictures.push_back(loadTextureFromImage("image/grass_background", 1, ".png"));
    pictures.push_back(loadTextureFromImage("image/game_winning", 1, ".jpg"));
    pictures.push_back(loadTextureFromImage("image/game_losing", 1, ".jpg"));
}