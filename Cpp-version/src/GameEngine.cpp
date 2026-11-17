
#include "GameEngine.h"
#include <iostream>
#include <string>

GameEngine::GameEngine(Player& player)
    : player(player),
      selectedGame(GameType::PickANumber),
      wager(0)
{}

Player& GameEngine::get_player()
{
    return player;
}

void GameEngine::set_game(GameType game)
{
    selectedGame = game;
}

GameType GameEngine::get_game() const
{
    return selectedGame;
}

bool GameEngine::set_wager(int wager)
{
    if (wager <= 0 || wager > player.get_credits())
    {
        this->wager = 0;
        return false;
    }

    this->wager = wager;
    return true;
}

int GameEngine::get_wager() const
{
    return wager;
}


// ========================================
// SINGLE-ACTION GAMES
// ========================================

GameResult GameEngine::playGame(int input)
{
    // Find the Ace uses its own multi-step interface.
    if (selectedGame == GameType::FindTheAce)
    {
        std::cout << "Use the Find the Ace methods to play this game.\n";
        return GameResult::InvalidInput;
    }

    if (wager <= 0 || wager > player.get_credits())
    {
        std::cout << "Invalid wager. Cannot play.\n";
        return GameResult::InvalidWager;
    }

    GameResult result = GameResult::InvalidInput;

    switch (selectedGame)
    {
        case GameType::PickANumber:
            result = pickANumber.play(input);
            break;

        case GameType::DealerNoMatch:
            result = dealerNoMatch.play();
            break;

        default:
            return GameResult::InvalidInput;
    }

    switch (result)
    {
        case GameResult::Win:
            std::cout << "You Win!\n";
            break;

        case GameResult::Lose:
            std::cout << "You Lose!\n";
            break;

        case GameResult::InvalidInput:
            std::cout << "Invalid input.\n";
            break;

        case GameResult::InvalidWager:
            std::cout << "Invalid wager.\n";
            break;
    }

    settleWager(result);

    return result;
}


// ========================================
// FIND THE ACE - MULTI-STEP GAME
// ========================================

bool GameEngine::startFindTheAce(int initialWager)
{
    // Do not overwrite an unfinished round.
    FindTheAceState currentState = findTheAce.get_state();

    if (currentState != FindTheAceState::NotStarted &&
        currentState != FindTheAceState::Resolved)
    {
        return false;
    }

    if (initialWager <= 0 ||
        initialWager > player.get_credits())
    {
        return false;
    }

    selectedGame = GameType::FindTheAce;

    findTheAce.start(initialWager);

    return true;
}


bool GameEngine::selectFindTheAceCard(int choice)
{
    if (selectedGame != GameType::FindTheAce)
        return false;

    return findTheAce.selectInitialCard(choice);
}


bool GameEngine::changeFindTheAceSelection()
{
    if (selectedGame != GameType::FindTheAce)
        return false;

    return findTheAce.changeSelection();
}


bool GameEngine::increaseFindTheAceWager(int amount)
{
    if (selectedGame != GameType::FindTheAce)
        return false;

    int availableCredits =
        player.get_credits() - findTheAce.get_initial_wager();

    if (amount <= 0 || amount > availableCredits)
        return false;

    return findTheAce.increaseWager(amount);
}


GameResult GameEngine::finishFindTheAce()
{
    if (selectedGame != GameType::FindTheAce)
        return GameResult::InvalidInput;

    GameResult result = findTheAce.finish();

    if (result == GameResult::Win ||
        result == GameResult::Lose)
    {
        settleFindTheAceWager(result);
    }

    return result;
}


// ========================================
// WAGER SETTLEMENT
// ========================================

void GameEngine::settleWager(GameResult result)
{
    if (result == GameResult::Win)
    {
        player.add_credits(wager);
    }
    else if (result == GameResult::Lose)
    {
        player.deduct_credits(wager);
    }
}


void GameEngine::settleFindTheAceWager(GameResult result)
{
    int totalWager =
        findTheAce.get_initial_wager() +
        findTheAce.get_additional_wager();

    if (result == GameResult::Win)
    {
        player.add_credits(totalWager);
    }
    else if (result == GameResult::Lose)
    {
        player.deduct_credits(totalWager);
    }
}

FindTheAceState GameEngine::getFindTheAceState() const
{
    return findTheAce.get_state();
}

int GameEngine::getFindTheAceSelectedPosition() const
{
    return findTheAce.get_selected_position();
}

int GameEngine::getFindTheAceRevealedQueenPosition() const
{
    return findTheAce.get_revealed_queen_position();
}

int GameEngine::getFindTheAceInitialWager() const
{
    return findTheAce.get_initial_wager();
}

int GameEngine::getFindTheAceAdditionalWager() const
{
    return findTheAce.get_additional_wager();
}
