#ifndef GAMEPLAYSTATE_HH
#define GAMEPLAYSTATE_HH

#include <ClanLib/gui.h>
#include "gamestate.hh"
#include "camera.hh"
#include "renderer.hh"
#include "level.hh"
#include "tower.hh"
#include "mob.hh"
#include "messagebox.hh"
#include "towerstats.hh"

class GameplayState : public GameState
{
public:
    GameplayState(GameComponent& game, const std::string levelFile);
    //~GameplayState();

    void createGUI();

    void update();
    void draw();
    void mouseMoved(const CL_InputEvent& evt, const CL_InputState& state);
    void mouseButtonUp(const CL_InputEvent& evt, const CL_InputState& state);
    void keyUp(const CL_InputEvent& evt, const CL_InputState& state);
    CL_Vec2f mousePositionToWorldPosition(const CL_Point& mousePosition) const;
    void buildTowerClicked(int towerType);
    void buildTower(size_t row, size_t column);
    void exitState();
private:
    Camera camera;
    Renderer renderer;
    Level level;
    CL_Font_System font;
    CL_SlotContainer slots;
    CL_Point lastMousePosition;
    CL_PushButton towerButton1;
    CL_PushButton towerButton2;
    CL_PushButton towerButton3;
    CL_PushButton towerButton4;
    CL_Rect bottomBar;
    CL_Rect healthBar;
    CL_Sprite tower1Sprite;
    CL_Sprite tower2Sprite;
    CL_Sprite tower3Sprite;
    CL_Sprite tower4Sprite;
    TowerStats towerStats;
    MessageBox youWinLoseBox;
    int buildTowerType;
};

#endif
