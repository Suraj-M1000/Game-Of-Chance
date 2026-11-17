
#ifndef APPLICATION_H
#define APPLICATION_H

#include "PlayerSession.h"

#include <memory>
#include <string>

class Application
{
private:
    std::unique_ptr<PlayerSession> currentSession;

    std::string dataFile;
    bool running;

    // Welcome menu
    void show_welcome_menu();

    // Account operations
    void register_player();
    void login_player();

    // Player menu
    void player_menu();
    void show_profile() const;
    void logout();

    // Gameplay
    void play_game();

    // Input handling
    bool read_int(const std::string& prompt, int& value);

public:
    explicit Application(
        const std::string& filePath = "data/players.txt"
    );

    void run();
};

#endif
