#ifndef ENGINE_HPP
#define ENGINE_HPP
#define SDL_MAIN_HANDLED

#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include "image.hpp"

#include <functional>
#include <string>
#include <vector>

typedef int TileArtDescriptor;
typedef int SpriteArtDescriptor;
typedef int SpriteDescriptor;

#define DEFAULT_FRAME_RATE_CAP 60

class Engine
{
public:
    Engine(const char *window_title, int window_height, int window_width);
    ~Engine();

    void addLayer(const char* filename);
    Image* getLayer(int layer);
    SDL_Rect setImageRenderArea(int, int , int, int);
    SDL_Rect setScreenRenderArea(int, int , int, int);

    SDL_Renderer *renderer;
    SDL_Window *window;

    struct Tile 
    {
        SDL_Surface* surf;
        SDL_Texture* tex;
        int tileType;
        bool isWalkable;
    };


    void setRunning(int running)
    {
        switch (running)
        {
        case 1:
            isRunning = true;
            break;
        case 0:
            isRunning = false;
            break;
        }
    }
    void setGridSize(int i){this->gridSize = i;}
    int getGridSize(){return this->gridSize;}
    
    void Run();
    void setRenderCopy(Image*, int x, int y, int width, int height, int ScreenWidth, int ScreenHeight);

    void grid(int gridSize, int sWidth, int sHeight);
    void initializeTileMap(int gridSize, int sWidth, int sHeight);
    void renderTileMap();
    void changeTile();
    void addTiles(const char* filename);
    void tilemap(int gridSize,int ScreenWidth, int ScreenHeight, int mouseX, int mouseY);


private:
    std::vector<Image*> backgrounds;
    std::vector<Tile> tiles;
    std::vector<std::vector<int> > tileMap;
    int gridSize;
    int tileNum = 0;
    bool isRunning;
};

#endif