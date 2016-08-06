#include <sstream>
#include <stdexcept>
#include "levelselectionstate.hh"
#include "gamecomponent.hh"
#include "gameplaystate.hh"

LevelSelectionState::LevelSelectionState(GameComponent& game)
    : GameState(game), levelIndex(0), refresh(false), infoLabel(&guiParent), previousButton(&guiParent),
      nextButton(&guiParent), playButton(&guiParent), backButton(&guiParent), messageBox(&guiParent)
{
    CL_Rect viewport = window.get_viewport();

    int top = 100;
    const int labelWidth = 200;
    const int labelHeight = 40;
    const int buttonHeight = 40;
    const int buttonWidth = 200;
    const int previousButtonWidth = 100;
    const int nextButtonWidth = previousButtonWidth;
    const int left = (viewport.get_width() / 2) - (buttonWidth / 2);
    const int marginTop = 20;

    CL_Rect labelRect(left, top, left + labelWidth, top + labelHeight);
    infoLabel.set_geometry(labelRect);
    infoLabel.set_id_name("menu");
    infoLabel.set_constant_repaint(true);

    top = labelRect.bottom + marginTop;

    CL_Rect previousButtonRect(left, top, left + previousButtonWidth, top + buttonHeight);
    previousButton.set_geometry(previousButtonRect);
    previousButton.set_text("Previous");
    previousButton.func_clicked().set(this, & LevelSelectionState::previousClicked);

    CL_Rect nextButtonRect(previousButtonRect.right, top, previousButtonRect.right + nextButtonWidth, top + buttonHeight);
    nextButton.set_geometry(nextButtonRect);
    nextButton.set_text("Next");
    nextButton.func_clicked().set(this, & LevelSelectionState::nextClicked);

    top = nextButtonRect.bottom + marginTop;

    CL_Rect playButtonRect(left, top, left + buttonWidth, top + buttonHeight);
    playButton.set_geometry(playButtonRect);
    playButton.set_text("Play");
    playButton.func_clicked().set(this, & LevelSelectionState::playClicked);

    top = playButtonRect.bottom + marginTop;

    CL_Rect backButtonRect(left, top, left + buttonWidth, top + buttonHeight);
    backButton.set_geometry(backButtonRect);
    backButton.set_text("Back");
    backButton.func_clicked().set(this, & LevelSelectionState::backClicked);

    createLevelVector();
    updateInfo();
}

void LevelSelectionState::createLevelVector()
{

    std::ifstream file;
    file.open("data/levels/levels.txt");

    if (file.is_open())
    {
        while (!file.eof())
        {
            std::string current_line;
            getline(file, current_line);
            trim(current_line);

            if (!current_line.empty())
            {
                levels.push_back("data/levels/"+current_line);
            }
        }
    }
    else
    {
        throw std::ios_base::failure("Couldn't open the filestream to the .lvl file.");
    }
}

void LevelSelectionState::updateInfo()
{
    std::stringstream levelInfo;

    levelInfo <<"Level "<< levelIndex + 1 <<" / "<< levels.size();

    int progress = 0;

    try
    {
        progress = game.getPlayer().getProgress();

        if (progress > 0)
        {
            int score = 0;

            /* getLevelScore actually gets the score from the database
             * every time it is called. This isn't exactly optimal. */
            if (static_cast<int>(levelIndex + 1) <= progress)
                score = game.getPlayer().getLevelScore(levelIndex + 1);

            if (score > 0)
            {
                levelInfo <<" (Completed)"<< std::endl;
                levelInfo <<"Highscore: "<< score;
            }
        }
    }
    catch (std::exception& e)
    {
        CL_Rect viewport = window.get_viewport();

        const int boxWidth = 300;
        const int boxHeight = 200;

        const int boxLeft = (viewport.get_width() / 2) - (boxWidth / 2);
        const int boxTop =  (viewport.get_height() / 2) - (boxHeight / 2);

        CL_Rect messageBoxRect(boxLeft, boxTop, boxLeft + boxWidth, boxTop + boxHeight);
        messageBox.show(messageBoxRect, "Error", e.what());
    }

    infoLabel.set_text(levelInfo.str());

    if (levelIndex > 0)
        previousButton.set_enabled(true);
    else
        previousButton.set_enabled(false);

    if (progress >= static_cast<int>(levelIndex + 1) && levelIndex < levels.size() - 1)
        nextButton.set_enabled(true);
    else
        nextButton.set_enabled(false);

    refresh = false;
}

void LevelSelectionState::previousClicked()
{
    if (levelIndex > 0)
    {
        levelIndex--;
        updateInfo();
    }
}

void LevelSelectionState::nextClicked()
{
    if (levelIndex < levels.size() - 1)
    {
        levelIndex++;
        updateInfo();
    }
}

void LevelSelectionState::playClicked()
{
    try
    {
        stateManager.push(new GameplayState(game, levels[levelIndex]));
        refresh = true;
    }
    catch (std::exception& e)
    {
        CL_Rect viewport = window.get_viewport();

        const int boxWidth = 300;
        const int boxHeight = 200;

        const int boxLeft = (viewport.get_width() / 2) - (boxWidth / 2);
        const int boxTop =  (viewport.get_height() / 2) - (boxHeight / 2);

        CL_Rect messageBoxRect(boxLeft, boxTop, boxLeft + boxWidth, boxTop + boxHeight);
        messageBox.show(messageBoxRect, "Error", e.what());
    }
}

void LevelSelectionState::backClicked()
{
    stateManager.pop();
}

void LevelSelectionState::update()
{
    if (refresh)
        updateInfo();
}
