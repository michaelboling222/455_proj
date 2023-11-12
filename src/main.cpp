#include <iostream>
#include <cstdint>
#include "engine.hpp"
#include "sprite.hpp"
#include "physics.hpp"
#define ScreenWidth 2560
#define ScreenHeight 1440
#define ScrollSpeed 1

int main()
{
    Engine *game = new Engine("Game", ScreenWidth, ScreenHeight);
    Sprite *sprite = new Sprite("./test_assets/Elmo.png", game->renderer, 5, 4);

    sprite->selectSprite(0, 0, 224);

    game->addLayer("./test_assets/Clouds3.png");
    game->addLayer("./test_assets/Grassy_Gary2.png");

    Image *image2 = game->getLayer(0); // Gets the first layer from the add layer vector

    game->addTiles("./test_assets/Dirt.png");
    game->addTiles("./test_assets/deepDirt.png");
    game->addTiles("./test_assets/Grass.png");

    game->setGridSize(32);
    int gridSize = game->getGridSize();
    game->initializeTileMap(gridSize, ScreenWidth, ScreenHeight);

    SDL_Event event;
    while (SDL_PollEvent(&event) >= 0)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            exit(0);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                game->tilemap(gridSize, 2560, 1440, mouseX, mouseY);
            }
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                game->changeTile();
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {

            case SDLK_d:
                game->moveRight(sprite, 10);
                break;

            case SDLK_a:
                game->moveLeft(sprite, 10);
                break;

            case SDLK_SPACE:
                game->jump(sprite, 500);
                break;
            }
        }
        // Clears the renderer, then copies the background and background copy to the render target, and then the foreground is copied.
        SDL_RenderClear(game->renderer);
        game->getDeltaTime();
        game->setRenderCopy(image2, 0, 0, 320, 180, 2560, 1440);
        game->grid(gridSize, ScreenWidth, ScreenHeight);
        game->renderTileMap();
        sprite->drawSelectedSprite(game->renderer);
        game->resolveCollisions(sprite);
        game->applyGravity(sprite);
        SDL_RenderPresent(game->renderer);
    }

    // Cleaning up all the variables once SDL_Quit() is called. Destroying the textures and surfaces, and then assigning all pointers used to nullptr
    game->window = nullptr;
    game->renderer = nullptr;
    image2 = nullptr;

    game->~Engine();
    return 0;
}