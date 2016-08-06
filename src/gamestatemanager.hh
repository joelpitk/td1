#ifndef STATEMANAGER_HH
#define STATEMANAGER_HH

#include <stack>

class GameState;
#include <ClanLib/core.h>
// Manages a LIFO-stack of gamestates.
class GameStateManager
{
public:
    //GameStateManager() : lastState(NULL) {}
    ~GameStateManager();

	// Pushes the given state to the stack
	void push(GameState* state);

	/* Pops the topmost state from the stack 
	 * and frees any memory associated with it. */
	void pop();

	/* Pops all states from the stack
	 * and frees any memory associated with them. */
	void popAll();

	/* Calls update on the topmost state on the stack.
	 * Returns false if the stack is empty, true otherwise */
	bool update();

	// Calls draw on the topmost state on the stack.
	void draw();
private:
	std::stack<GameState*> deleteStack;
	std::stack<GameState*> states;
};

#endif
