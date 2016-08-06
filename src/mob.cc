#include "mob.hh"

// Virtual destructor for the base class.
Mob::~Mob()
{
    // When the mob dies and destructor is called we need to remove the pointer to it from the current tile it's in.
    level.getTiles()[tile_row][tile_column].removeMob(this);
}

size_t Mob::getDistanceToExit() const
{
    // TODO: Improve: add approximate distance to the edge of the tile
    return level.getDistance(tile_row, tile_column);
}

// Returns the tilewise location of the mob in a x, y pair.
std::pair<size_t, size_t> Mob::getTileCoordinates()
{
    return std::pair<size_t, size_t> (static_cast<size_t>(x / TILE_SIZE), static_cast<size_t>(y / TILE_SIZE));
}

void Mob::slowdown(size_t time)
{
    slowdownTime = time;
}

BasicMob::BasicMob(float x, float y, int hitPoints, int difficulty, MobSprites sprites, Level& level, float speed) : Mob(x, y, hitPoints, sprites, level, speed, difficulty)
{   }

void BasicMob::update()
{
    if (dead)
	return;

    size_t old_column = tile_column; 
    size_t old_row = tile_row; 

    float slowdown = (slowdownTime > 0) ? 0.5 : 1;
    float s = slowdown * DT * speed;
    sprites.mobSprite.update(4);

    // Walk to the middle of the tile
    directions new_direction = level.getDirection(x,y);
    if ((direction == UP || direction == DOWN) && (new_direction == LEFT || new_direction == RIGHT))
    {
        float ypos = (y - TILE_SIZE*tile_row) / TILE_SIZE;
        if (direction == UP && ypos < 0.5)
            direction = new_direction;
        if (direction == DOWN && ypos > 0.5)
            direction = new_direction;
    }
    else if ((direction == LEFT || direction == RIGHT) && (new_direction == UP || new_direction == DOWN))
    {
        float xpos = (x - TILE_SIZE*tile_column) / TILE_SIZE;
        if (direction == LEFT && xpos < 0.5)
            direction = new_direction;
        if (direction == RIGHT && xpos > 0.5)
            direction = new_direction;
    }
    else
    {
        // In case the right direction changes horizontally or vertically
        direction = new_direction;
    }
        
    
    // Handles the rotation to the right direction.
    // The weird angles are because the mobs are drawn
    switch (direction)
    {
    case UP:
        angle = CL_Angle::from_degrees(180);
        y -= s;
        break;
    case DOWN:
        angle = CL_Angle::from_degrees(0);
        y += s;
        break;
    case LEFT:
        angle = CL_Angle::from_degrees(90);
        x -= s;
        break;
    case RIGHT:
        angle = CL_Angle::from_degrees(270);
        x += s;
    default:
        break;
    }

    // This part handles the smooth rotation for the mobs.
    CL_Angle angle_temp;
    angle_temp = sprites.mobSprite.get_angle().normalize_180();
    angle -= angle_temp;
    // The magic number in the divisor determines the speed of the rotation.
    angle.set_degrees( angle.normalize_180().to_degrees() / 20 );
    sprites.mobSprite.rotate(angle);

    tile_row = getTileCoordinates().second;
    tile_column = getTileCoordinates().first;

    if (tile_column != old_column || tile_row != old_row) 
    {
        level.getTiles()[old_row][old_column].removeMob(this);
        level.getTiles()[tile_row][tile_column].addMob(this);
    }

    if (slowdownTime > 0)
        slowdownTime--;
}

void BasicMob::receiveDamage(int damage)
{
    if (dead)
        return;

    if (animationFinished)
    {
        sprites.mobSprite.set_image_data(sprites.damageSprite);
        animationFinished = false;
    }

    hitPoints -= damage;

    if (hitPoints <= 0)
    {
        dead = true;
        sprites.mobSprite.set_image_data(sprites.deadSprite);
        level.changeMoney(maxHitPoints/4);
        level.addScore(maxHitPoints);
    }
}

SpawnerMob::SpawnerMob(float x, float y, int hitPoints, int difficulty, MobSprites sprites, Level& level, float speed, MobFactory mf) : BasicMob(x, y, hitPoints, difficulty, sprites, level, speed), mf(mf), cooldown(100)
{   }

void SpawnerMob::update()
{
    if (dead)
        return;
    
    BasicMob::update();

    cooldown--;
    if (cooldown == 0)
    {
        mf.spawn(level,x,y);
        cooldown = 100;
    }
}
