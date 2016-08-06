#ifndef TOWERDEFENSEGAME_H
#define TOWERDEFENSEGAME_H

class CL_GUIManager;

/* The main game class. 
 * Creates the render window and pushes the initial state onto the state stack.
 * Contains the main loop of the game. */
class TowerDefenseGame
{
public:
    void run();
    void onWindowClose(CL_GUIManager* guiManager);
private:
    bool running;
};

#endif
