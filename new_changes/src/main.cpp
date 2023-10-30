#include <iostream>
#include <cstdint>
#include "engine.hpp"
#define ScreenWidth 1920
#define ScreenHeight 1080
#define ScrollSpeed 1

int main()
{
    Engine *game = new Engine("Game", ScreenWidth, ScreenHeight);

    game->addLayer("./test_assets/Clouds3.png");
    game->addLayer("./test_assets/Grassy_Gary2.png");

    Image *image2 = game->getLayer(0); // Gets the first layer from the add layer vector

    game->addTiles("./test_assets/Dirt.png");
    game->addTiles("./test_assets/deepDirt.png");
    game->addTiles("./test_assets/Grass.png");

    game->setGridSize(64);
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
                game->tilemap(gridSize, 1920, 1080, mouseX, mouseY);
            }
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                game->changeTile();
            }

            break;
        }
        // Clears the renderer, then copies the background and background copy to the render target, and then the foreground is copied.
        SDL_RenderClear(game->renderer);
        game->setRenderCopy(image2, 0, 0, 320, 180, 1920, 1080);
        // game->setRenderCopy(image, 0, 0, 320, 180, 1920, 1080);
        game->grid(gridSize, ScreenWidth, ScreenHeight);
        game->renderTileMap();
        SDL_RenderPresent(game->renderer);
        SDL_Delay(10);
    }

    // Cleaning up all the variables once SDL_Quit() is called. Destroying the textures and surfaces, and then assigning all pointers used to nullptr
    game->window = nullptr;
    game->renderer = nullptr;
    image2 = nullptr;

    game->~Engine();
    return 0;
}