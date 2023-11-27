#include "sprite.hpp"
#include <iostream>

Sprite::Sprite(const char* filename, SDL_Renderer* rend, int row, int col)
{
    sheetRow = row;
    sheetCol = col;

    spriteSheet = IMG_Load(filename);
    spriteSheetTexture = SDL_CreateTextureFromSurface(rend, spriteSheet);
    SDL_SetSurfaceBlendMode(spriteSheet, SDL_BLENDMODE_BLEND);

    sprite.w = spriteSheet->w / col;
    sprite.h = spriteSheet->h / row;

    SDL_FreeSurface(spriteSheet);
}


Sprite ::~Sprite()
{
    SDL_DestroyTexture(spriteSheetTexture);
}

void Sprite ::selectSprite(int x, int y, int spriteSize)
{
    sprite.x = x * sprite.w;
    sprite.y = y * sprite.h;

    toScreen.x = 0;
    toScreen.y = 0;
    toScreen.w = spriteSize;
    toScreen.h = spriteSize;
}
void Sprite::changeSprite(int x, int y)
{
    sprite.x = x * sprite.w;
    sprite.y = y * sprite.h;
}

void Sprite ::drawSelectedSprite(SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, spriteSheetTexture, &sprite, &toScreen);
}

void Sprite ::editToScreen(int x, int y, int size, int size2)
{
    toScreen.x += x;
    toScreen.y += y;
    toScreen.w += size;
    toScreen.h += size2;
}

void Sprite ::editSprite(int x, int y, int size, int size2)
{
    sprite.x += x;
    sprite.y += y;
    sprite.w += size;
    sprite.h += size2;
}

SDL_Rect Sprite ::accessToScreen()
{
    return toScreen;
}

void Sprite ::spriteGrav()
{
    if (this->yVelocity > 0)
        editToScreen(0, yVelocity, 0, 0);
}

void Sprite ::spriteMove()
{
    if (this->xVelocity > 0)
        editToScreen(xVelocity, 0, 0, 0);
    else if (this->xVelocity < 0)
        editToScreen(xVelocity, 0, 0, 0);
}

void Sprite ::spriteJump()
{
    int jump = 0;
    if (this->jumpVelocity < 0)
    {
        while(jump > jumpVelocity)
        {
            editToScreen(0,jump, 0, 0);
            --jump;
        }
    }
}
void Sprite ::spriteRespawn()
{
    toScreen.x = 0;
    toScreen.y = 0;
}
void Sprite ::x_spriteRespawn()
{
    int temp = 0;
    toScreen.y = temp;
    editToScreen(0, toScreen.y, 0, 0);
}

// there is 1 frame where the sprite will be shown in
// top left corner...refer to changeSprite() to fix...
void Sprite ::spriteCrouch()
{
    double xVal, yVal;
    xVal = toScreen.x;
    yVal = toScreen.y;
    selectSprite(animate_x, animate_y, 150);
    editToScreen(xVal, yVal, 0, 0);
}