
#ifndef GUIAPPLICATION_H
#define GUIAPPLICATION_H

#include "raylib.h"
#include "PlayerSession.h"
#include "GameResult.h"

#include <string>
#include <memory>

enum class GuiScreen
{
    Welcome,
    Register,
    Login,
    MainMenu,
    GameSelection,
    PickANumber,
    DealerNoMatch,
    FindTheAce,
    Profile
};

class GuiApplication
{
private:
    // Session management
    std::unique_ptr<PlayerSession> session;
    std::string dataFile;

    GuiScreen currentScreen;

    // Authentication input
    std::string uidInput;
    std::string nameInput;
    std::string authMessage;

    // Pick A Number GUI state
    std::string wagerInput = "10";
    int selectedNumber = 1;
    bool hasPlayed = false;
    std::string resultMessage;

    // Find The Ace GUI state
    std::string aceInitialWagerInput = "10";
    std::string aceAdditionalWagerInput = "10";
    std::string aceResultMessage;

    bool uidFieldActive = true;
    bool nameFieldActive = false;

    // Screen drawing
    void drawWelcome();
    void drawRegister();
    void drawLogin();

    void drawMainMenu();
    void drawGameSelection();
    void drawPickANumber();
    void drawDealerNoMatch();
    void drawFindTheAce();
    void drawProfile();

    // Authentication operations
    void registerPlayer();
    void loginPlayer();
    void logoutPlayer();

public:
    explicit GuiApplication(
        const std::string& filePath = "data/players.txt"
    );

    void run();
};

#endif
