#ifndef STORAGE_H
#define STORAGE_H

#include "Player.h"

#include <string>

class Storage
{
public:
    static bool save_player(
        const Player& player,
        const std::string& filePath = "data/players.txt"
    );

    static bool load_player(
        int uid,
        Player& player,
        const std::string& filePath = "data/players.txt"
    );
};

#endif
