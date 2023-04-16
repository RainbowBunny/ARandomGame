#pragma once
#ifndef GALLERY_H
#define GALLERY_H
// Custom library

// SDL2 library
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Standard library
#include <string>
#include <vector>

// enumeration for different types of image.
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

class Gallery {
/*
    This is the class to load all the image in the game
*/
private:
    std::vector <std::vector <SDL_Texture*> > pictures;
    SDL_Renderer* renderer;
    std::vector <SDL_Texture*> loadTextureFromImage(std::string path, 
        int numberOfFrame, std::string extension, SDL_BlendMode blendMode);
    SDL_Texture* loadTextureFromText(std::string textString, SDL_Color textColor);
public:
    Gallery(SDL_Renderer* _renderer);
    ~Gallery();

    void loadGamePictures();
    std::vector <SDL_Texture*> getImage(PictureID id) const { return pictures[id]; }
    SDL_Texture* getFrame(PictureID obj, int currentFrame) const { return pictures[obj][currentFrame % (int)pictures[obj].size()]; }
};

#endif