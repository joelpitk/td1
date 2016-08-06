#include <ClanLib/display.h>
#include "gamestate.hh"
#include "gamecomponent.hh"

 GameState::GameState(GameComponent& game)
	 : game(game), stateManager(game.getStateManager()), window(game.getWindow()), resources(game.getResourceManager()), gc(game.getGraphicContext()),
	   keyboard(game.getKeyboard()), mouse(game.getMouse()), guiParent(&game, CL_GUITopLevelDescription(window.get_viewport(), false))
{
}

 void GameState::showGUI()
 {
     guiParent.set_visible(true);
 }

 void GameState::hideGUI()
 {
     guiParent.set_visible(false);
 }
