#include "tile.hh"
#include "mob.hh"

Tile::Tile(char tile, CL_GraphicContext& gc, CL_ResourceManager& resources)
{

    walkable = false;
    buildable = false;
    exit = false;
    entrance = false;
    containsTower = false;
    tower = NULL;
    
    switch (tile) {

    case '#':
        sprite = CL_Sprite(gc, "sprites/mountain", &resources);
        walkable = false; buildable = false;
        break;
    case 'E':
        sprite = CL_Sprite(gc, "sprites/exit", &resources);
        walkable = true; buildable = false; exit = true;
        break;
    case '.':
        sprite = CL_Sprite(gc, "sprites/ground", &resources);
        walkable = true; buildable = true;
        break;
    case '*':
        sprite = CL_Sprite(gc, "sprites/rocks", &resources);
        walkable = false; buildable = true;
        break;
        // Keep entrance as default because all numbers from 0-9 are ok.
        // TODO: Possibly a check that the letter is from a number from 0 to 9 else throw something.
    default:
        sprite = CL_Sprite(gc, "sprites/entrance", &resources);
        walkable = true; buildable = false; entrance = true;
        break;
    }
}

bool Tile::isWalkable() const
{
    return walkable && !containsTower;
}

bool Tile::isBuildable() const
{
    return buildable && !containsTower;
}

bool Tile::isEntrance() const
{
    return entrance;
}

bool Tile::isExit() const
{
    return exit;
}

void Tile::setWalkable(bool walkable)
{
    this->walkable = walkable;
}

void Tile::setBuildable(bool buildable)
{
    this->buildable = buildable;
}

CL_Sprite& Tile::getSprite()
{
    return sprite;
}

void Tile::addMob(Mob* mob)
{
    mobs.push_back(mob);
}

void Tile::removeMob(Mob* mob)
{
    mobs.remove(mob);
}

void Tile::clearMobs() {
    mobs.clear();
}

std::list<Mob*>& Tile::getMobs() {
    return mobs;
}

void Tile::setTower(Tower* t)
{
    tower = t;
    containsTower = true;
}

Tower* Tile::getTower()
{
    return tower;
}

void Tile::removeTower()
{
    tower = NULL;
    containsTower = false;
}
