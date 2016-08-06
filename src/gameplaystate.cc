#include <ClanLib/display.h>
#include "gamestatemanager.hh"
#include "gamestate.hh"
#include "gameplaystate.hh"
#include "renderer.hh"
#include "level.hh"
#include "gamecomponent.hh"
#include "tower.hh"
#include "constants.hh"

GameplayState::GameplayState(GameComponent& game, const std::string levelFile) try :
    GameState(game), renderer(gc, camera), level(levelFile, gc, resources),
    font(gc, "Monospace", 16), lastMousePosition(mouse.get_position()),
    towerButton1(&guiParent), towerButton2(&guiParent), towerButton3(&guiParent), towerButton4(&guiParent),
    tower1Sprite(gc, "sprites/guntower_prev", &resources), tower2Sprite(gc, "sprites/areatower_prev", &resources), tower3Sprite(gc, "sprites/cannontower_prev", &resources), tower4Sprite(gc, "sprites/icetower_prev", &resources),
    towerStats(&guiParent, CL_Size(160, 120)), youWinLoseBox(&guiParent), buildTowerType(-1)
{

    slots.connect(mouse.sig_pointer_move(), this, &GameplayState::mouseMoved);
    slots.connect(mouse.sig_key_up(), this, &GameplayState::mouseButtonUp);
    slots.connect(keyboard.sig_key_up(), this, &GameplayState::keyUp);
    slots.connect(youWinLoseBox.windowClosedSignal, this, &GameplayState::exitState);

    createGUI();
}
catch (std::exception& e)
{
    throw;
}

void GameplayState::createGUI()
{
    CL_Rect viewport = window.get_viewport();

    const int bottomBarHeight = viewport.get_height() / 10;
    const int bottomBarWidth = viewport.get_width() - 100;

    int left = (viewport.get_width() - bottomBarWidth) / 2;
    const int top = viewport.bottom - bottomBarHeight;
    const int right = viewport.right - left;
    const int bottom = viewport.bottom;

    const int buttonWidth = 50;
    const int buttonHeight = 50;

    const int margin = 5;

    bottomBar = CL_Rect(left, top, right, bottom);

    towerButton1.set_geometry(CL_Rect(left + margin, top + margin, left + buttonWidth, top + margin + buttonHeight));
    towerButton1.set_id_name("guntower");
    towerButton1.func_clicked().set(this, &GameplayState::buildTowerClicked, 0);

    left += buttonWidth;

    towerButton2.set_geometry(CL_Rect(left + margin, top + margin, left + buttonWidth, top + margin + buttonHeight));
    towerButton2.set_id_name("areatower");
    towerButton2.func_clicked().set(this, &GameplayState::buildTowerClicked, 1);
    
    left += buttonWidth;

    towerButton3.set_geometry(CL_Rect(left + margin, top + margin, left + buttonWidth, top + margin + buttonHeight));
    towerButton3.set_id_name("cannontower");
    towerButton3.func_clicked().set(this, &GameplayState::buildTowerClicked, 2);

    left += buttonWidth;

    towerButton4.set_geometry(CL_Rect(left + margin, top + margin, left + buttonWidth, top + margin + buttonHeight));
    towerButton4.set_id_name("icetower");
    towerButton4.func_clicked().set(this, &GameplayState::buildTowerClicked, 3);

    tower1Sprite.set_alpha(0.5f);
    tower2Sprite.set_alpha(0.5f);
    tower3Sprite.set_alpha(0.5f);
    tower4Sprite.set_alpha(0.5f);

    left += buttonWidth + margin;

    const int healthBarWidth = level.getPlayerHealth() * 10;
    const int healthBarHeight = 10;

    healthBar = CL_Rect(left, bottomBar.top + margin, left + healthBarWidth, bottomBar.top + margin + healthBarHeight);
}

