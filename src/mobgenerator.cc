#include "mobgenerator.hh"
#include "mobfactory.hh"
#include "mobsprites.hh"
#include "mob.hh"

MobGenerator::MobGenerator(int number, std::string& type, int entrance, int mob_interval, int difficulty) 
	: number(number), type(type), difficulty(difficulty), entrance(entrance), mob_interval(mob_interval), tick(0) {	}

void MobGenerator::createMobs(Level& level, CL_GraphicContext& gc, CL_ResourceManager& resources)
{
    tick ++;

    // This makes sure all the mobs aren't piled up when created.
    if ((tick % 12) != 0)
        return;

    // Getting the entrance tile's mapwise coordinates.
    std::pair<size_t, size_t> entrance_tile = level.getEntrances()[entrance];
    
    // Transforming into float point (world) coordinates. Adding TILE_SIZE/2 to get the mob into the middle of the tile.
    float x_coord = (entrance_tile.second * TILE_SIZE) + (TILE_SIZE/2);
    float y_coord = (entrance_tile.first * TILE_SIZE) + (TILE_SIZE/2);

    MobSprites sprites;

    /* TYPE MAPPING:
     * 1: peasant; hp=100, speed=0.1f
     * 2: spawner; hp=70, speed=0.1f
     * 3: bomber; hp=150, speed=0.25f
     * 4: steamtank; hp=500, speed=0.15f
     * 5: mammothtank; hp=1000, speed=0.075f
     * 6: airship; hp=1200, speed=0.12f
     *
     * Points received for killing a mob is the mob's maxhitpoints. Money received is the maxhitpoints / 2.
     */

    std::map<std::string, int> type_map;
    type_map["peasant"] = 1;
    type_map["spawner"] = 2;
    type_map["bomber"] = 3;
    type_map["steamtank"] = 4;
    type_map["mammothtank"] = 5;
    type_map["airship"] = 6;

    switch (type_map[type])
    {
        case 1:
            sprites.mobSprite = CL_Sprite(gc);
            sprites.walkSprite = CL_Sprite(gc, "sprites/peasant", &resources);
            sprites.damageSprite = CL_Sprite(gc, "sprites/peasant_damage", &resources);
            sprites.deadSprite = CL_Sprite(gc, "sprites/deadmob", &resources);
            level.addMob(new BasicMob(x_coord, y_coord, 100 + (0.5*difficulty), difficulty, sprites, level, 0.1f));
            break;
        case 2:
            sprites.mobSprite = CL_Sprite(gc);
            sprites.walkSprite = CL_Sprite(gc, "sprites/spawner", &resources);
            sprites.damageSprite = CL_Sprite(gc, "sprites/spawner_damage", &resources);
            sprites.deadSprite = CL_Sprite(gc, "sprites/deadmob", &resources);
            level.addMob(new SpawnerMob(x_coord, y_coord, 70 + (0.5*difficulty), difficulty, sprites, level, 0.1f, MobFactory(0, gc, resources)));
            break;
        case 3:
            sprites.mobSprite = CL_Sprite(gc);
            sprites.walkSprite = CL_Sprite(gc, "sprites/bomber", &resources);
            sprites.damageSprite = CL_Sprite(gc, "sprites/bomber_damage", &resources);
            sprites.deadSprite = CL_Sprite(gc, "sprites/deadmob", &resources);
            level.addMob(new BasicMob(x_coord, y_coord, 150 + (0.5*difficulty), difficulty, sprites, level, 0.25f));
            break;
        case 4:
            sprites.mobSprite = CL_Sprite(gc);
            sprites.walkSprite = CL_Sprite(gc, "sprites/steamtank", &resources);
            sprites.damageSprite = CL_Sprite(gc, "sprites/steamtank_damage", &resources);
            sprites.deadSprite = CL_Sprite(gc, "sprites/deadmob_mech", &resources);
            level.addMob(new BasicMob(x_coord, y_coord, 500 + (0.5*difficulty), difficulty, sprites, level, 0.15f));
            break;
        case 5:
            sprites.mobSprite = CL_Sprite(gc);
            sprites.walkSprite = CL_Sprite(gc, "sprites/mammothtank", &resources);
            sprites.damageSprite = CL_Sprite(gc, "sprites/mammothtank_damage", &resources);
            sprites.deadSprite = CL_Sprite(gc, "sprites/deadmob_mech", &resources);
            level.addMob(new BasicMob(x_coord, y_coord, 1000 + (0.5*difficulty), difficulty, sprites, level, 0.075f));
            break;
        case 6:
            sprites.mobSprite = CL_Sprite(gc);
            sprites.walkSprite = CL_Sprite(gc, "sprites/airship", &resources);
            sprites.damageSprite = CL_Sprite(gc, "sprites/airship_damage", &resources);
            sprites.deadSprite = CL_Sprite(gc, "sprites/deadmob_mech", &resources);
            level.addMob(new BasicMob(x_coord, y_coord, 1200 + (0.5*difficulty), difficulty, sprites, level, 0.12f));
            break;
        default:
            break;
    }

    number--;
}

int MobGenerator::getInterval()
{
    return mob_interval;
}

bool MobGenerator::isEmpty()
{
    if (number == 0)
    {
        return true;
    }
    return false;
}

void MobGenerator::setEmpty()
{
    number = 0;
}
