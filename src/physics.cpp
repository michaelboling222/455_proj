#include "physics.hpp"

void PhysicsEngine :: applyGravity(Sprite* sprite, double delta)
{
    double speed  = gravity * delta;
    sprite->editToScreen(0, gravity, 0, 0);
    
}

bool PhysicsEngine :: resolveCollisions(Sprite* sprite)
{
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    auto Width = DM.w;
    auto Height = DM.h; 
    if(sprite->accessToScreen().y == Height)
    {
        return true;
    }
    return false;
}