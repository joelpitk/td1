#include "tower.hh"
#include "projectile.hh"


void Tower::aim(Mob* target)
{
    // Sets the aiming angle of the tower.
    CL_Vec2f aim = target->getLocation()-getLocation();
    aim.normalize();
    angle = CL_Angle::from_degrees(aim.angle(CL_Vec2f(0,1)).to_degrees());

    if(target->getLocation().x > getLocation().x)
    {
        angle = CL_Angle::from_degrees(360 - angle.to_degrees());
    }

    tower_sprites.gunSprite.set_angle(angle);

    // Shoots the projectile against the enemy.
    // + 16 in order to shoot from the middle of the tower
    // *(tower_sprites).gunSprite.get_height() in order to shoot from the tip of the cannon.
    level.addProjectile(new BasicProjectile(CL_Vec2f(getLocation().x + TILE_SIZE/2 + aim.x*(tower_sprites.gunSprite.get_height()/2), getLocation().y + TILE_SIZE/2 + aim.y*(tower_sprites.gunSprite.get_height()/2)), 10, projectile_sprites, target, level));
}

void GunTower::update()
{
    if (isReady())
    {
        Mob* target = getTarget();

        if (target != NULL)
        {
            aim(target);

            // Shoots the projectile against the enemy.
            // + 16 in order to shoot from the middle of the tower
            level.addProjectile(new BasicProjectile(CL_Vec2f(getLocation().x + 16, getLocation().y + 16), 10, projectile_sprites, target, level));

            level.addDamage(0, target, damage);
            cooldown = cooldownLength;
        }
    }
    else
    {
        cooldown--;
    }
}

Mob* GunTower::getTarget()
{
    const std::list<Mob*> mobs = level.getMobs();
    std::list<Mob*>::const_iterator mob_it;

    float minDist = LARGE_VALUE; 
    Mob* target = NULL;

    float ty = row + 0.5f;
    float tx = column + 0.5f;

    for (mob_it = mobs.begin(); mob_it != mobs.end(); mob_it++)
    {
	if ((*mob_it)->isDead())
	    continue;
	float mx = (*mob_it)->getX()/TILE_SIZE;
	float my = (*mob_it)->getY()/TILE_SIZE;
	float dist = (tx-mx)*(tx-mx) + (ty-my)*(ty-my);
	if ( dist < radius*radius )
	{
	    double d = (*mob_it)->getDistanceToExit();
	    if (d < minDist) 
	    {
                minDist = d;
                target = (*mob_it);
	    }
	}
    }
    return target;
}

// Gun tower upgrade path: done.
void GunTower::upgrade()
{
    moneySpent += upgradePrice;

    switch (towerlevel)
    {
    case 1:
        radius = 5.2f;
        damage = 45;
        cooldownLength = 50;
        upgradePrice = 400;
        tower_sprites.upgradeSprite.set_image_data(tower_sprites.lvl_1);
        break;
    case 2:
        radius = 5.7f;
        damage = 65;
        cooldownLength = 45;
        upgradePrice = 700;
        tower_sprites.upgradeSprite.set_image_data(tower_sprites.lvl_2);
        break;
    case 3:
        radius = 6.2f;
        damage = 70;
        cooldownLength = 30;
        upgradePrice = 0;
        tower_sprites.upgradeSprite.set_image_data(tower_sprites.lvl_3);
        break;
    default:
        break;
    }

    towerlevel += 1;
}

void CannonTower::upgrade()
{

    moneySpent += upgradePrice;

    switch (towerlevel)
    {
    case 1:
        radius = 7.2f;
        damage = 60;
        cooldownLength = 42;
        upgradePrice = 800;
        tower_sprites.upgradeSprite.set_image_data(tower_sprites.lvl_1);
        break;
    case 2:
        radius = 7.5f;
        damage = 65;
        cooldownLength = 38;
        upgradePrice = 1200;
        tower_sprites.upgradeSprite.set_image_data(tower_sprites.lvl_2);
        break;
    case 3:
        radius = 8.0f;
        damage = 100;
        cooldownLength = 35;
        upgradePrice = 0;
        tower_sprites.upgradeSprite.set_image_data(tower_sprites.lvl_3);
        break;
    default:
        break;
    }

    towerlevel += 1;
}


void AreaTower::update()
{

    if (isReady())
    {
        const std::list<Mob*> mobs = level.getMobs();
        std::list<Mob*>::const_iterator mob_it;

        float ty = row + 0.5f;
        float tx = column + 0.5f;

        for (mob_it = mobs.begin(); mob_it != mobs.end(); mob_it++)
        {
            if ((*mob_it)->isDead())
                continue;
            float mx = (*mob_it)->getX()/TILE_SIZE;
            float my = (*mob_it)->getY()/TILE_SIZE;
            float dist = (tx-mx)*(tx-mx) + (ty-my)*(ty-my);
            if ( dist < radius*radius )
            {
                // Shoots the projectile against the enemy.
                aim(*mob_it);
                damageEnemy(*mob_it);
            }
        }
        cooldown = cooldownLength;
    }
    else
    {
        cooldown--;
    }
}

void AreaTower::damageEnemy(Mob* m)
{
    m->receiveDamage(damage);
}

Mob* AreaTower::getTarget()
{
    return NULL;
}

void AreaTower::upgrade()
{

    moneySpent += upgradePrice;

    switch (towerlevel)
    {
    case 1:
        radius = 4.5f;
        damage = 8;
        cooldownLength = 13;
        upgradePrice = 550;
        tower_sprites.upgradeSprite.set_image_data(tower_sprites.lvl_1);
        break;
    case 2:
        radius = 4.8f;
        damage = 10;
        cooldownLength = 10;
        upgradePrice = 900;
        tower_sprites.upgradeSprite.set_image_data(tower_sprites.lvl_2);
        break;
    case 3:
        radius = 5.5f;
        damage = 17;
        cooldownLength = 5;
        upgradePrice = 0;
        tower_sprites.upgradeSprite.set_image_data(tower_sprites.lvl_3);
        break;
    default:
        break;
    }

    towerlevel += 1;
}

void IceTower::damageEnemy(Mob* m)
{
    m -> slowdown(2*FPS);

    for (int i = 0; i < 10; i++)
    {
        level.addDamage(i*FPS, m, damage);
    }
}

void IceTower::upgrade()
{

    moneySpent += upgradePrice;

    switch (towerlevel)
    {
    case 1:
        radius = 4.4f;
        damage = 3;
        cooldownLength = 48;
        upgradePrice = 700;
        tower_sprites.upgradeSprite.set_image_data(tower_sprites.lvl_1);
        break;
    case 2:
        radius = 5.1f;
        damage = 5;
        cooldownLength = 38;
        upgradePrice = 1000;
        tower_sprites.upgradeSprite.set_image_data(tower_sprites.lvl_2);
        break;
    case 3:
        radius = 6.0f;
        damage = 12;
        cooldownLength = 20;
        upgradePrice = 0;
        tower_sprites.upgradeSprite.set_image_data(tower_sprites.lvl_3);
        break;
    default:
        break;
    }

    towerlevel += 1;
}
