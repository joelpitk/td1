#ifndef MOBFACTORY_HH
#define MOBFACTORY_HH

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "common.hh"

class Level;

class MobFactory {

public:

    MobFactory(int monster_id, CL_GraphicContext& gc, CL_ResourceManager& resources);
    void spawn(Level& level, float x, float y);

private:
    std::string type;
    int monster_id;
    CL_GraphicContext& gc;
    CL_ResourceManager& resources;
};

#endif
