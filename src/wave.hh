#ifndef WAVE_HH
#define WAVE_HH

#include "common.hh"
#include "mobgenerator.hh"

class Wave 
{

public:
    Wave(KeyvalueMap& values, std::vector<KeyvalueMap>& mobvec);
    const int getInterval();
    const std::list<MobGenerator>& getMobs() const;

private:
    std::list<MobGenerator> mobs;
    int interval;
};

#endif
