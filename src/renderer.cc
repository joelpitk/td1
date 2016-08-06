#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "renderer.hh"
#include "camera.hh"
#include "tile.hh"
#include "map.hh"
#include "tower.hh"
#include "mob.hh"
#include "projectile.hh"

Renderer::Renderer(CL_GraphicContext& gc, Camera& camera) : gc(gc), camera(camera), drawRanges(false)
{
}

void Renderer::render(CL_Sprite& sprite, const CL_Vec2f& position)
{
    const CL_Vec2f& cameraPosition = camera.getPosition();
    
    // Dont draw the sprite if:
    // The sprite is outside the left border or bottom border.
    if ((position.x + sprite.get_width()) > cameraPosition.x && (position.y + sprite.get_height()) > cameraPosition.y)
    {
        // The sprite is outside the right border or top border.
        if (position.x < (cameraPosition.x + gc.get_width()) && position.y < (cameraPosition.y + gc.get_height()))
            sprite.draw(gc, position.x - cameraPosition.x, position.y - cameraPosition.y);
    }
}

void Renderer::circle(float x, float y, float radius)
{
    const CL_Vec2f& cameraPosition = camera.getPosition();
    CL_Colorf white = CL_Colorf(255.0f, 255.0f, 255.0f, 0.2f);
    CL_Draw::circle(gc, x - cameraPosition.x, y - cameraPosition.y, radius, white);

}


void Renderer::render(std::vector<std::vector<Tile> >& tiles)
{
    for (size_t row = 0; row < tiles.size(); row++)
    {
        for (size_t column = 0; column < tiles[row].size(); column++)
        {
            CL_Sprite& sprite = tiles[row][column].getSprite();
            CL_Vec2f spritePos = CL_Vec2f(static_cast<float>(column*sprite.get_width()), static_cast<float>(row*sprite.get_height()));
            render(sprite, spritePos);
        }
    }
}

void Renderer::render(const std::list<Projectile*>& projectiles)
{
    for (std::list<Projectile*>::const_iterator it = projectiles.begin(); it != projectiles.end(); it++)
    {
        render((*it)->getSprite(), (*it)->getLocation());
    }
}

void Renderer::render(const std::list<Tower*>& towers)
{

    if (drawRanges) 
    {
        for (std::list<Tower*>::const_iterator it = towers.begin(); it != towers.end(); it++)
        {
            circle((0.5f+(*it)->getColumn())*TILE_SIZE, (0.5f+(*it)->getRow())*TILE_SIZE, (*it)->getRadius()*TILE_SIZE);
        }
    }

    for (std::list<Tower*>::const_iterator it = towers.begin(); it != towers.end(); it++)
    {
        render((*it)->getBaseSprite(), (*it)->getLocation());
        render((*it)->getGunSprite(), (*it)->getLocation());
        render((*it)->getUpgradeSprite(), (*it)->getLocation());
    }
}

void Renderer::render(const std::list<Mob*>& mobs)
{
    for (std::list<Mob*>::const_iterator it = mobs.begin(); it != mobs.end(); it++)
    {
        if ((*it)->isVisible())
            render((*it)->getSprite(), (*it)->getLocation());
    }
}

void Renderer::toggleDrawRanges()
{
    drawRanges = !drawRanges;
}
