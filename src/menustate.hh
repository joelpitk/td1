#ifndef MENUSTATE_HH
#define MENUSTATE_HH

#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include "gamestate.hh"

class MenuState : public GameState
{
public:
    MenuState(GameComponent& game);

    void newGameClicked();
    void logoutClicked();
    void exitClicked();

    void draw();
    void update();

private:
    CL_Slot onKeyUpSlot;
    CL_PushButton continueGameButton;
    CL_PushButton newGameButton;
    CL_PushButton logoutButton;
    CL_PushButton exitButton;
    CL_Label welcomeLabel;
};

#endif
