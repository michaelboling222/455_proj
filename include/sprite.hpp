#ifndef SPRITE_HPP
#define SPRITE_HPP
#include "SDL2/SDL.h"
#include <vector>
#include "SDL2/SDL_image.h"

class Sprite
{
public:
    Sprite(const char* filename, SDL_Renderer* rend, int animType);
    ~Sprite();

    void selectSprite(int x, int y, int spriteSize);
    void drawSelectedSprite(SDL_Renderer *renderer);
    void Gravity(float);
    void editToScreen(int, int, int, int);
    void editSprite(int, int, int, int);
    void spritePush(std::vector<SDL_Texture*>, SDL_Texture* tex);

    // setters:
    void setyVelocity(float v) { yVelocity = v; }
    void setxVelocity(float v) { xVelocity = v; }
    void setJumpVelocity(float v) { jumpVelocity = v; }
    void set_xcord(float x) { x_cord = x; }
    void set_ycord(float y) { y_cord = y; }
    void set_animatex(int x) { animate_x = x; }
    void set_animatey(int y) { animate_y = y; }

    // getters:
    double getJumpVelocity() { return jumpVelocity; }
    double ret_xcord() { return x_cord; }
    double ret_ycord() { return y_cord; }
    int get_animatex() { return animate_x; }
    int get_animatey() { return animate_y; }
    SDL_Rect accessToScreen();

    void spriteGrav();
    void spriteJump();
    void spriteMove();
    void spriteRespawn();
    void x_spriteRespawn();
    void spawn();
    void spriteCrouch();
    void changeSprite(int, int);

private:
    SDL_Rect sprite;
    SDL_Rect toScreen;
    int sheetRow;
    int sheetCol;
    SDL_Surface *spriteSurface;
    SDL_Texture *spriteTexture;

    int animate_y = 0;
    int animate_x = 0;
    double x_cord;
    double y_cord;
    double yVelocity;
    double xVelocity;
    double jumpVelocity;
};

#endif