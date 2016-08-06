#include "level.hh"
#include "projectile.hh"

// Level constructor, calls the readLevel and checkLevel functions.
Level::Level(const std::string &filename,  CL_GraphicContext& gc, CL_ResourceManager& resources) : gc(gc), resources(resources), until_next_wave(0), until_next_mob(0), score(0)
{

    std::ifstream file;
    file.open(filename.c_str());

    if (file.is_open()) 
    {
        readLevel(file, gc, resources);
    }
    else 
    {
        throw std::ios_base::failure("Couldn't open the filestream to the .lvl file.");
    }

    file.close();
    checkLevel();

    std::pair<size_t, size_t> size = getDimensions();
    width = size.first;
    height = size.second;

    // TODO: Extract to a separate class?
    // TODO: A path for exit
    for (size_t r = 0; r < height; r++) 
    {
        std::vector<size_t> row;
        std::vector<bool> brow;
        std::vector< directions > drow;

        for (size_t c = 0; c < width; c++) 
        {
            row.push_back(LARGE_VALUE);
            brow.push_back(false);
            drow.push_back(NO);
        }

        distances.push_back(row);
        reachable.push_back(brow);
        direction.push_back(drow);
    }

    updatePaths();
    tick = 0;
    money = STARTING_MONEY;
    player_health = STARTING_HEALTH;
    lost = false;
    current_generator.setEmpty();
}

Level::~Level()
{
    for (std::list<Mob*>::iterator it_mob = mobs.begin(); it_mob != mobs.end(); it_mob++)
    {
        delete (*it_mob);
    }
    mobs.clear();

    for (std::list<Mob*>::iterator it_dead_mob = dead_mobs.begin(); it_dead_mob != dead_mobs.end(); it_dead_mob++)
    {
        delete (*it_dead_mob);
    }
    dead_mobs.clear();

    for (std::list<Projectile*>::iterator it_proj = projectiles.begin(); it_proj != projectiles.end(); it_proj++)
    {
        delete (*it_proj);
    }
    projectiles.clear();

    for (std::list<Tower*>::iterator it_tower = towers.begin(); it_tower != towers.end(); it_tower++)
    {
        delete (*it_tower);
    }
    towers.clear();

    while (!damage_queue.empty())
    {
        damage_queue.pop();
    }
}


// Returns the level name.
std::string Level::getName() 
{
    return level_name;
}

// Returns the level number.
int Level::getNumber() 
{
    return level_number;
}

// Returns the level map.
std::vector<std::vector<Tile> >& Level::getTiles()
{
    return map.getTiles();
}

const std::list<Mob*>& Level::getMobs() const
{
    return mobs;
}

const std::list<Mob*>& Level::getDeadMobs() const
{
    return dead_mobs;
}

const std::list<Tower*>& Level::getTowers() const
{
    return towers;
}

// Returns the wave vector.
const std::list<Wave>& Level::getWaves() const
{
    return waves;
}

const int Level::getWavesSize() const
{
    return waves.size();
}

const std::list<Projectile*>& Level::getProjectiles() const
{
    return projectiles;
}

const int Level::getWaveInterval() const
{
    return until_next_wave / FPS;
}

bool isDestroyed(Projectile* projectile)
{
    return projectile->isDestroyed();
}

// Returns the level dimensions as x, y (size_t) coordinate pair.
const std::pair<size_t, size_t> Level::getDimensions() 
{
    return map.getDimensions();
}

// This is used to get time through the level class. Returns one tick, tick is updated 60 times each second.
size_t Level::getTick()
{
    return tick;
}

void Level::addMob(Mob* mob)
{
    mobs.push_back(mob);
}

// Implements the adding of a projectile. Remove is handled with update.
void Level::addProjectile(Projectile* projectile)
{
    projectiles.push_back(projectile);
}

