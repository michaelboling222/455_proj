// add properites for sprites (user control, height, width, filepath), location , physics properties
//(increase speed, jump height...etc), change animation, isprojectile bool (if projectile then kill player)
// foreground tile, does it collide, and filepath

#include "physics.hpp"

void PhysicsEngine ::applyGravity(Sprite *sprite, double delta)
{
    double speed = gravity * delta;
    sprite->editToScreen(0, gravity, 0, 0);
}
