#include "gamecomponent.hh"
#include "levelselectionstate.hh"

MenuState::MenuState(GameComponent& game)
    : GameState(game), continueGameButton(&guiParent), newGameButton(&guiParent),
      logoutButton(&guiParent), exitButton(&guiParent), welcomeLabel(&guiParent)
{
    CL_Rect viewport = window.get_viewport();
    const int menuTop = viewport.top + 100;
    const int welcomeTop = viewport.top + 80;
    const int welcomeHeight = 20;
    const int buttonWidth = 200;
    const int buttonHeight = 40;
    const int buttonLeft = (viewport.get_width() / 2) - (buttonWidth / 2);
    const int marginTop = 15;

    int buttonTop = menuTop;

    welcomeLabel.set_geometry(CL_Rect(buttonLeft, welcomeTop, buttonLeft + buttonWidth, welcomeTop + welcomeHeight));
    welcomeLabel.set_id_name("menu");
    welcomeLabel.set_text("Welcome, " + game.getPlayer().getName() + "!");

    newGameButton.set_geometry(CL_Rect(buttonLeft, buttonTop, buttonLeft + buttonWidth, buttonTop + buttonHeight));
    newGameButton.set_text("New game");
    newGameButton.func_clicked().set(this, &MenuState::newGameClicked);

    buttonTop += buttonHeight + marginTop;

    logoutButton.set_geometry(CL_Rect(buttonLeft, buttonTop, buttonLeft + buttonWidth, buttonTop + buttonHeight));
    logoutButton.set_text("Logout");
    logoutButton.func_clicked().set(this, &MenuState::logoutClicked);

    buttonTop += buttonHeight + marginTop;

    exitButton.set_geometry(CL_Rect(buttonLeft, buttonTop, buttonLeft + buttonWidth, buttonTop + buttonHeight));
    exitButton.set_text("Exit");
    exitButton.func_clicked().set(this, &MenuState::exitClicked);
}

void MenuState::newGameClicked()
{
    stateManager.push(new LevelSelectionState(game));
}

void MenuState::logoutClicked()
{
    game.logoutPlayer();
    stateManager.pop();
}

void MenuState::exitClicked()
{
    game.logoutPlayer();
    stateManager.popAll();
}

void MenuState::update()
{

}

void MenuState::draw()
{

}
