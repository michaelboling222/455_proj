#ifndef TILEMAP_HPP
#define TILEMAP_HPP
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include<vector>

class TileMap
{
public:
    struct Tile
    {
        SDL_Surface *surf;
        SDL_Texture *tex;
        int tileType;
        bool isWalkable;
    };

    void addTiles(const char *filename);
    void initializeTileMap(int gridSize, int sWidth, int sHeight);
    void tilemap(int gridSize, int ScreenWidth, int ScreenHeight, int mouseX, int mouseY);
    void changeTile();
    void renderTileMap();


private:
    std::vector<std::vector<int> > tileMap;
    int gridSize;
    std::vector<Tile> tiles;
};

#endif