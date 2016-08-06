#include "loginstate.hh"
#include "gamecomponent.hh"

LoginState::LoginState(GameComponent& game)
    : GameState(game), userLabel(&guiParent), passwordLabel(&guiParent),
      userEdit(&guiParent), passwordEdit(&guiParent), createPlayerCheck(&guiParent),
      loginButton(&guiParent), exitButton(&guiParent), messageBox(&guiParent)
{
    CL_Rect viewport = window.get_viewport();
    const int loginTop = viewport.top + 100;
    const int buttonWidth = 200;
    const int buttonHeight = 40;
    const int left = (viewport.get_width() / 2) - (buttonWidth / 2);
    const int marginTop = 20;

    const int lineEditWidth = 200;
    const int lineEditHeight = 20;

    const int labelHeight = 20;
    int labelTop = loginTop;

    int lineEditTop = labelTop + labelHeight;

    userLabel.set_id_name("menu");
    userLabel.set_text("Username:");
    userLabel.set_geometry(CL_Rect(left, labelTop, left + lineEditWidth, labelTop + labelHeight));

    userEdit.set_geometry(CL_Rect(left, lineEditTop, left + lineEditWidth, lineEditTop + lineEditHeight));

    labelTop += labelHeight + lineEditHeight + marginTop;
    passwordLabel.set_id_name("menu");
    passwordLabel.set_text("Password:");
    passwordLabel.set_geometry(CL_Rect(left, labelTop, left + lineEditWidth, labelTop + labelHeight));

    lineEditTop = labelTop + labelHeight;
    passwordEdit.set_geometry(CL_Rect(left, lineEditTop, left + lineEditWidth, lineEditTop + lineEditHeight));
    passwordEdit.set_password_mode(true);

    const int checkTop = lineEditTop + lineEditHeight + 5;
    createPlayerCheck.set_geometry(CL_Rect(left, checkTop, left + lineEditWidth, checkTop + lineEditHeight));
    createPlayerCheck.set_text("Create player");
    createPlayerCheck.set_checked(false);

    int buttonTop = checkTop + lineEditHeight + marginTop;
    loginButton.set_geometry(CL_Rect(left, buttonTop, left + buttonWidth, buttonTop + buttonHeight));
    loginButton.set_text("Login");
    loginButton.func_clicked().set(this, &LoginState::loginClicked);

    buttonTop += buttonHeight + marginTop;
    exitButton.set_geometry(CL_Rect(left, buttonTop, left + buttonWidth, buttonTop + buttonHeight));
    exitButton.set_text("Exit");
    exitButton.func_clicked().set(this, &LoginState::exitClicked);
}

void LoginState::loginClicked()
{
    Player* player = NULL;
    try
    {
        player = new Player(userEdit.get_text(), passwordEdit.get_text(), !createPlayerCheck.is_checked());
        game.loginPlayer(player);
        stateManager.push(new MenuState(game));
    }
    catch (std::exception& e)
    {
        if (player)
            delete player;

        CL_Rect viewport = window.get_viewport();

        const int boxWidth = 300;
        const int boxHeight = 200;

        const int boxLeft = (viewport.get_width() / 2) - (boxWidth / 2);
        const int boxTop =  (viewport.get_height() / 2) - (boxHeight / 2);

        CL_Rect messageBoxRect(boxLeft, boxTop, boxLeft + boxWidth, boxTop + boxHeight);
        messageBox.show(messageBoxRect, "Error", e.what());
    }
}

void LoginState::exitClicked()
{
    stateManager.pop();
}
