#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl1.h>
#include <ClanLib/swrender.h>

#include "gamestatemanager.hh"
#include "gameplaystate.hh"
#include "towerdefensegame.hh"
#include "fpscounter.hh"
#include "gamecomponent.hh"

void TowerDefenseGame::run()
{
    CL_SetupDisplay setupDisplay;

    // This should come from a configuration file
    CL_SetupGL1 setupRender;
    //CL_SetupSWRender setup;
    CL_DisplayWindowDescription desc;
    desc.set_title("TD");
    desc.set_size(CL_Size(800, 600), false);
    desc.show_maximize_button(false);
    desc.set_allow_resize(false);
    CL_DisplayWindow window(desc);

    CL_GUIManager guiManager(window, "data/gui/");
    GameComponent game(&guiManager, window);

    CL_Slot closeSlot = window.sig_window_close().connect(this, &TowerDefenseGame::onWindowClose, &guiManager);

    guiManager.exec(true);
}

void TowerDefenseGame::onWindowClose(CL_GUIManager* guiManager)
{
    guiManager->exit_with_code(0);
}
