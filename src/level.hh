#ifndef LEVEL_HH
#define LEVEL_HH

class Mob;
class Tower;
class Wave;
class Level;
class Projectile;

#include "common.hh"
#include "map.hh"
#include "wave.hh"
#include "tower.hh"
#include "mob.hh"
#include "mobgenerator.hh"

#include <queue>
#include <list>

#include <stdexcept>


class timecomparison
{
  bool reverse;

public:
    timecomparison(const bool& revparam=false)
    {
        reverse=revparam;
    }

    bool operator() (const std::pair<size_t, std::pair< Mob*, int > >& lhs, const std::pair<size_t, std::pair< Mob*, int > >& rhs) const
    {
        if (reverse) return (lhs.first < rhs.first);
        else return (lhs.first > rhs.first);
    }
};

class Level {

public:

    Level(const std::string& filename, CL_GraphicContext& gc, CL_ResourceManager& resources);
    ~Level();
    std::string getName();
    int getNumber();
    std::vector<std::vector<Tile> >& getTiles();

    const std::list<Mob*>& getMobs() const;
    const std::list<Mob*>& getDeadMobs() const;
    const std::list<Tower*>& getTowers() const;
    const std::list<Wave>& getWaves() const;
    const int getWavesSize() const;
    const std::list<Projectile*>& getProjectiles() const;
    const int getWaveInterval() const;

    const std::pair<size_t, size_t> getDimensions();

    const std::vector<std::pair<size_t, size_t> >& getEntrances() const;
    const std::pair<size_t, size_t> getExit() const;
    bool getLost() const; // And don't show your face here again!
    bool hasWon() const;
    int getPlayerHealth() const;

    void readLevel(std::ifstream &file,  CL_GraphicContext& gc, CL_ResourceManager& resources);
    void checkLevel();

    size_t minDist(size_t r, size_t c, size_t current);

    /*
     * Returns: Whether the exit is reachable from all entrances
     */
    bool updatePaths();
    void updateDistance(size_t r, size_t c, size_t dist);

    /* Update functions for different individual parts. */
    void updateWaves();
    void updateProjectiles();
    void updateMobs();
    void updateTowers();

    /* Returns the distance to the exit */
    size_t getDistance(size_t r, size_t c);

    /* Returns the direction a mob should go to in a tile at
     * coordinates (r,c). Note the possibility of confusion, as the
     * float variant uses (x,y)-coordinates */
    directions getDirection(size_t r, size_t c);
    /* Returns the direction a mob should go to in a tile at
     * coordinates (x,y). Note the possibility of confusion, as the
     * other variant uses (r,c)-coordinates */
    directions getDirection(float x, float y);

    std::pair<size_t,size_t> getCoords(float x, float y)
    {
        return std::make_pair(y / TILE_SIZE, x / TILE_SIZE);
    }

    /* Returns the current game time */
    size_t getTick();

    void addMob(Mob* mob);
    void addProjectile(Projectile* projectile);

    void update();

    void addDamage(size_t delta, Mob* m, int damage);
    void addTower(Tower* t);
    void removeTower(Tower* t);

    typedef std::pair<size_t, std::pair< Mob*, int > > damage_event_t;

    /*
     * Returns: whether the change is allowed (adding is, subtraction only when it keeps the total nonzero)
     */
    bool changeMoney(int delta);

    /* Returns the players current money */
    int getMoney() const;

    void addScore(int s);
    int getScore() const;

    /* Debug functions */

    void killAll();
    
private:

    std::string level_name;
    int level_number;

    Map map;
    size_t width;
    size_t height;

    std::list<Wave> waves;
    std::list<Mob*> mobs;
    std::list<Mob*> dead_mobs;
    std::list<Tower*> towers;
    std::list<Projectile*> projectiles;

    std::vector< std::vector<size_t> > distances;
    std::vector< std::vector<bool> > reachable;
    std::vector< std::vector< directions  > > direction;

    CL_GraphicContext& gc;
    CL_ResourceManager& resources;

    int until_next_wave;
    int until_next_mob;
    std::list<MobGenerator> generators;
    MobGenerator current_generator;

    std::priority_queue< damage_event_t, std::vector<damage_event_t>, timecomparison > damage_queue;
    size_t tick;

    // Player information
    int money;
    int player_health;
    bool lost;
    int score;
};

KeyvalueMap readSection(std::ifstream &file, std::vector<KeyvalueMap>& mobs);

#endif