void GameplayState::mouseMoved(const CL_InputEvent& evt, const CL_InputState& state)
{
    if (level.getLost() && level.hasWon()) return;

    CL_Point currentMousePosition = mouse.get_position();

    if (mouse.get_keycode(CL_MOUSE_RIGHT))
    {
        towerStats.hide();
        CL_Point mouseMovement = currentMousePosition - lastMousePosition;
        camera.move(-mouseMovement.x, -mouseMovement.y);
    }

    lastMousePosition = currentMousePosition;
}

void GameplayState::mouseButtonUp(const CL_InputEvent& evt, const CL_InputState& state)
{
    if (level.getLost() && level.hasWon()) return;

    if (evt.id == CL_MOUSE_LEFT && !bottomBar.contains(evt.mouse_pos))
    {
        CL_Vec2f mouseWorld = mousePositionToWorldPosition(mouse.get_position());
        std::pair<size_t, size_t> levelCoordinates = level.getCoords(mouseWorld.x, mouseWorld.y);
        std::pair<size_t, size_t> dimensions = level.getDimensions();
        size_t row = levelCoordinates.first;
        size_t column = levelCoordinates.second;

        if (row < dimensions.second && column < dimensions.first)
        {
            Tile& tile = level.getTiles()[row][column];
            Tower* selectedTower = tile.getTower();

            if (selectedTower && buildTowerType == -1)
                towerStats.show(mouse.get_position(), selectedTower, &level);
        }

        if (buildTowerType > -1)
        {
            buildTower(row, column);
        }
    }
}

void GameplayState::keyUp(const CL_InputEvent& evt, const CL_InputState& state)
{
    if (level.getLost() && level.hasWon()) return;

    if (evt.id == CL_KEY_1)
    {
        if (towerButton1.is_enabled())
            buildTowerClicked(0);
    }

    if (evt.id == CL_KEY_2)
    {
        if (towerButton2.is_enabled())
            buildTowerClicked(1);
    }

    if (evt.id == CL_KEY_3)
    {
        if (towerButton3.is_enabled())
            buildTowerClicked(2);
    }

    if (evt.id == CL_KEY_4)
    {
        if (towerButton4.is_enabled())
            buildTowerClicked(3);
    }

    if (evt.id == CL_KEY_Q)
    {
        renderer.toggleDrawRanges();
    }

    if (evt.id == CL_KEY_SPACE)
    {
        game.toggleSpeed();
    }

    if (DEBUG && evt.id == CL_KEY_K)
    {
        level.killAll();
    }
}

CL_Vec2f GameplayState::mousePositionToWorldPosition(const CL_Point& mousePosition) const
{
    const CL_Vec2f cameraPosition = camera.getPosition();
    return CL_Vec2f(mousePosition.x + cameraPosition.x, mousePosition.y + cameraPosition.y);
}

void GameplayState::buildTowerClicked(int towerType)
{
    if (level.getLost() && level.hasWon()) return;

    if (buildTowerType == -1)
        buildTowerType = towerType;
    else
        buildTowerType = -1;
}

