#ifndef LOGINSTATE_HH
#define LOGINSTATE_HH

#include <ClanLib/display.h>
#include <ClanLib/gui.h>
#include "gamestate.hh"
#include "messagebox.hh"
#include "player.hh"

class LoginState : public GameState
{
public:
    LoginState(GameComponent& game);

    void loginClicked();
    void exitClicked();

    void update() {}
    void draw() {}

private:
    CL_Label userLabel;
    CL_Label passwordLabel;
    CL_LineEdit userEdit;
    CL_LineEdit passwordEdit;
    CL_CheckBox createPlayerCheck;
    CL_PushButton loginButton;
    CL_PushButton exitButton;
    MessageBox messageBox;
};

#endif
