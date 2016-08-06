#include "mobfactory.hh"
#include "mobsprites.hh"
#include "mob.hh"

MobFactory::MobFactory(int monster_id, CL_GraphicContext& gc, CL_ResourceManager& resources)
    : monster_id(monster_id), gc(gc), resources(resources)
{ }

void MobFactory::spawn(Level& level, float x, float y)
{
    MobSprites sprites;

    /* TYPE MAPPING:
     * 0: Spawner
     */

    switch (monster_id)
    {
    case 0:
        sprites.mobSprite = CL_Sprite(gc);
        sprites.walkSprite = CL_Sprite(gc, "sprites/spawner", &resources);
        sprites.damageSprite = CL_Sprite(gc, "sprites/spawner_damage", &resources);
        sprites.deadSprite = CL_Sprite(gc, "sprites/deadmob", &resources);
        level.addMob(new BasicMob(x, y, 50, 1, sprites, level, 0.15f));
        break;
    default:
        break;
    }

}
