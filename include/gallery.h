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
    PIC_COUNT
};

class Gallery {
/*
    This is the class to load all the image in the game
*/
private:
    std::vector <std::vector <SDL_Texture*> > pictures;
    SDL_Renderer* renderer;
    std::vector <SDL_Texture*> loadTextureFromImage(std::string path, int numberOfFrame);
public:
    Gallery(SDL_Renderer* _renderer);
    ~Gallery();

    void loadGamePictures();
    std::vector <SDL_Texture*> getImage(PictureID id) const { return pictures[id]; }
    SDL_Texture* getFrame(PictureID obj, int currentFrame) const { return pictures[obj][currentFrame % (int)pictures[obj].size()]; }
};