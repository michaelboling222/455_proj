#ifndef SPRITE_HPP
#define SPRITE_HPP
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

class Sprite
{
public:
    Sprite(const char *filename, SDL_Renderer *rend, int row, int col);
    ~Sprite();

    void selectSprite(int x, int y);
    void drawSelectedSprite(SDL_Renderer *renderer, SDL_Rect *posRect);

private:
    SDL_Rect sprite;
    SDL_Surface *spriteSheet;
    SDL_Texture *spriteSheetTexture;
};

#endif