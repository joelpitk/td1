#ifndef GAMECOMPONENT_HH
#define GAMECOMPONENT_HH

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gui.h>
#include "gamestatemanager.hh"
#include "menustate.hh"
#include "fpscounter.hh"
#include "loginstate.hh"

class GameComponent : public CL_GUIComponent
{
public:
    GameComponent(CL_GUIManager *guiManager, CL_DisplayWindow& window)
        : CL_GUIComponent(guiManager, CL_GUITopLevelDescription(window.get_viewport(), false)),
          guiManager(guiManager), window(window), resources("data/resources.xml"), fpsCounter(30), speed(17)
    {
        set_constant_repaint(true);
        func_render().set(this, &GameComponent::onRender);
        font = CL_Font(get_gc(), "Tahoma", 16);

        stateManager.push(new LoginState(*this));

        timer.func_expired().set(this, &GameComponent::update);
        timer.start(17, true);
    }

    CL_DisplayWindow& getWindow()
    {
        return window;
    }

    CL_GraphicContext& getGraphicContext()
    {
        return get_gc();
    }

    GameStateManager& getStateManager()
    {
        return stateManager;
    }

    CL_InputDevice& getKeyboard() const
    {
        return window.get_ic().get_keyboard();
    }

    CL_InputDevice& getMouse() const
    {
        return window.get_ic().get_mouse();
    }

    CL_ResourceManager& getResourceManager()
    {
        return resources;
    }

    void loginPlayer(Player* p)
    {
        player = p;
    }

    Player& getPlayer()
    {
        return *player;
    }

    void logoutPlayer()
    {
        delete player;
        player = NULL;
    }

    void toggleSpeed() {
        speed = (speed == 17) ? 4 : 17;
        timer.stop();
        timer.start(speed, true);
    }
    
private:
    void onRender(CL_GraphicContext& gc, const CL_Rect& clip_rect)
    {
        get_gc().clear(CL_Colorf::black);
        fpsCounter.beginFrame();
        stateManager.draw();
        fpsCounter.endFrame();

        font.draw_text(get_gc(), 10, 40, cl_format("%1 FPS", fpsCounter.getFPS()));
    }

    void update()
    {
        if (!stateManager.update())
            guiManager->exit_with_code(0);
    }

    CL_GUIManager* guiManager;
    CL_DisplayWindow& window;
    CL_ResourceManager resources;
    CL_Timer timer;
    GameStateManager stateManager;
    CL_Font font;
    FPSCounter fpsCounter;
    Player* player;
    int speed;        
};

#endif
