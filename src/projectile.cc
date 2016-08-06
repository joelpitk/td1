#include "projectile.hh"

Projectile::Projectile (CL_Vec2f location, int velocity, ProjectileSprites sprites, Mob* target, Level& level)
  : location(location), velocity(velocity), sprites(sprites), target(target), level(level), destroyed(false)
{
    spriteWidth = sprites.flySprite.get_width();
    spriteHeight = sprites.flySprite.get_height();

    angle = CL_Angle::from_radians(0);
    sprites.projectileSprite.set_image_data(sprites.flySprite);

    slots.connect(sprites.projectileSprite.sig_animation_finished(), this, &Projectile::onAnimationFinished);
}

Projectile::~Projectile() { }

CL_Sprite& Projectile::getSprite()
{
    return sprites.projectileSprite;
}

// Returns the location of the projectile for renderer.
CL_Vec2f Projectile::getLocation()
{
    return location;
}

// Returns true if the projectile has reached its target.
bool Projectile::isDestroyed()
{
    return destroyed;
}

// Set true once the projectile has reached is target.
void Projectile::setDestroyed()
{
    destroyed = true;
}

void Projectile::onAnimationFinished()
{
    sprites.projectileSprite.set_image_data(sprites.flySprite);
}

BasicProjectile::BasicProjectile(CL_Vec2f location, int velocity, ProjectileSprites sprites, Mob* target, Level& level) : Projectile(location, velocity, sprites, target, level)
{

}

void BasicProjectile::update()
{
    if (destroyed || target == NULL || target->isDead())
    {
        destroyed = true;
        return;
    }

    CL_Vec2f targetLocation = target->getCenterLocation();


    
    location += (targetLocation - getLocation()).normalize() * velocity;

    int spriteWidth = target->getSprite().get_width();
    int spriteHeight = target->getSprite().get_height();

    if (location.x >= targetLocation.x && location.x <= targetLocation.x + spriteWidth)
    {
        if (location.y >= targetLocation.y && location.y <= targetLocation.y + spriteHeight)
        {
            setDestroyed();
        }
    }
}
