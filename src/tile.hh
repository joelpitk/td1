#ifndef TILE_HH
#define TILE_HH
#include "common.hh"
#include <ClanLib/display.h>

// Forward declaration to fix the problem with include's
class Mob;
class BasicMob;
class Tower;

class Tile
{

public:

    Tile(char tile, CL_GraphicContext& gc, CL_ResourceManager& resources);

    bool isWalkable() const;
    bool isBuildable() const;
    bool isEntrance() const;
    bool isExit() const;

    void setWalkable(bool walkable);
    void setBuildable(bool buildable);

    CL_Sprite& getSprite();

    void addMob(Mob* mob);
    void removeMob(Mob* mob);
    void clearMobs();
    std::list<Mob*>& getMobs();
 
    void setTower(Tower* t);
    Tower* getTower();
    void removeTower();

private:

    CL_Sprite sprite;
    bool walkable;
    bool buildable; 
    bool entrance;
    bool exit;
    bool containsTower;

    std::list<Mob*> mobs;
    Tower* tower;
};

#endif
