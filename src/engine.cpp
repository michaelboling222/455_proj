#include "engine.hpp"
#include <iostream>

#define NO_FLAGS 0

Engine::Engine(const char *window_title, int window_height, int window_width)
{

    if (SDL_Init(SDL_INIT_EVERYTHING) != -1)
    {
        setRunning(1);
        this->window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_RESIZABLE);
        if (this->window)
        {
            // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
            this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
            if (this->renderer)
            {   
                std::cout << "Everything checks out" << std::endl;
            }
            else
            {
                setRunning(0);
                SDL_DestroyWindow(this->window);
                SDL_Quit();
                std::cout << "Failed to initialize SDL renderer: " << SDL_GetError() << std::endl;
                return;
            }
    
        }
        else
        {
            setRunning(0);
            SDL_Quit();
            std::cout << "Failed to initialize SDL window: " << SDL_GetError() << std::endl;
            return;
        }
    }
    else
    {
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }
}


Engine::~Engine()
{
    for(Image* image: this->backgrounds)
    {
        SDL_DestroyTexture(image->accessTexture());
        SDL_FreeSurface(image->accessSurface());
    }
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
    IMG_Quit();
}


void Engine:: addLayer(const char* filename)
{
    Image* image = new Image(filename, this->renderer);
    this->backgrounds.emplace_back(image);
}


Image* Engine:: getLayer(int layer)
{
    return this->backgrounds.at(layer);
}


SDL_Rect Engine:: setImageRenderArea(int x, int y, int width, int height)
{
    SDL_Rect srcRect = {x, y, width, height};
    return srcRect;
}


SDL_Rect Engine:: setScreenRenderArea(int x, int y, int width, int height)
{
    SDL_Rect dstRect = {x, y, width, height};
    return dstRect;
}


void Engine:: setRenderCopy(Image* img, int x, int y, int width, int height, int ScreenWidth, int ScreenHeight)
{
    img->setSrcRect(x, y, width, height);
    img->setDstRect(x, y, ScreenWidth, ScreenHeight);

    SDL_Rect src = img->accessSrcRect();
    SDL_Rect dst = img->accessDstRect();
   
    SDL_RenderCopy(this->renderer, img->accessTexture(), &src, &dst);
}


void Engine :: addTiles(const char* filename)
{
    Tile art;
    art.surf = IMG_Load(filename);
    art.tex = SDL_CreateTextureFromSurface(renderer, art.surf);
    SDL_FreeSurface(art.surf);
    tiles.emplace_back(art);
}


void Engine :: grid(int gridSize, int ScreenWidth, int ScreenHeight)
{
    int maxRows = ScreenHeight / gridSize;  

    int maxCols = ScreenWidth / gridSize;

    int adjustedGridSize = std::min(ScreenHeight / maxRows, ScreenWidth / maxCols);

    for (int row = 0; row < maxRows; ++row) 
    {
        for (int col = 0; col < maxCols; ++col) 
        {
            SDL_RenderDrawLine(renderer, col * adjustedGridSize, 0, col * adjustedGridSize, ScreenHeight);
            SDL_RenderDrawLine(renderer, 0, row * adjustedGridSize, ScreenWidth, row * adjustedGridSize);
        }
}

}


void Engine :: initializeTileMap(int gridSize, int sWidth, int sHeight)
{
    const int numRows = sHeight/gridSize;
    const int numCols = sWidth/gridSize;
    this->tileMap.assign(numRows, std::vector<int>(numCols, -1));
}


void Engine :: tilemap(int gridSize,int ScreenWidth, int ScreenHeight, int mouseX, int mouseY)
{  
    int numRows = ScreenHeight/gridSize;
    int numCols = ScreenWidth/gridSize;
    int cellX = mouseX/gridSize;
    int cellY = mouseY/gridSize;
    
    // Check array bounds
    tileMap[cellY][cellX] = 0; // Assign a tile ID or texture ID
}


void Engine :: changeTile()
{
    int maxNum = tiles.size();
    if(tileNum < maxNum)
        ++tileNum;
    if(tileNum == maxNum)
        tileNum = 0;
}

void Engine :: renderTileMap()
{
    for (int row = 0; row < tileMap.size(); ++row) {
        for (int col = 0; col < tileMap[0].size(); ++col) {
            const int tileID = tileMap[row][col];
            if (tileID != -1) 
            {
                SDL_Rect tileRect = {col * gridSize, row * gridSize, gridSize, gridSize};
                SDL_RenderCopy(renderer, tiles[tileNum].tex, NULL, &tileRect);
            }
        }
    }
}

void Engine :: getDeltaTime()
{
    Uint32 currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastFrameTime) / 1000.0; // Convert to seconds
    lastFrameTime = currentTime;
}