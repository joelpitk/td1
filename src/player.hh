#ifndef PLAYER_HH
#define PLAYER_HH

#include <ClanLib/core.h>
#include <ClanLib/database.h>
#include <ClanLib/sqlite.h>

#include "common.hh"

class Player {

public:

    Player(const CL_String& name, const CL_String& password, bool login);

    std::string& getName();
    std::string& getPassword();
    int getId();
    int getProgress();
    int getLevelScore(int level_id);

    void updateLevelScore(int level_id, int newscore);
    void updateProgress(int newlevel);

    void deletePlayer();

private:

    std::string name;
    std::string password;
    int id;
    int progress;
};

void createDatabase();
void printDatabase(Player& player);

#endif
