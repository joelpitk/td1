#include "player.hh"

// Player constructor.
Player::Player(const CL_String& name, const CL_String& password, bool login) : name(name), password(password)
{
    try 
    {
        CL_SqliteConnection connection("database.db");

        // If the player is logging in.
        if (login) 
        {
            CL_DBCommand login_cmd = connection.create_command("SELECT PlayerId, Progress FROM Players WHERE PlayerName=?1 AND Password=?2");

            login_cmd.set_input_parameter_string(1, name);
            login_cmd.set_input_parameter_string(2, password);

            CL_DBReader login_reader = connection.execute_reader(login_cmd);
            login_reader.retrieve_row();

            if (login_reader.get_column_int(0)) 
            {
                id = login_reader.get_column_int(0);
                progress = login_reader.get_column_int(1);
            }
            else 
            {
                // Invalid user information, did not recieve player id.
                throw std::invalid_argument("Invalid username or password.");
            }

            login_reader.close();
        }
        
        // If the player is new and now created.
        else 
        {
            CL_DBTransaction transaction = connection.begin_transaction();

            // Adding the player data to the table.
            CL_DBCommand create_cmd = connection.create_command("INSERT OR IGNORE INTO Players (PlayerName, Password) VALUES (?1, ?2)");
            create_cmd.set_input_parameter_string(1, name);
            create_cmd.set_input_parameter_string(2, password);
            connection.execute_non_query(create_cmd);

            // If the player name already exists in the database throw error.
            if (!create_cmd.get_output_last_insert_rowid())
            {
                throw std::invalid_argument("Player name already in use.");
            }

            // Fetching the player id from the database (primary key).
            CL_DBCommand id_cmd = connection.create_command("SELECT PlayerId FROM Players WHERE PlayerName=?1");
            id_cmd.set_input_parameter_string(1, name);
            id = connection.execute_scalar_int(id_cmd);
            progress = 0;

            transaction.commit();
        }
    }
    catch (CL_Exception& e) 
    {
        // Basically database connection error.
        throw std::invalid_argument("Database connection error.");
    }
}

// Returns the player's name.
std::string& Player::getName() 
{
    return name;
}

// Returns the player's password.
std::string& Player::getPassword()
{
    return password;
}

// Returns the player's id.
int Player::getId() 
{
    return id;
}

// Returns the player's id.
int Player::getProgress() 
{
    return progress;
}


int Player::getLevelScore(int level_id)
{
    CL_SqliteConnection connection("database.db");

    CL_DBCommand get_score_cmd = connection.create_command("SELECT Score FROM Scores WHERE PlayerId=?1 AND LevelId=?2");
    get_score_cmd.set_input_parameter_int(1, id);
    get_score_cmd.set_input_parameter_int(2, level_id);
    int score = connection.execute_scalar_int(get_score_cmd);

    return score;
}

void Player::updateProgress(int newlevel)
{
    CL_SqliteConnection connection("database.db");

    CL_DBCommand update_cmd = connection.create_command("UPDATE Players SET Progress=?1 WHERE PlayerId=?2");
    update_cmd.set_input_parameter_int(1, newlevel);
    update_cmd.set_input_parameter_int(2, id);
    connection.execute_non_query(update_cmd);
    progress = newlevel;
}

// Updates the player's score for level entered in the parameter.
void Player::updateLevelScore(int level_id, int newscore) 
{
    try 
    {
        CL_SqliteConnection connection("database.db");

        // Checking row count to see if there is already an entry for the level score in the database.
        CL_DBCommand count_cmd = connection.create_command("SELECT COUNT(*) FROM Scores WHERE LevelId=?1 AND PlayerId=?2");
        count_cmd.set_input_parameter_int(1, level_id);
        count_cmd.set_input_parameter_int(2, id);
        int retval = connection.execute_scalar_int(count_cmd);

        // If the score for that player and level exists.
        if (retval) 
        {
            CL_DBCommand update_cmd = connection.create_command("UPDATE Scores SET Score=?1 WHERE LevelId=?2 AND PlayerId=?3");
            update_cmd.set_input_parameter_int(1, newscore);
            update_cmd.set_input_parameter_int(2, level_id);
            update_cmd.set_input_parameter_int(3, id);
            connection.execute_non_query(update_cmd);
        }
        // If the score for that player and level doesn't exist.
        else
        {
            CL_DBCommand create_cmd = connection.create_command("INSERT INTO Scores VALUES (?1,?2,?3)");
            create_cmd.set_input_parameter_int(1, level_id);
            create_cmd.set_input_parameter_int(2, id);
            create_cmd.set_input_parameter_int(3, newscore);
            connection.execute_non_query(create_cmd);
        }
    }
    catch (CL_Exception& e) 
    {
        throw std::invalid_argument("Error while updating level score.");
    }
}

