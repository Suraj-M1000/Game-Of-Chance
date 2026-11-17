
#include "Application.h"
#include "Storage.h"

#include <iostream>
#include <stdexcept>
#include <exception>
#include <limits>
#include <utility>

Application::Application(const std::string& filePath)
    : currentSession(nullptr),
      dataFile(filePath),
      running(true)
{
}

// ========================================
// INPUT HANDLING
// ========================================

bool Application::read_int(
    const std::string& prompt,
    int& value)
{
    std::string input;

    std::cout << prompt;

    if (!std::getline(std::cin, input))
    {
        running = false;
        return false;
    }

    try
    {
        size_t position = 0;
        int number = std::stoi(input, &position);

        if (position != input.size())
            return false;

        value = number;
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}


void Application::show_profile() const
{
    if (!currentSession)
        return;

    const Player& player = currentSession->get_player();

    std::cout << "\n========================================\n";
    std::cout << "              PLAYER PROFILE\n";
    std::cout << "========================================\n";

    std::cout << "Player ID : " << player.get_uid() << '\n';
    std::cout << "Name      : " << player.get_name() << '\n';
    std::cout << "Credits   : " << player.get_credits() << '\n';
    std::cout << "Highscore : " << player.get_highscore() << '\n';

    std::cout << "========================================\n";
}


void Application::play_game()
{
    if (!currentSession)
        return;

    GameEngine* engine = currentSession->get_engine();

    if (engine == nullptr)
    {
        std::cout << "No active game session.\n";
        return;
    }

    int choice;

    std::cout << "\n========================================\n";
    std::cout << "               SELECT GAME\n";
    std::cout << "========================================\n";
    std::cout << "1. Pick A Number\n";
    std::cout << "2. Dealer No Match\n";
    std::cout << "3. Find The Ace\n";
    std::cout << "4. Back\n";
    std::cout << "========================================\n";

    if (!read_int("Enter your choice: ", choice))
    {
        if (running)
            std::cout << "Invalid input.\n";

        return;
    }

    if (choice == 4)
        return;

    int wager;

    // ========================================
    // PICK A NUMBER
    // ========================================

    if (choice == 1)
    {
        engine->set_game(GameType::PickANumber);

        if (!read_int("Enter your wager: ", wager))
        {
            if (running)
                std::cout << "Invalid wager input.\n";

            return;
        }

        if (!engine->set_wager(wager))
        {
            std::cout << "Invalid wager or insufficient credits.\n";
            return;
        }

        int guess;

        if (!read_int("Guess a number between 1 and 10: ", guess))
        {
            if (running)
                std::cout << "Invalid guess input.\n";

            return;
        }

        engine->playGame(guess);
    }

    // ========================================
    // DEALER NO MATCH
    // ========================================

    else if (choice == 2)
    {
        engine->set_game(GameType::DealerNoMatch);

        if (!read_int("Enter your wager: ", wager))
        {
            if (running)
                std::cout << "Invalid wager input.\n";

            return;
        }

        if (!engine->set_wager(wager))
        {
            std::cout << "Invalid wager or insufficient credits.\n";
            return;
        }

        std::cout << "\nDealer is generating numbers...\n";

        engine->playGame(0);
    }

    // ========================================
    // FIND THE ACE
    // ========================================

    else if (choice == 3)
    {
        if (!read_int("Enter your initial wager: ", wager))
        {
            if (running)
                std::cout << "Invalid wager input.\n";

            return;
        }

        if (!engine->startFindTheAce(wager))
        {
            std::cout << "Unable to start the game. Check your wager.\n";
            return;
        }

        int cardChoice;

        std::cout << "\n========== FIND THE ACE ==========\n";
        std::cout << "[1]  [2]  [3]\n";

        // Initial card selection.
        while (true)
        {
            if (!read_int("Choose a card (1-3): ", cardChoice))
            {
                if (!running)
                    return;

                std::cout << "Enter a valid number.\n";
                continue;
            }

            if (engine->selectFindTheAceCard(cardChoice))
                break;

            std::cout << "Invalid card. Choose between 1 and 3.\n";
        }

        // Reveal the Queen.
        int queenPosition =
            currentSession->get_player().get_uid(); // Placeholder removed below

        (void)queenPosition;

        // The revealed Queen position is exposed by FindTheAce,
        // so we will display it through a small GameEngine getter
        // added in the next step.

        int decision;

        while (true)
        {
            std::cout << "\n1. Change card selection\n";
            std::cout << "2. Increase wager\n";

            if (!read_int("Choose your decision: ", decision))
            {
                if (!running)
                    return;

                std::cout << "Invalid input.\n";
                continue;
            }

            if (decision == 1)
            {
                if (engine->changeFindTheAceSelection())
                    break;

                std::cout << "Unable to change selection.\n";
            }
            else if (decision == 2)
            {
                int additionalWager;

                if (!read_int("Enter additional wager: ", additionalWager))
                {
                    if (!running)
                        return;

                    std::cout << "Invalid wager input.\n";
                    continue;
                }

                if (engine->increaseFindTheAceWager(additionalWager))
                    break;

                std::cout << "Invalid additional wager or insufficient credits.\n";
            }
            else
            {
                std::cout << "Invalid choice.\n";
            }
        }

        GameResult result = engine->finishFindTheAce();

        std::cout << "\n========== GAME RESULT ==========\n";

        if (result == GameResult::Win)
            std::cout << "Congratulations! You found the Ace!\n";
        else if (result == GameResult::Lose)
            std::cout << "You lost! Better luck next time.\n";
        else
            std::cout << "Unable to resolve the game.\n";
    }

    else
    {
        std::cout << "Invalid game choice.\n";
        return;
    }

    std::cout << "\nCurrent Credits: "
              << currentSession->get_player().get_credits()
              << '\n';
}



// ========================================
// APPLICATION LOOP
// ========================================

void Application::run()
{
    while (running)
    {
        if (currentSession)
            player_menu();
        else
            show_welcome_menu();
    }

    std::cout << "\nThank you for playing Game of Chance!\n";
}

// ========================================
// WELCOME MENU
// ========================================

void Application::show_welcome_menu()
{
    int choice;

    std::cout << "\n========================================\n";
    std::cout << "             GAME OF CHANCE\n";
    std::cout << "========================================\n";
    std::cout << "1. Register\n";
    std::cout << "2. Login\n";
    std::cout << "3. Exit\n";
    std::cout << "========================================\n";

    if (!read_int("Enter your choice: ", choice))
    {
        if (running)
            std::cout << "Invalid input. Enter a number.\n";

        return;
    }

    switch (choice)
    {
        case 1:
            register_player();
            break;

        case 2:
            login_player();
            break;

        case 3:
            running = false;
            break;

        default:
            std::cout << "Invalid menu choice.\n";
    }
}

// ========================================
// REGISTRATION
// ========================================

void Application::register_player()
{
    int uid;
    std::string name;

    std::cout << "\n========== PLAYER REGISTRATION ==========\n";

    if (!read_int("Choose a Player ID: ", uid))
    {
        if (running)
            std::cout << "Invalid Player ID.\n";

        return;
    }

    if (uid <= 0)
    {
        std::cout << "Player ID must be positive.\n";
        return;
    }

    // Check whether this UID already exists.
    Player existingPlayer;

    if (Storage::load_player(uid, existingPlayer, dataFile))
    {
        std::cout << "This Player ID is already registered.\n";
        return;
    }

    std::cout << "Enter your name: ";

    if (!std::getline(std::cin, name))
    {
        running = false;
        return;
    }

    if (name.empty() || name.find('|') != std::string::npos)
    {
        std::cout << "Invalid name. Please enter a valid name.\n";
        return;
    }

    try
    {
        Player newPlayer(uid, name, 100);

        if (!Storage::save_player(newPlayer, dataFile))
        {
            std::cout << "Registration failed. Could not save player.\n";
            return;
        }

        currentSession =
            std::make_unique<PlayerSession>(newPlayer);

        std::cout << "\nRegistration successful!\n";
        std::cout << "Welcome, " << name << "!\n";
    }
    catch (const std::invalid_argument& error)
    {
        std::cout << "Registration error: "
                  << error.what() << '\n';
    }
}

// ========================================
// LOGIN
// ========================================

void Application::login_player()
{
    int uid;

    std::cout << "\n=============== PLAYER LOGIN ===============\n";

    if (!read_int("Enter your Player ID: ", uid))
    {
        if (running)
            std::cout << "Invalid Player ID.\n";

        return;
    }

    Player loadedPlayer;

    if (!Storage::load_player(uid, loadedPlayer, dataFile))
    {
        std::cout << "Player not found. Please register first.\n";
        return;
    }

    currentSession =
        std::make_unique<PlayerSession>(loadedPlayer);

    std::cout << "\nLogin successful!\n";
    std::cout << "Welcome back, "
              << loadedPlayer.get_name() << "!\n";
}

// ========================================
// TEMPORARY PLAYER MENU
// ========================================


void Application::player_menu()
{
    if (!currentSession)
        return;

    const Player& player = currentSession->get_player();

    int choice;

    std::cout << "\n========================================\n";
    std::cout << "              PLAYER MENU\n";
    std::cout << "========================================\n";
    std::cout << "Welcome, " << player.get_name() << "!\n";
    std::cout << "Credits: " << player.get_credits() << '\n';
    std::cout << "Highscore: " << player.get_highscore() << '\n';
    std::cout << "----------------------------------------\n";
    std::cout << "1. Play Game\n";
    std::cout << "2. View Profile\n";
    std::cout << "3. Logout\n";
    std::cout << "4. Exit\n";
    std::cout << "========================================\n";

    if (!read_int("Enter your choice: ", choice))
    {
        if (running)
            std::cout << "Invalid input.\n";

        return;
    }

    switch (choice)
    {
        case 1:
            play_game();
            break;

        case 2:
            show_profile();
            break;

        case 3:
            logout();
            break;

        case 4:
            logout();

            if (!currentSession)
                running = false;

            break;

        default:
            std::cout << "Invalid menu choice.\n";
    }
}

// ========================================
// LOGOUT
// ========================================

void Application::logout()
{
    if (!currentSession)
        return;

    Player& player = currentSession->get_player();

    // Save before ending the session.
    if (!Storage::save_player(player, dataFile))
    {
        std::cout << "Logout failed: Could not save player data.\n";
        return;
    }

    if (!currentSession->end_session())
    {
        std::cout << "Could not end the player session.\n";
        return;
    }

    currentSession.reset();

    std::cout << "\nPlayer data saved successfully.\n";
    std::cout << "Logged out successfully.\n";
}
