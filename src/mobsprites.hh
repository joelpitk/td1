#ifndef MOBSPRITES_HH
#define MOBSPRITES_HH

struct MobSprites
{
    CL_Sprite mobSprite; // This is the main sprite updated by Mob

    //These are animations which are set to mobSprite with set_image_data
    CL_Sprite walkSprite;
    CL_Sprite damageSprite;
    CL_Sprite deadSprite;

};

#endif
