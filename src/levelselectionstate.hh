#ifndef LEVELSELECTIONSTATE_HH
#define LEVELSELECTIONSTATE_HH

#include <ClanLib/gui.h>
#include <vector>
#include <string>
#include "gamestate.hh"
#include "messagebox.hh"
#include "common.hh"

class LevelSelectionState : public GameState
{
public:
    LevelSelectionState(GameComponent& game);

    void createLevelVector();
    void updateInfo();

    void previousClicked();
    void nextClicked();
    void playClicked();
    void backClicked();


    void update();
    void draw() {}
private:
    std::vector<std::string> levels;
    size_t levelIndex;
    bool refresh;
    CL_Label infoLabel;
    CL_PushButton previousButton;
    CL_PushButton nextButton;
    CL_PushButton playButton;
    CL_PushButton backButton;
    MessageBox messageBox;
};

#endif
