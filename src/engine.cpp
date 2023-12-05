#include "engine.hpp"
#include "sprite.hpp"

#include <iostream>

#define NO_FLAGS 0

Engine::Engine(const char *window_title, int window_width, int window_height)

{
    this->window_title = window_title;
    this->window_height = window_height;
    this->window_width = window_width;

    if (SDL_Init(SDL_INIT_EVERYTHING) != -1)
    {
        setRunning(1);

        this->window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_RESIZABLE);
        if (this->window)
        {
            // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
            this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
            set_Screen_width(window_width);
            set_Screen_height(window_height);

            if (this->renderer)
            {

                std::cout << "Everything checks out" << std::endl;
                std::cout << "height is" << this->window_height << std::endl;
                std::cout << "width is" << this->window_width << std::endl;
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
    for (Image *image : this->backgrounds)
    {
        SDL_DestroyTexture(image->accessTexture());
        SDL_FreeSurface(image->accessSurface());
    }
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
    IMG_Quit();
}

// add image and push it to backrounds vector
void Engine::addLayer(const char *filename)
{
    Image *image = new Image(filename, this->renderer);
    this->backgrounds.emplace_back(image);
}

// will be used to get the image that was pushed into the backrounds vector
Image *Engine::getLayer(int layer)
{
    return this->backgrounds.at(layer);
}

//*
SDL_Rect Engine::setImageRenderArea(int x, int y, int width, int height)
{
    SDL_Rect srcRect = {x, y, width, height};
    return srcRect;
}

//*
SDL_Rect Engine::setScreenRenderArea(int x, int y, int width, int height)
{
    SDL_Rect dstRect = {x, y, width, height};
    return dstRect;
}

// function to render image on screen
void Engine::setRenderCopy(Image *img, int x, int y, int width, int height, int ScreenWidth, int ScreenHeight)
{
    // adding new position for x val to where the backround image will be rendered:
    img->setSrcRect(get_backroundLocation(), y, width, height);
    // set the new location applicable to the users screen dimensions:
    img->setDstRect(x, y, ScreenWidth, ScreenHeight);

    SDL_Rect src = img->accessSrcRect();
    SDL_Rect dst = img->accessDstRect();

    SDL_RenderCopy(this->renderer, img->accessTexture(), &src, &dst);
}

// function to add appropriate image(s) to a tiles vector of Tile obj's
// it will contain the rendered texture of the given img
void Engine ::addTiles(const char *filename)
{
    Tile art;
    art.surf = IMG_Load(filename);
    art.tex = SDL_CreateTextureFromSurface(renderer, art.surf);
    SDL_FreeSurface(art.surf);
    tiles.emplace_back(art);
}

// renders grid on screen
void Engine ::grid(int gridSize, int ScreenWidth, int ScreenHeight)
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

// does proper initalization and manipulates the tileMap vector based on given parameters of the function
void Engine ::initializeTileMap(int gridSize, int sWidth, int sHeight)
{

    const int numRows = sHeight / gridSize;
    const int numCols = sWidth / gridSize;
    this->tileMap.assign(numRows, std::vector<int>(numCols, -1));
}

void Engine ::tilemap(int gridSize, int ScreenWidth, int ScreenHeight, int mouseX, int mouseY)
{
    int numRows = ScreenHeight / gridSize;
    int numCols = ScreenWidth / gridSize;
    int cellX = mouseX / gridSize;
    int cellY = mouseY / gridSize;

    tileMap[cellY][cellX] = 0;
}

void Engine ::changeTile()
{
    int maxNum = tiles.size();
    if (tileNum < maxNum)
        ++tileNum;
    if (tileNum == maxNum)
        tileNum = 0;
}

void Engine ::renderTileMap()
{
    for (int row = 0; row < tileMap.size(); ++row)
    {
        for (int col = 0; col < tileMap[0].size(); ++col)
        {
            const int tileID = tileMap[row][col];
            if (tileID != -1)
            {
                SDL_Rect tileRect = {col * gridSize, row * gridSize, gridSize, gridSize};
                SDL_RenderCopy(renderer, tiles[tileNum].tex, NULL, &tileRect);
            }
        }
    }
}

bool Engine ::resolveCollisions(Sprite *sprite)
{
    SDL_Rect spriteBoundingBox = sprite->accessToScreen();
    for (int row = 0; row < tileMap.size(); ++row)
    {
        for (int col = 0; col < tileMap[0].size(); ++col)
        {
            if (tileMap[row][col] == 0)
            {
                SDL_Rect tileBoundingBox = {col * gridSize, row * gridSize, gridSize, gridSize};
                if (SDL_HasIntersection(&spriteBoundingBox, &tileBoundingBox))
                {
                    // Handle the collision
                    // For example, stop the sprite from falling
                    // std::cout << "COLLISION!" << std::endl;
                    return true;
                }
            }
        }
    }
    return false;
}

void Engine ::applyGravity(Sprite *sprite)
{
    if (!resolveCollisions(sprite))
    {
        sprite->setyVelocity(gravity * deltaTime);
        sprite->spriteGrav();
        // std::cout << "MOVING!!" << std::endl;
    }
    else
        sprite->setyVelocity(0);
}

void Engine ::moveRight(Sprite *sprite, int speed)
{
    sprite->setxVelocity(speed);
    sprite->spriteMove();
    // std::cout << "MOVING!!" << std::endl;
}

void Engine ::moveLeft(Sprite *sprite, int speed)
{
    sprite->setxVelocity(-speed);
    sprite->spriteMove();
    // std::cout << "MOVING!!" << std::endl;
}

void Engine ::jump(Sprite *sprite, int height)
{
    if (resolveCollisions(sprite))
    {

        for (int y = 0; y < height; y++)
        {

            sprite->setJumpVelocity(-y);
            sprite->spriteJump();

            std::cout << "the val of jump vel is:" << sprite->getJumpVelocity() << std::endl;
        }

        // std::cout << "MOVING!!" << std::endl;
    }
    else
        sprite->setJumpVelocity(0);
}
void Engine ::respawn(Sprite *sprite)
{
    sprite->spriteRespawn();
}
void Engine ::respawn_x(Sprite *sprite)
{
    sprite->set_animatex(0);
    sprite->x_spriteRespawn();
}
void Engine ::spawn(Sprite *sprite)
{
    std::cout << "the value of x animate is.." << sprite->get_animatex() << std::endl;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            sprite->selectSprite(i, j, 150);
        }
    }
    sprite->selectSprite(0, 0, 150);
}
void Engine ::crouch(Sprite *sprite)
{
    if (sprite->get_animatex() % 2 == 0)
    {
        sprite->set_animatex(3);
        sprite->spriteCrouch();
    }
    else
    {
        sprite->set_animatex(0);
        sprite->spriteCrouch();
    }
}