void GameplayState::buildTower(size_t row, size_t column)
{
    std::pair<size_t, size_t> dimensions = level.getDimensions();

    if (row < dimensions.second && column < dimensions.first)
    {
        Tile& tile = level.getTiles()[row][column];
        if (tile.isBuildable())
        {
            bool walkable = tile.isWalkable();
            tile.setWalkable(false);
            if (level.updatePaths())
            {
                // These two sprite groups determine the sprites for tower and its projectiles.
                ProjectileSprites projectile_sprites;
                TowerSprites tower_sprites;

                /* TYPE MAPPING:
                 * 0: GUNTOWER
                 * 1: AREATOWER
                 * 2: CANNONTOWER
                 * 3: ICETOWER
                 */

                // All towers start from upgrade level 0.
                tower_sprites.upgradeSprite = CL_Sprite(gc, "sprites/upgrade0", &resources);

                // These are the upgrade sprites for all the towers.
                tower_sprites.lvl_1 = CL_Sprite(gc, "sprites/upgrade1", &resources);
                tower_sprites.lvl_2 = CL_Sprite(gc, "sprites/upgrade2", &resources);
                tower_sprites.lvl_3 = CL_Sprite(gc, "sprites/upgrade3", &resources);

                switch (buildTowerType)
                {
                case 0:
                    projectile_sprites.projectileSprite = CL_Sprite(gc);
                    projectile_sprites.flySprite = CL_Sprite(gc, "sprites/guntower_projectile", &resources);
                    projectile_sprites.damageSprite = CL_Sprite(gc, "sprites/guntower_projectile", &resources);

                    tower_sprites.baseSprite = CL_Sprite(gc, "sprites/guntower", &resources);
                    tower_sprites.gunSprite = CL_Sprite(gc, "sprites/guntower_gun", &resources);

                    level.addTower(new GunTower(level, row, column, tower_sprites, projectile_sprites));
                    level.updatePaths();
                    level.changeMoney(-GunTower::price);
                    break;

                case 1:
                    projectile_sprites.projectileSprite = CL_Sprite(gc);
                    projectile_sprites.flySprite = CL_Sprite(gc, "sprites/areatower_projectile", &resources);
                    projectile_sprites.damageSprite = CL_Sprite(gc, "sprites/areatower_projectile", &resources);

                    tower_sprites.baseSprite = CL_Sprite(gc, "sprites/areatower", &resources);
                    tower_sprites.gunSprite = CL_Sprite(gc, "sprites/areatower_gun", &resources);

                    level.addTower(new AreaTower(level, row, column, tower_sprites, projectile_sprites));
                    level.updatePaths();
                    level.changeMoney(-AreaTower::price);
                    break;
                case 2:
                    projectile_sprites.projectileSprite = CL_Sprite(gc);
                    projectile_sprites.flySprite = CL_Sprite(gc, "sprites/cannontower_projectile", &resources);
                    projectile_sprites.damageSprite = CL_Sprite(gc, "sprites/cannontower_projectile", &resources);

                    tower_sprites.baseSprite = CL_Sprite(gc, "sprites/cannontower", &resources);
                    tower_sprites.gunSprite = CL_Sprite(gc, "sprites/cannontower_gun", &resources);

                    level.addTower(new CannonTower(level, row, column, tower_sprites, projectile_sprites));
                    level.updatePaths();
                    level.changeMoney(-CannonTower::price);
                    break;
                case 3:
                    projectile_sprites.projectileSprite = CL_Sprite(gc);
                    projectile_sprites.flySprite = CL_Sprite(gc, "sprites/icetower_projectile", &resources);
                    projectile_sprites.damageSprite = CL_Sprite(gc, "sprites/icetower_projectile", &resources);

                    tower_sprites.baseSprite = CL_Sprite(gc, "sprites/icetower", &resources);
                    tower_sprites.gunSprite = CL_Sprite(gc, "sprites/icetower_gun", &resources);

                    level.addTower(new IceTower(level, row, column, tower_sprites, projectile_sprites));
                    level.updatePaths();
                    level.changeMoney(-IceTower::price);
                    break;

                }
            }
            tile.setWalkable(walkable);
        }
        buildTowerType = -1;
    }
}

void GameplayState::exitState()
{
    if (level.hasWon())
    {
        Player& player = game.getPlayer();

        int progress = player.getProgress();
        int levelNumber = level.getNumber();
        int score = level.getScore();


        if (progress < levelNumber)
        {
            // The player hadn't completed this level before so update progress and score
            player.updateProgress(levelNumber);
            player.updateLevelScore(levelNumber, score);
        }
        else
        {
            // The player had completed this level before so check the highscore and update if required
            int highscore = player.getLevelScore(levelNumber);
            if (score > highscore)
                player.updateLevelScore(levelNumber, score);
        }
    }
    stateManager.pop();
}