void Level::readLevel(std::ifstream &file, CL_GraphicContext& gc, CL_ResourceManager& resources)
{

    std::string first_line;
    getline(file, first_line);
    trim(first_line);

    // Checking the first line to confirm that the file is of .map type.
    if (first_line != "!LEVEL") 
    {
        throw std::invalid_argument("First line in .lvl files should always be: \"!LEVEL\"");
    }

    // Starting the actual reading of the file
    while (!file.eof()) 
    {

        std::string current_line;
        getline(file, current_line);
        trim(current_line);

        if (current_line.empty()) continue;

        // If the line wasn't empty preparing to store block data into KeyvalueMap and sub block data into KeyvalueMap vector.
        KeyvalueMap values;
        std::vector<KeyvalueMap> mobs;

        // Reads and stores the level information.
        if (current_line.find(".level") != std::string::npos) 
        {
            values = readSection(file, mobs);
            level_name = values["name"];

            // The level number needs to be convereted into a string stream to be put in place correctly.
            level_number = boost::lexical_cast<int>(values["number"]);

            continue;
        }

        // Reads and stores the level map.
        else if (current_line.find(".map") != std::string::npos) 
        {
            map = Map(file, gc, resources);
        }

        // Reads and stores the individual waves for the level.
        else if (current_line.find(".wave") != std::string::npos) 
        {
            values = readSection(file, mobs);
            Wave new_wave = Wave(values, mobs);
            waves.push_back(new_wave);
        }
    }
}

/* Check to confirm that nothing was left out of the level during the read. */
void Level::checkLevel() 
{

    if (level_name.length() == 0 || !level_number || waves.size() == 0) 
    {
        throw std::invalid_argument("Level information missing after read. Possible cause, unexpected end of file.");
    }
}

/* Reads an individual section from { to }. */
KeyvalueMap readSection(std::ifstream &file, std::vector<KeyvalueMap>& mobs) 
{

    KeyvalueMap values;

    while (!file.eof())
    {

        std::string current_line;
        getline(file, current_line);
        trim(current_line);

        // Skipping empty lines, if a closing bracket is found break the loop.
        if (current_line.empty()) continue; 
        if (current_line.find("}") != std::string::npos) break;

        // The only allowed nested blocks are of type .mob, checking for those.
        if (current_line.find(".mob") != std::string::npos) 
        {
            mobs.push_back(readSection(file, mobs));
        }

        // Parsing the key value pairs out of key=value syntax.
        std::string key, val;
        key = current_line.substr(0, current_line.find("="));
        val = current_line.substr(current_line.find("=") + 1);
        trim(key); trim(val);
        values[key] = val;
    }

    return values;
}

const std::vector<std::pair<size_t, size_t> >& Level::getEntrances() const
{
    return map.getEntrances();
}

const std::pair<size_t, size_t> Level::getExit() const
{
    return map.getExits().at(0);
}

bool Level::getLost() const
{
    return lost;
}

bool Level::hasWon() const
{
    return waves.empty() && mobs.empty() && generators.empty();
}

int Level::getPlayerHealth() const
{
    return player_health;
}

