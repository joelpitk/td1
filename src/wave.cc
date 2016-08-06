#include "wave.hh"

Wave::Wave(KeyvalueMap& values, std::vector<KeyvalueMap>& mobvec) 
{
    try 
    {
        interval = boost::lexical_cast<int>(values["wave_interval"]);

        for (size_t i = 0; i < mobvec.size(); i++) 
        {

            int number, entrance, level, mob_interval;

            // Casting the values from string to int's and double. I know using atoi and atof isnt perfect, but it works.
            number = boost::lexical_cast<int>(mobvec[i]["number"]);
            entrance = boost::lexical_cast<int>(mobvec[i]["entrance"]);
            level = boost::lexical_cast<int>(mobvec[i]["level"]);
            mob_interval = boost::lexical_cast<int>(mobvec[i]["enemy_interval"]);

            mobs.push_back(MobGenerator(number, mobvec[i]["type"], entrance, mob_interval, level));
        }
    }   
    catch (const boost::bad_lexical_cast& ex) 
    {
        throw std::invalid_argument("Wave value format error. Check the .lvl file.");
    }
}

const int Wave::getInterval()
{
    return interval;
}

const std::list<MobGenerator>& Wave::getMobs() const
{
    return mobs;
}
