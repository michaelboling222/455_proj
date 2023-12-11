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
    Sprite *sprite = new Sprite("./test_assets/Otter_Sprite_Sheet.png", game->renderer, 3, 12);

    // sprite->selectSprite(0, 0, 224);

    game->addLayer("./test_assets/forest.png");
    game->addLayer("./test_assets/Grassy_Gary2.png");

    Image *image2 = game->getLayer(0); // Gets the first layer from the add layer vector
    game->addTiles("./test_assets/Dirt.png");
    game->addTiles("./test_assets/deepDirt.png");
    game->addTiles("./test_assets/Grass.png");
    game->changeTile(2);
    game->setGridSize(32);
    int gridSize = game->getGridSize();
    game->initializeTileMap(gridSize, ScreenWidth, ScreenHeight);

    Uint64 lastFrameTime = SDL_GetPerformanceCounter();

    SDL_Event event;
    while (SDL_PollEvent(&event) >= 0)
    {
        Uint64 currentFrameTime = SDL_GetPerformanceCounter();
        game->setDeltaTime((currentFrameTime - lastFrameTime) / static_cast<double>(SDL_GetPerformanceFrequency()));
        lastFrameTime = currentFrameTime;
        switch (event.type)
        {
        case SDL_QUIT:
            game->save();
            exit(0);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                game->tilemap(gridSize, game->get_Screen_width(), game->get_Screen_height(), mouseX, mouseY);
            }
            break;

        case SDL_MOUSEMOTION:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                game->tilemap(gridSize, game->get_Screen_width(), game->get_Screen_height(), mouseX, mouseY);
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                // add scrolling here so when sprite moves forward the backround will scroll with it...
            case SDLK_d:
                game->moveRight(sprite, 10);
                game->set_backroundLocation(game->get_backroundLocation() + ScrollSpeed);
                break;
                // same logic except for backwards...
            case SDLK_a:
                game->moveLeft(sprite, 10);
                game->set_backroundLocation(game->get_backroundLocation() - ScrollSpeed);

                break;

            case SDLK_SPACE:
                game->jump(sprite, 10);
                break;

            case SDLK_4:
                game->respawn(sprite);
                break;
            case SDLK_r:
                game->respawn_x(sprite);
                break;
            case SDLK_f:
                game->spawn(sprite);
                break;
            }
        }
        // 320,180
        //  Clears the renderer, then copies the background and background copy to the render target, and then the foreground is copied.
        SDL_RenderClear(game->renderer);
        game->setRenderCopy(image2, 0, 0, 700, 180, game->get_Screen_width(), game->get_Screen_height());
        // game->grid(gridSize, ScreenWidth, ScreenHeight);
        game->renderTileMap();
        sprite->drawSelectedSprite(game->renderer);
        game->resolveCollisions(sprite);
        game->applyGravity(sprite);
        sprite->deccelerate();
        sprite->setState();
        sprite->animate(0,1, 2, 12);
        sprite->spriteJump();
        SDL_RenderPresent(game->renderer);
    }

    // Cleaning up all the variables once SDL_Quit() is called. Destroying the textures and surfaces, and then assigning all pointers used to nullptr
    game->window = nullptr;
    game->renderer = nullptr;
    image2 = nullptr;

    game->~Engine();
    return 0;
}