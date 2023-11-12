#ifndef SPRITE_HPP
#define SPRITE_HPP
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

class Sprite
{
public:
    Sprite(const char *filename, SDL_Renderer *rend, int row, int col);
    ~Sprite();

    void selectSprite(int x, int y, int spriteSize);
    void drawSelectedSprite(SDL_Renderer *renderer);
    void Gravity(float);
    void editToScreen(int, int, int, int);
    SDL_Rect accessToScreen();

    void setyVelocity(float v) { yVelocity = v; }
    void setxVelocity(float v) { xVelocity = v; }
    void setJumpVelocity(float v) { jumpVelocity = v; }

    void spriteGrav();
    void spriteJump();
    void spriteMove();

private:
    SDL_Rect sprite;
    SDL_Rect toScreen;
    int sheetRow;
    int sheetCol;
    SDL_Surface *spriteSheet;
    SDL_Texture *spriteSheetTexture;

    double yVelocity;
    double xVelocity;
    double jumpVelocity;
};

#endif