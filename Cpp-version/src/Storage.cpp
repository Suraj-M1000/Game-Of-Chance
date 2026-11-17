#include "Storage.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

// Validate player names.
static bool valid_name(const std::string& name)
{
    return !name.empty() &&
           name.find('|') == std::string::npos &&
           name.find('\n') == std::string::npos &&
           name.find('\r') == std::string::npos;
}

bool Storage::save_player(
    const Player& player,
    const std::string& filePath)
{
    if (player.get_uid() <= 0 ||
        !valid_name(player.get_name()) ||
        player.get_credits() < 0 ||
        player.get_highscore() < player.get_credits())
    {
        return false;
    }

    std::ofstream file(filePath, std::ios::app);

    if (!file.is_open())
        return false;

    file << player.get_uid() << '|'
         << player.get_name() << '|'
         << player.get_credits() << '|'
         << player.get_highscore() << '\n';

    return static_cast<bool>(file);
}

// Convert string into integer safely.
static bool parse_integer(const std::string& value, int& result)
{
    try
    {
        size_t position = 0;

        int number = std::stoi(value, &position);

        if (position != value.size())
            return false;

        result = number;
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}


bool Storage::load_player(
    int uid,
    Player& player,
    const std::string& filePath)
{
    if (uid <= 0)
        return false;

    std::ifstream file(filePath);

    if (!file.is_open())
        return false;

    bool found = false;

    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream stream(line);

        std::vector<std::string> fields;
        std::string field;

        while (std::getline(stream, field, '|'))
            fields.push_back(field);

        if (fields.size() != 4)
            continue;

        int recordUID;
        int credits;
        int highscore;

        if (!parse_integer(fields[0], recordUID) ||
            !parse_integer(fields[2], credits) ||
            !parse_integer(fields[3], highscore))
        {
            continue;
        }

        if (recordUID != uid)
            continue;

        if (!valid_name(fields[1]) ||
            credits < 0 ||
            highscore < credits)
        {
            continue;
        }

        try
        {
            Player loadedPlayer(
                recordUID,
                fields[1],
                credits
            );

            loadedPlayer.set_highscore(highscore);

            player = loadedPlayer;
            found = true;
        }
        catch (const std::invalid_argument&)
        {
            continue;
        }
    }

    return found;
}
