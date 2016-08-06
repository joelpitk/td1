#ifndef TOWER_HH
#define TOWER_HH

#include "constants.hh"

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "mob.hh"
#include "projectilesprites.hh"
#include "towersprites.hh"

class Level;
class Mob;
class Tower
{
public:
    
    Tower(Level& level, size_t row, size_t column, TowerSprites tower_sprites, ProjectileSprites projectile_sprites) : level(level), row(row), column(column), sprite(sprite), tower_sprites(tower_sprites), projectile_sprites(projectile_sprites), angle(CL_Angle::from_degrees(0)), cooldown(0), moneySpent(0), towerlevel(1)
    {   }

    virtual void aim(Mob* target);

    virtual void update() = 0;
    virtual Mob* getTarget() = 0;

    virtual void upgrade() {};

    int getSellPrice() const 
    {
        return moneySpent/3;
    }

    int getUpgradePrice() const
    {
        return upgradePrice;
    }

    int getDamage() const
    {
        return damage;
    }

    virtual bool isReady() const {
        return cooldown == 0;
    }
    
    float getRadius() const
    {
        return radius;
    }

    CL_Vec2f getLocation()
    {
        return CL_Vec2f(static_cast<float>(TILE_SIZE * column), static_cast<float>(TILE_SIZE * row));
    }

    size_t getRow() const
    {
        return row;
    }

    size_t getColumn() const
    {
        return column;
    }

    CL_Sprite& getBaseSprite()
    {
        return tower_sprites.baseSprite;
    }

    CL_Sprite& getGunSprite()
    {
        return tower_sprites.gunSprite;
    }

    CL_Sprite& getUpgradeSprite()
    {
        return tower_sprites.upgradeSprite;
    }

    float getRadius()
    {
	return radius;
    }
    
protected:
    Level& level;
    size_t row;
    size_t column;
    CL_Sprite sprite;
    TowerSprites tower_sprites;
    ProjectileSprites projectile_sprites;
    CL_Angle angle;

    int damage;
    int cooldown;
    int cooldownLength;
    int upgradePrice;
    int moneySpent;
    int towerlevel;
    float radius;
};

class GunTower : public Tower
{
public:

    GunTower(Level& level, size_t row, size_t column, TowerSprites tower_sprites, ProjectileSprites projectile_sprites) : Tower(level, row, column, tower_sprites, projectile_sprites)
    {
        radius = 5.0f;
        damage = 30;
        cooldownLength = 57;
        upgradePrice = 300;
        moneySpent += price;
    }

    static const int price = 150;

    virtual void update();
    virtual Mob* getTarget();    
    virtual void upgrade();
    
private:

};

class CannonTower : public GunTower
{
public:
    CannonTower(Level& level, size_t row, size_t column, TowerSprites tower_sprites, ProjectileSprites projectile_sprites) : GunTower(level, row, column, tower_sprites, projectile_sprites)
    {
        radius = 6.7f;
        damage = 50;
        cooldownLength = 50;
        upgradePrice = 600;
        moneySpent += price;
    }

    virtual void upgrade();
    
    static const int price = 300;

private:
};


class AreaTower : public Tower
{

public:
    AreaTower(Level& level, size_t row, size_t column, TowerSprites tower_sprites, ProjectileSprites projectile_sprites) : Tower(level, row, column, tower_sprites, projectile_sprites)
    {
        radius = 4.0f;
        damage = 5;
        cooldownLength = 10;
        upgradePrice = 400;
        moneySpent += price;
    }

    static const int price = 200;
	
    virtual void update();
    virtual Mob* getTarget();
    virtual void damageEnemy(Mob* m);
    
    virtual void upgrade();

private:

};


class IceTower : public AreaTower
{

public:
    IceTower(Level& level, size_t row, size_t column, TowerSprites towerSprites, ProjectileSprites projectileSprites) : AreaTower(level, row, column, towerSprites, projectileSprites)
    {
        radius = 4.0f;
        damage = 2;
        cooldownLength = 55;
        upgradePrice = 350;
        moneySpent += price;
    }

    static const int price = 250;

    virtual void damageEnemy(Mob* m);
    virtual void upgrade();

private:
    
};


#endif