bool Level::updatePaths()
{
    std::pair<size_t, size_t> exit = getExit();
    std::list<std::pair<size_t,size_t> > queue;

    for (size_t r = 0; r < height; r++)
    {
        for (size_t c = 0; c < width; c++)
        {
            distances[r][c] = LARGE_VALUE;
            reachable[r][c] = false;
        }
    }

    queue.push_back(std::make_pair(exit.first,exit.second));

    while (!queue.empty())
    {
        std::pair<size_t,size_t> cell = queue.front();
        size_t r = cell.first;
        size_t c = cell.second;
        queue.pop_front();

        if (map(r,c).isExit())
        {
            distances[r][c] = 0;
            reachable[r][c] = true;

            if (r>0 && map(r-1,c).isWalkable())
            {
                queue.push_back(std::make_pair(r-1,c));
                reachable[r-1][c] = true;
            }
            if (r+1 < width && map(r+1,c).isWalkable())
            {
                queue.push_back(std::make_pair(r+1,c));
                reachable[r+1][c] = true;
            }
            if (c > 0 && map(r,c-1).isWalkable())
            {
                queue.push_back(std::make_pair(r,c-1));
                reachable[r][c-1] = true;
            }
            if (c+1 < height && map(r,c+1).isWalkable())
            {
                queue.push_back(std::make_pair(r,c+1));
                reachable[r][c+1] = true;
            }
            continue;
        }

        size_t dist = distances[r][c];
        if (r > 0 && reachable[r-1][c])
        {
            if (dist >= (distances[r-1][c] + NORM_COST))
            {
                dist = (distances[r-1][c] + NORM_COST);
                direction[r][c] = UP;
            }
        }
        if (r+1 < height && reachable[r+1][c])
        {
            if (dist >= (distances[r+1][c] + NORM_COST))
            {
                dist = (distances[r+1][c] + NORM_COST);
                direction[r][c] = DOWN;
            }
        }
        if (c > 0 && reachable[r][c-1])
        {
            if (dist >= (distances[r][c-1] + NORM_COST))
            {
                dist = (distances[r][c-1] + NORM_COST);
                direction[r][c] = LEFT;
            }
        }
        if (c+1 < width && reachable[r][c+1])
        {
            if (dist >= (distances[r][c+1] + NORM_COST))
            {
                dist = (distances[r][c+1] + NORM_COST);
                direction[r][c] = RIGHT;
            }
        }

        distances[r][c] = dist;

        // Add neighbours to the queue
        if (r > 0 && !reachable[r-1][c] && map(r-1,c).isWalkable())
        {
            queue.push_back(std::make_pair(r-1,c));
            reachable[r-1][c] = true;
        }
        if (r+1 < height && !reachable[r+1][c] && map(r+1,c).isWalkable())
        {
            queue.push_back(std::make_pair(r+1,c));
            reachable[r+1][c] = true;
        }
        if (c > 0 && !reachable[r][c-1] && map(r,c-1).isWalkable())
        {
            queue.push_back(std::make_pair(r,c-1));
            reachable[r][c-1] = true;
        }
        if (c+1 < width && !reachable[r][c+1] && map(r,c+1).isWalkable())
        {
            queue.push_back(std::make_pair(r,c+1));
            reachable[r][c+1] = true;
        }

    }

    if (DEBUG)
    {
        std::cerr << "---" << std::endl;
        for (size_t r = 0; r < height; r++) {
            for (size_t c = 0; c < width; c++) {
                directions d = direction[r][c];
                if (d == UP) std::cerr << "^";
                if (d == DOWN) std::cerr << "v";
                if (d == LEFT) std::cerr << "<";
                if (d == RIGHT) std::cerr << ">";
                if (d == NO) std::cerr << "O";
            }
            std::cerr << std::endl;
        }
        std::cerr << "---" << std::endl;
    }
    

    // Check that all entrances are reachable
    const std::vector<std::pair<size_t, size_t> >& entrances = getEntrances();
    std::vector<std::pair<size_t, size_t> >::const_iterator it;
    bool allReachable = true;
    for (it = entrances.begin(); it != entrances.end(); it++) {
        if (!reachable[(*it).first][(*it).second])
        {
            allReachable = false;
        }
    }
    return allReachable;
}

size_t Level::getDistance(size_t r, size_t c)
{
    if (r < height && c < width)
        return distances[r][c];
    else
        return LARGE_VALUE;
}

directions Level::getDirection(size_t r, size_t c)
{
    if (r < height && c < width)
        return direction[r][c];
    else
        return NO;
}

directions Level::getDirection(float x, float y)
{
    std::pair<size_t,size_t> coords = getCoords(x,y);
    size_t r = coords.first;
    size_t c = coords.second;
    if (r < height && c < width)
    {
        return direction[r][c];
    }
    else
    {
        throw std::out_of_range("Tried to access coordinates not on map");
    }
}

void Level::update()
{
    tick++;

    updateWaves();

    while ( (!damage_queue.empty()) && damage_queue.top().first <= tick) 
    {
        std::pair<Mob*, int> e = damage_queue.top().second;
        Mob* m = e.first;
        int damage = e.second;

        m->receiveDamage(damage);

        damage_queue.pop();
    }

    updateTowers();
    updateMobs();
    updateProjectiles();

    if (player_health <= 0)
    {
        lost = true;
    }
}

