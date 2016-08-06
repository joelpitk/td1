#include "towerstats.hh"
#include "level.hh"
#include "tower.hh"

TowerStats::TowerStats(CL_GUIComponent* parent, CL_Size size)
    : size(size), window(parent, CL_GUITopLevelDescription(CL_Rect(0, 0, size.width, size.height), false)),
      upgradeButton(&window), sellButton(&window), statsLabel(&window), tower(NULL), level(NULL)
{
    window.set_visible(false, false);
    window.set_title("Tower stats");
    window.set_id_name("stats");
    window.func_close().set(this, &TowerStats::hide);

    const int windowWidth = window.get_width();
    const int windowHeight = window.get_height();

    const int buttonWidth = windowWidth / 3;
    const int buttonHeight = 20;
    const int upgradeButtonLeft = windowWidth / 10;
    const int margin = 10;

    CL_Rect upgradeButtonRect(upgradeButtonLeft, windowHeight - buttonHeight - margin, upgradeButtonLeft + buttonWidth, windowHeight - margin);
    upgradeButton.set_geometry(upgradeButtonRect);
    upgradeButton.func_clicked().set(this, &TowerStats::upgradeClicked);

    const int sellButtonLeft = windowWidth - buttonWidth - windowWidth / 10;
    CL_Rect sellButtonRect(sellButtonLeft, windowHeight - buttonHeight - margin, sellButtonLeft + buttonWidth, windowHeight - margin);
    sellButton.set_geometry(sellButtonRect);
    sellButton.func_clicked().set(this, &TowerStats::sellClicked);

    CL_Rect clientArea = window.get_client_area();
    CL_Rect labelRect(clientArea.left, clientArea.top, clientArea.right, clientArea.bottom - buttonHeight - margin);
    statsLabel.set_geometry(labelRect);

    upgradeButton.set_text("Upgrade");
    sellButton.set_text("Sell");
}

void TowerStats::show(CL_Point position, Tower* t, Level* l)
{
    tower = t;
    level = l;

    updateStats();

    CL_Rect wr;
    wr.set_top_left(position);
    wr.set_size(size);

    window.set_geometry(wr);
    window.set_visible(true, false);
}

bool TowerStats::hide()
{
    window.set_visible(false, false);
    return true;
}

void TowerStats::upgradeClicked()
{
    if (tower && level)
    {
        if (level->changeMoney(-tower->getUpgradePrice()))
        {
            tower->upgrade();
            updateStats();
        }
    }
}

void TowerStats::sellClicked()
{
    if (tower && level)
    {
        level->changeMoney(tower->getSellPrice());
        level->removeTower(tower);
        level->updatePaths();
        tower = NULL;
        hide();
    }
}

void TowerStats::updateStats()
{
    if (tower)
    {
        std::stringstream stats;
        stats<<"Damage: "<<tower->getDamage()<<std::endl
             <<"Radius: "<<tower->getRadius()<<std::endl
             <<"Upgrade price: "<<tower->getUpgradePrice()<<std::endl
             <<"Sell price: "<<tower->getSellPrice();

        statsLabel.set_text(stats.str());
    }
}
