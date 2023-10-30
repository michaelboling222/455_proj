#include "sprite.hpp"

Sprite ::Sprite(const char *filename, SDL_Renderer *rend, int row, int col)
{
    spriteSheet = IMG_Load(filename);
    spriteSheetTexture = SDL_CreateTextureFromSurface(rend, spriteSheet);

    sprite.w = spriteSheet->w / col;
    sprite.h = spriteSheet->h / row;

    SDL_FreeSurface(spriteSheet);
}

Sprite ::~Sprite()
{
    SDL_DestroyTexture(spriteSheetTexture);
}

void Sprite ::selectSprite(int x, int y)
{
    sprite.x = x * sprite.w;
    sprite.y = y * sprite.h;
}

void Sprite ::drawSelectedSprite(SDL_Renderer *renderer, SDL_Rect *posRect)
{
    SDL_RenderCopy(renderer, spriteSheetTexture, &sprite, posRect);
}