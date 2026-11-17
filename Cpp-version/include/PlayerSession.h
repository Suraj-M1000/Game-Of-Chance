#ifndef PLAYERSESSION_H
#define PLAYERSESSION_H

#include "Player.h"
#include "GameEngine.h"

class PlayerSession
{
private:
    Player player;
    GameEngine engine;
    bool active;

public:
    explicit PlayerSession(Player player);

    // Prevent copying and moving the session.
    PlayerSession(const PlayerSession&) = delete;
    PlayerSession& operator=(const PlayerSession&) = delete;

    PlayerSession(PlayerSession&&) = delete;
    PlayerSession& operator=(PlayerSession&&) = delete;

    bool is_active() const;
    bool end_session();

    Player& get_player();
    const Player& get_player() const;

    GameEngine* get_engine();
    const GameEngine* get_engine() const;
};

#endif
