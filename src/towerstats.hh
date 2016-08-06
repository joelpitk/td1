#ifndef TOWERSTATS_HH
#define TOWERSTATS_HH

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gui.h>
#include <sstream>

class Tower;
class Level;

class TowerStats
{
public:
    TowerStats(CL_GUIComponent* parent, CL_Size size);
    void show(CL_Point position, Tower* t, Level* l);
    bool hide();

private:
    void upgradeClicked();
    void sellClicked();
    void updateStats();

    CL_Size size;
    CL_Window window;
    CL_PushButton upgradeButton;
    CL_PushButton sellButton;
    CL_Label statsLabel;
    Tower* tower;
    Level* level;
};

#endif