// Deletes the current player from the database.
void Player::deletePlayer()
{
    try 
    {
        CL_SqliteConnection connection("database.db");

        // Database queries handled as transactions to ensure that everything goes into the db.
        CL_DBTransaction transaction = connection.begin_transaction();

        CL_DBCommand del_player_cmd = connection.create_command("DELETE FROM Players WHERE PlayerId=?1");

        del_player_cmd.set_input_parameter_int(1, id);

        CL_DBCommand del_score_cmd = connection.create_command("DELETE FROM Scores WHERE PlayerId=?1");
        
        del_score_cmd.set_input_parameter_int(1, id);

        connection.execute_non_query(del_player_cmd);
        connection.execute_non_query(del_score_cmd);

        transaction.commit();
    }
    catch (CL_Exception& e)
    {
        throw std::invalid_argument("Database connection error.");
    }
}

void createDatabase() {
    try 
    {
        CL_SqliteConnection connection("database.db");

        // Database queries handled as transactions to ensure that everything goes into the db.
        CL_DBTransaction transaction = connection.begin_transaction();

        CL_DBCommand create_scores_cmd = connection.create_command("CREATE TABLE Scores (LevelId INTEGER NOT NULL, PlayerId INTEGER NOT NULL, Score INTEGER DEFAULT 0, PRIMARY KEY (LevelId, PlayerId));");
        CL_DBCommand create_players_cmd = connection.create_command("CREATE TABLE Players (PlayerId INTEGER PRIMARY KEY, PlayerName VARCHAR(20) UNIQUE NOT NULL, Password VARCHAR(20) NOT NULL, Progress INTEGER DEFAULT 0, FOREIGN KEY (PlayerId) REFERENCES Scores(PlayerId));");
        
        connection.execute_non_query(create_scores_cmd);
        connection.execute_non_query(create_players_cmd);

        transaction.commit();
    }
    catch (CL_Exception& e) 
    {
        throw std::invalid_argument("Database connection error.");
    }
}

void printDatabase(Player& player) {

    CL_SqliteConnection connection("database.db");

    CL_Console::write_line("Information of the logged in player");
    CL_Console::write_line("PlayerId: %1, Password: %2, Progress: %3\n", player.getId(), player.getPassword(), player.getProgress());
        
    for (int i = 1; i < player.getProgress(); i++)
    {
        CL_Console::write_line("LevelId: %1, Score: %2", i, player.getLevelScore(i));
    }

    CL_Console::write_line("");

    // Reading the players table
    CL_Console::write_line("All rows from Players table:");

    CL_DBCommand read_players_cmd = connection.create_command("SELECT * FROM Players");
    CL_DBReader player_reader = connection.execute_reader(read_players_cmd);
    while (player_reader.retrieve_row())
    {
        int player_id = player_reader.get_column_int(0);
        CL_String player_name = player_reader.get_column_string(1);
        CL_String player_password = player_reader.get_column_string(2);
        int player_progress = player_reader.get_column_int(3);

        CL_Console::write_line("Id: %1, Player name: %2, Password: %3, Progress: %4", player_id, player_name, player_password, player_progress);
    }
    player_reader.close();

    // Reading the scores table
    CL_Console::write_line("\nAll rows from Scores table:");

    CL_DBCommand read_scores_cmd = connection.create_command("SELECT * FROM Scores");
    CL_DBReader score_reader = connection.execute_reader(read_scores_cmd);
    while (score_reader.retrieve_row())
    {
        int level_id = score_reader.get_column_int(0);
        int player_id = score_reader.get_column_int(1);
        int player_score = score_reader.get_column_int(2);

        CL_Console::write_line("LevelId: %1, PlayerId: %2, Score: %3", level_id, player_id, player_score);
    }
    score_reader.close();

    CL_Console::write_line("");
}
