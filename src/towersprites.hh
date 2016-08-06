#ifndef TOWERSPRITES_HH
#define TOWERSPRITES_HH

struct TowerSprites
{
    //These are animations which are set to mobSprite with set_image_data
    CL_Sprite baseSprite;
    CL_Sprite gunSprite;
    CL_Sprite upgradeSprite;

    // Sprites for the different tower levels.
    CL_Sprite lvl_1;
    CL_Sprite lvl_2;
    CL_Sprite lvl_3;
};

#endif
