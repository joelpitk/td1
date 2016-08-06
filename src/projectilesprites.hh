#ifndef PROJECTILESPRITES_HH
#define PROJECTILESPRITES_HH

struct ProjectileSprites
{
    // This is the main sprite updated by Projectile.
    CL_Sprite projectileSprite;

    // These are animations which are set to projectileSprite with set_image_data
    CL_Sprite flySprite;
    CL_Sprite damageSprite;
};

#endif
