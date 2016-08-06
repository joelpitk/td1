#include "gamestatemanager.hh"
#include "gamestate.hh"

GameStateManager::~GameStateManager()
{
    popAll();
    while (!deleteStack.empty())
    {
        delete deleteStack.top();
        deleteStack.pop();
    }
}

void GameStateManager::push(GameState* state)
{
    if (!states.empty())
    {
        states.top()->hideGUI();
    }

    states.push(state);
}

void GameStateManager::pop()
{
    if (!states.empty())
    {
        deleteStack.push(states.top());
        states.pop();

        if (!states.empty())
            states.top()->showGUI();
    }
}

void GameStateManager::popAll()
{
    while (!states.empty())
        pop();
}

bool GameStateManager::update()
{
    while (!deleteStack.empty())
    {
        delete deleteStack.top();
        deleteStack.pop();
    }

    if (!states.empty())
    {
        states.top()->update();
        return true;
    }
		
    return false;
}

void GameStateManager::draw()
{
    if (!states.empty())
        states.top()->draw();
}
