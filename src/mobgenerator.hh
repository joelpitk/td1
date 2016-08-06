#ifndef MOBGENERATOR_HH
#define MOBGENERATOR_HH

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "common.hh"
#include "mobfactory.hh"

class Level;

class MobGenerator {

public:

    MobGenerator() {    }
    MobGenerator(int number, std::string& type, int entrance, int mob_interval =0, int difficulty =1);
    void createMobs(Level& level, CL_GraphicContext& gc, CL_ResourceManager& resources);
    int getInterval();
    bool isEmpty();
    void setEmpty();

private:
    int number;
    std::string type;
    int difficulty, entrance, mob_interval, tick;
};

#endif
