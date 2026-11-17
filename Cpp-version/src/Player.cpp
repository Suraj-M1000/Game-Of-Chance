
#include "Player.h"
#include <stdexcept>

Player::Player()   // Player - Class name , :: - scope resolution operator. It tells C++ that this function belongs to the Player class., Player() - constructor name
    : uid(0), name(""), credits(0), highscore(0)   // member initializer list - When constructing this object, initialize its members with these values.
{
}

Player::Player(int uid, const std::string& name, int startingCredits)
    : uid(uid),
      name(name),
      credits(startingCredits),
      highscore(startingCredits)
{
    if (uid <= 0)
        throw std::invalid_argument("Player UID must be positive.");

    if (name.empty())
        throw std::invalid_argument("Player name cannot be empty.");

    if (startingCredits < 0)
        throw std::invalid_argument("Starting credits cannot be negative.");
}

int Player::get_uid() const{    // Player:: - The get_uid() function that belongs to Player.
    return uid;
}

void Player::set_uid(int uid){
    this->uid = uid;
}

int Player::get_credits() const{
    return credits;
}

void Player::set_credits(int credits)
{
    if (credits < 0)
        return;

    this->credits = credits;
    update_highscore();
}

const std::string& Player::get_name() const{
    return name;
}

void Player::set_name(const std::string &name){
    this->name = name;
}

int Player::get_highscore() const{
    return highscore;
}


void Player::set_highscore(int highscore)
{
    if (highscore > this->highscore && highscore >= credits)
        this->highscore = highscore;
}

void Player::add_credits(int amount)
{
    if (amount <= 0)
        return;

    credits += amount;
    update_highscore();
}

bool Player::deduct_credits(int amount)
{
    if (amount <= 0 || amount > credits)
        return false;

    credits -= amount;
    return true;
}


void Player::update_highscore()
{
    if (credits > highscore)
        highscore = credits;
}