void Level::updateWaves()
{
    // Fetch the next wave if all has been read from the current_generator, there are no more generators
    // there are more waves and the time is full.
    if (current_generator.isEmpty() && generators.empty() && mobs.empty() && !waves.empty() && until_next_wave <= 0)
    {
        while (!damage_queue.empty())
        {
            damage_queue.pop();
        }

        generators = waves.front().getMobs();
        current_generator = generators.front();
        until_next_wave = waves.front().getInterval() * FPS;
        waves.pop_front();
        generators.pop_front();
    }
    // If we're out of mobs in the current generator we need to do something about it.
    else if (current_generator.isEmpty() && !generators.empty())
    {
        // If the generators (of the wave) are NOT empty + until next mob is <= 0
        // pop another generator and keep at it.
        if (until_next_mob <= 0)
        {
            current_generator = generators.front();
            until_next_mob = generators.front().getInterval() * FPS;
            generators.pop_front();
        }
        // Time isn't full for next mob, pass the time.
        else
        {
            until_next_mob--;
        }
    }
    else if (!current_generator.isEmpty() && until_next_mob <= 0)
    {
        current_generator.createMobs(*this, gc, resources);
    }
    else
    {
        until_next_mob--;
    }

    // If there are no more mobs on the field or in the making, start counting down until next wave.
    if (generators.empty() && mobs.empty() && current_generator.isEmpty())
    {
        until_next_wave--;
    }
}

void Level::updateProjectiles()
{
    std::list<Projectile*>::iterator pro_it;
    for (pro_it = projectiles.begin(); pro_it != projectiles.end(); pro_it++)
    {
        (*pro_it) -> update();

        if ((*pro_it)->isDestroyed())
        {
            pro_it = projectiles.erase(pro_it);
        }
    }
}

void Level::updateMobs()
{
    // Updating the mobs and checking whether they're dead yet.
    std::list<Mob*>::iterator it;
    for (it = mobs.begin(); it != mobs.end(); it++)
    {
        (*it)->update();

        if ((*it)->isDead())
        {
            dead_mobs.push_back(*it);
            it = mobs.erase(it);
        }
    }

    // Check if enemies have reached their goal.
    std::pair<size_t, size_t> exit = getExit();
    std::list<Mob*> exitmobs = map(exit.first, exit.second).getMobs();

    for (std::list<Mob*>::iterator mob_it = exitmobs.begin(); mob_it != exitmobs.end(); mob_it++)
    {
        // The mob is at exit tile and is not dead.
        if (!(*mob_it)->isDead())
        {
            // Set the mob dead (projectiles start ignoring it). Remove it from the render list (mobs).
            // Do damage to the player.
            (*mob_it)->setDead(true);
            (*mob_it)->setVisible(false);
            // We could remove it from the mobs, but there can be at
            // most 20 of them and being dead it's already ignored practically everywhere.
            player_health--;

            if (DEBUG)
                std::cerr << "Arr, it hurts! Health: " << player_health << std::endl;
        }
    }

    // Clean up the bodies
    while (dead_mobs.size() > 100 && damage_queue.empty())
    {
        delete dead_mobs.front();
        dead_mobs.pop_front();
    }
}

void Level::updateTowers()
{
    std::list<Tower*>::iterator it;
    for (it = towers.begin(); it != towers.end(); it++)
    {
        (*it)->update();
    }
}

void Level::addDamage(size_t delta, Mob* m, int damage)
{
    damage_queue.push( std::make_pair(tick+delta, std::make_pair(m, damage)));
}

void Level::addTower(Tower* t)
{
    towers.push_back(t);
    Tile& tile = getTiles()[t->getRow()][t->getColumn()];
    tile.setTower(t);
}

void Level::removeTower(Tower* t)
{
    Tile& tile = getTiles()[t->getRow()][t->getColumn()];
    tile.removeTower();
    towers.remove(t);
    delete t;
}

bool Level::changeMoney(int delta)
{
    if (money + delta >= 0)
    {
        money += delta;
        return true;
    }
    return false;
}

int Level::getMoney() const
{
    return money;
}

void Level::addScore(int s)
{
    score += s;
}

int Level::getScore() const
{
    return score;
}

void Level::killAll()
{
    if (DEBUG)
    {
        std::cerr << "Killing all" << std::endl;
        for (std::list<Mob*>::iterator it_mob = mobs.begin(); it_mob != mobs.end(); it_mob++)
        {
            (*it_mob)->receiveDamage(10000);
        }
    }
}
