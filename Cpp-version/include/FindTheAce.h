
#ifndef FINDTHEACE_H
#define FINDTHEACE_H

#include "GameResult.h"
#include <random>

enum class FindTheAceState
{
    NotStarted,
    WaitingForInitialPick,
    WaitingForDecision,
    WaitingForAdditionalWager,
    Finished,
    Resolved
};

class FindTheAce
{
private:
    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;

    int ace_position;
    int selected_position;
    int revealed_queen_position;

    int initial_wager;
    int additional_wager;

    FindTheAceState state;

public:
    FindTheAce();

    void start(int wager);

    bool selectInitialCard(int choice);

    bool changeSelection();

    bool increaseWager(int amount);

    GameResult finish();

    FindTheAceState get_state() const;

    int get_selected_position() const;
    int get_revealed_queen_position() const;

    int get_initial_wager() const;
    int get_additional_wager() const;
};

#endif
