#include "tilemap.hpp"


void TileMap :: addTiles(const char *filename)
{
    Tile art;
    art.surf = IMG_Load(filename);
    SDL_FreeSurface(art.surf);
    tiles.emplace_back(art);
}



void TileMap :: initializeTileMap(int gridSize, int sWidth, int sHeight)
{

    const int numRows = sHeight / gridSize;
    const int numCols = sWidth / gridSize;
    this->tileMap.assign(numRows, std::vector<int>(numCols, -1));
}

void TileMap :: tilemap(int gridSize, int ScreenWidth, int ScreenHeight, int mouseX, int mouseY)
{
    int numRows = ScreenHeight / gridSize;
    int numCols = ScreenWidth / gridSize;
    int cellX = mouseX / gridSize;
    int cellY = mouseY / gridSize;

    tileMap[cellY][cellX] = 0;
}

void TileMap :: changeTile()
{
    
}

void TileMap :: renderTileMap()
{
    for (int row = 0; row < tileMap.size(); ++row)
    {
        for (int col = 0; col < tileMap[0].size(); ++col)
        {
            const int tileID = tileMap[row][col];
            if (tileID != -1)
            {
                SDL_Rect tileRect = {col * gridSize, row * gridSize, gridSize, gridSize};
            }
        }
    }
}