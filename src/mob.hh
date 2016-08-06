#ifndef MOB_HH
#define MOB_HH

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "level.hh"
#include "mobsprites.hh"
#include "mobfactory.hh"

class Mob
{
public:
    
    Mob(float x, float y, int hitPoints, MobSprites sprites, Level& level, float speed, int difficulty =1)
        : x(x), y(y), hitPoints(hitPoints), maxHitPoints(hitPoints), sprites(sprites), level(level), speed(speed), tile_row(static_cast<size_t>(y / TILE_SIZE)), tile_column(static_cast<size_t>(x / TILE_SIZE)), dead(false), direction(NO), animationFinished(true), difficulty(difficulty), slowdownTime(0), visible(true)
    {
        spriteWidth = sprites.walkSprite.get_width();
        spriteHeight = sprites.walkSprite.get_height();
        // We assume we're going from left to right.
        angle = CL_Angle::from_radians(270);
        sprites.mobSprite.set_image_data(sprites.walkSprite);
        slots.connect(sprites.mobSprite.sig_animation_finished(), this, &Mob::onAnimationFinished);
    }
    
    // Virtual destructor for the base class.
    virtual ~Mob();

    // Update method is called every turn
    virtual void update() = 0;

    // Returns the location where the unit should be drawn
    CL_Vec2f getLocation()
    {
        return CL_Vec2f(x-spriteWidth/2,y-spriteHeight/2);
    }

    CL_Vec2f getCenterLocation()
    {
        return CL_Vec2f(x,y);
    }

    // Returns the (row,column) location of the mob on the grid
    std::pair<size_t, size_t> getTileCoordinates();

    // Returns true if the mob is dead
    bool isDead()
    {
        return dead;
    }

    void setDead(bool d)
    {
        dead = d;
    }

    float getX()
    {
        return x;
    }

    float getY()
    {
        return y;
    }

    CL_Sprite& getSprite()
    {
        return sprites.mobSprite;
    }

    bool isVisible()
    {
        return visible;
    }

    void setVisible(bool b)
    {
        visible = b;
    }

    size_t getDistanceToExit() const;

    virtual void receiveDamage(int damage) = 0;

    void onAnimationFinished()
    {
        sprites.mobSprite.set_image_data(sprites.walkSprite);
        animationFinished = true;
    }

    virtual void slowdown(size_t time);

protected:
    // Float point world coordinates of the mob, starts from an entrance.
    float x;
    float y;

    // Hit points and mob level (= difficulty), told in the file.
    // maxHitPoints stored in order to make the class more generic and determine the amount of money and score
    // received from the mob by its maxhitpoints.
    int hitPoints;
    int maxHitPoints;

    // Sprite information, angle, width, height
    MobSprites sprites;

    // Level reference
    Level& level;

    // Speed, direction of the mob and boolean of dead status.
    float speed;

    // Stores the current tilewise coordinates for the mob.
    size_t tile_row;
    size_t tile_column;

    // Keeps track whether the mob is dead.
    bool dead;
    directions direction;

    bool animationFinished;

    // Mob level.
    int difficulty;

    CL_SlotContainer slots;

    // Sprite information, angle, width, height.
    CL_Angle angle;
    size_t spriteWidth;
    size_t spriteHeight;
    size_t slowdownTime;
    bool visible;
};

class BasicMob : public Mob 
{

public:

    BasicMob(float x, float y, int hitPoints, int difficulty, MobSprites sprites, Level& level, float speed);


    virtual void update();

    virtual void receiveDamage(int damage);
    
private:

};

class SpawnerMob : public BasicMob
{
public:
    SpawnerMob(float x, float y, int hitPoints, int difficulty, MobSprites sprites, Level& level, float speed, MobFactory mf);

    virtual void update();


private:
    MobFactory mf;
    int cooldown;
};

#endif

