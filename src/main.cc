#include <ClanLib/core.h>
#include <ClanLib/application.h>

#include <vector>

#include "player.hh"
#include "towerdefensegame.hh"

/*  This class provides the entry point for the application.
    Setups ClanLib core and creates an instance of TowerDefenceGame. */
class ClanLibProgram
{
public:
    static int main(const std::vector<CL_String>& args)
    {

        CL_SetupCore setupCore;
        try
        {
            TowerDefenseGame game;
            game.run();
        }
        catch (CL_Exception& ex)
        {
            CL_ConsoleWindow console("Console", 100, 160);
            CL_Console::write_line("Error: " + ex.get_message_and_stack_trace());
            console.display_close_message();

            return -1;
        }
        return 0;
    }
};

CL_ClanApplication app(&ClanLibProgram::main);