void GameplayState::update()
{
    const float cameraSpeed = 8.7f;

    if (level.getLost() || level.hasWon())
    {

        if (youWinLoseBox.isVisible()) return;

        CL_Rect viewport = window.get_viewport();
        const int boxWidth = 300;
        const int boxHeight = 200;

        const int boxLeft = (viewport.get_width() / 2) - (boxWidth / 2);
        const int boxTop =  (viewport.get_height() / 2) - (boxHeight / 2);

        CL_Rect messageBoxRect(boxLeft, boxTop, boxLeft + boxWidth, boxTop + boxHeight);

        if (level.getLost())
            youWinLoseBox.show(messageBoxRect, "", "YOU LOSE!", "winloselabel");
        else
        {
            youWinLoseBox.show(messageBoxRect, "", "YOU WIN!", "winloselabel");
        }
    }
    else
    {
        if (keyboard.get_keycode(CL_KEY_LEFT))
            camera.move(-cameraSpeed, 0);

        if (keyboard.get_keycode(CL_KEY_RIGHT))
            camera.move(cameraSpeed, 0);

        if (keyboard.get_keycode(CL_KEY_UP))
            camera.move(0, -cameraSpeed);

        if (keyboard.get_keycode(CL_KEY_DOWN))
            camera.move(0, cameraSpeed);

        if (keyboard.get_keycode(CL_KEY_ESCAPE))
        {
            stateManager.pop();
        }

        level.update();

        if (level.getPlayerHealth() > 0)
            healthBar.set_width(level.getPlayerHealth() * 10);
        else
            healthBar.set_width(0);

        int money = level.getMoney();

        if (money < GunTower::price)
            towerButton1.set_enabled(false);
        else
            towerButton1.set_enabled(true);

        if (money < AreaTower::price)
            towerButton2.set_enabled(false);
        else
            towerButton2.set_enabled(true);

        if (money < CannonTower::price)
            towerButton3.set_enabled(false);
        else
            towerButton3.set_enabled(true);

        if (money < IceTower::price)
            towerButton4.set_enabled(false);
        else
            towerButton4.set_enabled(true);
    }
}

void GameplayState::draw()
{
    renderer.render(level.getTiles());
    renderer.render(level.getDeadMobs());
    renderer.render(level.getTowers());
    renderer.render(level.getMobs());
    renderer.render(level.getProjectiles());


    if (buildTowerType > -1)
    {
        const CL_Vec2f& cameraPosition = camera.getPosition();
        CL_Point mousePosition = mouse.get_position();

        CL_Vec2f worldPos(((mousePosition.x + static_cast<int> (cameraPosition.x)) / TILE_SIZE) * TILE_SIZE,
                          ((mousePosition.y + static_cast<int> (cameraPosition.y)) / TILE_SIZE) * TILE_SIZE);

        switch (buildTowerType)
        {
        case 0:
            renderer.render(tower1Sprite, worldPos);
            break;
        case 1:
            renderer.render(tower2Sprite, worldPos);
            break;
        case 2:
            renderer.render(tower3Sprite, worldPos);
            break;
        case 3:
            renderer.render(tower4Sprite, worldPos);
            break;
        default:
            std::cerr << "Unknown tower type" << std::endl;
        }
    }

    CL_Draw::fill(gc, bottomBar, CL_Colorf::chocolate);
    CL_Draw::fill(gc, healthBar, CL_Colorf::greenyellow);
    font.draw_text(gc, 10, 25, cl_format("Mouse: %1, %2", mouse.get_x(), mouse.get_y()));

    font.draw_text(gc, healthBar.left, healthBar.bottom + healthBar.get_height() + 5, cl_format("Money: %1, Score: %2, Next wave in: %3 seconds, Waves left: %4", level.getMoney(), level.getScore(), level.getWaveInterval(), level.getWavesSize()), CL_Colorf::black);
}

