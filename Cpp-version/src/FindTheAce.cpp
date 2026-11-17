#include "FindTheAce.h"

FindTheAce::FindTheAce()
    : generator(std::random_device{}()),
      distribution(0, 2),
      ace_position(-1),
      selected_position(-1),
      revealed_queen_position(-1),
      initial_wager(0),
      additional_wager(0),
      state(FindTheAceState::NotStarted)
{
}

void FindTheAce::start(int wager)
{
    initial_wager = wager;
    additional_wager = 0;

    ace_position = distribution(generator);

    selected_position = -1;
    revealed_queen_position = -1;

    state = FindTheAceState::WaitingForInitialPick;
}

bool FindTheAce::selectInitialCard(int choice)
{
    if (state != FindTheAceState::WaitingForInitialPick)
        return false;

    if (choice < 1 || choice > 3)
        return false;

    // Convert card number 1-3 into position 0-2.
    selected_position = choice - 1;

    // Find a card that is neither the player's choice nor the Ace.
    for (int i = 0; i < 3; i++)
    {
        if (i != selected_position && i != ace_position)
        {
            revealed_queen_position = i;
            break;
        }
    }

    state = FindTheAceState::WaitingForDecision;

    return true;
}

bool FindTheAce::changeSelection()
{
    if (state != FindTheAceState::WaitingForDecision)
        return false;

    // There is only one other hidden card.
    for (int i = 0; i < 3; i++)
    {
        if (i != selected_position && i != revealed_queen_position)
        {
            selected_position = i;
            break;
        }
    }

    state = FindTheAceState::Finished;

    return true;
}

bool FindTheAce::increaseWager(int amount)
{
    if (state != FindTheAceState::WaitingForDecision)
        return false;

    if (amount <= 0)
        return false;

    additional_wager = amount;

    state = FindTheAceState::Finished;

    return true;
}

GameResult FindTheAce::finish()
{
    if (state != FindTheAceState::Finished)
        return GameResult::InvalidInput;

    GameResult result;

    if (selected_position == ace_position)
        result = GameResult::Win;
    else
        result = GameResult::Lose;

    state = FindTheAceState::Resolved;

    return result;
}

FindTheAceState FindTheAce::get_state() const
{
    return state;
}

int FindTheAce::get_selected_position() const
{
    return selected_position;
}

int FindTheAce::get_revealed_queen_position() const
{
    return revealed_queen_position;
}

int FindTheAce::get_initial_wager() const
{
    return initial_wager;
}

int FindTheAce::get_additional_wager() const
{
    return additional_wager;
}
