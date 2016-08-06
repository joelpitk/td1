#ifndef RENDERER_HH
#define RENDERER_HH

#include <vector>
#include <list>

class CL_GraphicContext;
class Camera;
class Tile;
class Mob;
class Tower;
class Projectile;

class Renderer
{
public:
    Renderer(CL_GraphicContext& gc, Camera& camera);
	
    /* Draws the given sprite relative to camera coordinates */
    void render(CL_Sprite& sprite, const CL_Vec2f& position);

    void render(std::vector<std::vector<CL_Sprite> >& tiles);

    /* Draws a 2D vector of tiles (tilemap) */
    void render(std::vector<std::vector<Tile> >& tiles);

    void render(const std::list<Projectile*>& projectiles);

    void render(const std::list<Tower*>& towers);
    void render(const std::list<Mob*>& mobs);

    void circle(float x, float y, float radius);
    void toggleDrawRanges();

private:

    CL_GraphicContext& gc;
    Camera& camera;
	
    bool drawRanges; 
};

#endif
