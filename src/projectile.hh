#ifndef PROJECTILE_HH
#define PROJECTILE_HH

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "projectilesprites.hh"
#include "mob.hh"

class Projectile
{

public:

    Projectile (CL_Vec2f location, int velocity, ProjectileSprites sprites, Mob* target, Level& level);
    virtual ~Projectile();

    CL_Sprite& getSprite();
    CL_Vec2f getLocation();

    virtual void update() = 0;

    bool isDestroyed();
    void setDestroyed();

    void onAnimationFinished();

protected:

    // Float point coordinates of the projectile.
    CL_Vec2f location;

    // Velocity of the projectile, used as a multiplier for a constant speed.
    int velocity;

    // SlotContainer used for catching CL_Signals.
    CL_SlotContainer slots;
    ProjectileSprites sprites;

    // Current angle of the projectile in radians.
    CL_Angle angle;

    // The target of the projectile.
    Mob* target;
    Level& level;

    size_t spriteWidth;
    size_t spriteHeight;

    bool destroyed;
};

class BasicProjectile : public Projectile
{

public:

    BasicProjectile(CL_Vec2f location, int velocity, ProjectileSprites sprites, Mob* target, Level &level);
    void update();

private:


};

#endif
