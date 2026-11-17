#include "PlayerSession.h"
#include <utility>

PlayerSession::PlayerSession(Player player)
    : player(std::move(player)),
      engine(this->player),
      active(true)
{}

bool PlayerSession::is_active() const
{
    return active;
}

bool PlayerSession::end_session()
{
    if (!active)
        return false;

    active = false;
    return true;
}

Player& PlayerSession::get_player()
{
    return player;
}

const Player& PlayerSession::get_player() const
{
    return player;
}

GameEngine* PlayerSession::get_engine()
{
    if (!active)
        return nullptr;

    return &engine;
}

const GameEngine* PlayerSession::get_engine() const
{
    if (!active)
        return nullptr;

    return &engine;
}
