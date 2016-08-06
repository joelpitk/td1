#ifndef GAMESTATE_HH
#define GAMESTATE_HH

#include <ClanLib/gui.h>

/* There's no need to include the complete types here as these classes
 * are only used for reference types here. */
class GameStateManager;
class CL_DisplayWindow;
class CL_ResourceManager;
class CL_GraphicContext;
class CL_InputDevice;
class GameComponent;

// Base class for all gamestates
class GameState
{
public:
	GameState(GameComponent& game);
	virtual ~GameState() {};

	virtual void update() = 0;
	virtual void draw() = 0;

	void showGUI();
	void hideGUI();

protected:
	GameComponent& game;
	GameStateManager& stateManager;
	CL_DisplayWindow& window;
	CL_ResourceManager& resources;
	CL_GraphicContext& gc;
	CL_InputDevice& keyboard;
	CL_InputDevice& mouse;
	CL_GUIComponent guiParent;
private:
	GameState(const GameState&);
	GameState& operator=(const GameState&);
};

#endif
