#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Player.h"
#include "PickANumber.h"
#include "DealerNoMatch.h"
#include "FindTheAce.h"
#include "GameResult.h"



enum class GameType      // Scope Enum  -- enumeration is a user-defined data type that consists of a set of named integer constants. helps assigning name to integer.
{
    PickANumber,        // it is assign as 0
    DealerNoMatch,      // it is assign as 1
    FindTheAce          // it is assign as 2
};

class GameEngine{
    private:
        Player& player; // & mean reference - A reference is essentially another name/alias for an existing object.
                            //  GameEngine stores a reference to the Player that was given to it

        GameType selectedGame;    // Persistent GameEngine state.
        int wager;
        PickANumber pickANumber;
        DealerNoMatch dealerNoMatch;
        FindTheAce findTheAce;

        void settleWager(GameResult result);
        void settleFindTheAceWager(GameResult result);

    public:
        GameEngine(Player& player);   // The GameEngine constructor expects a reference to an existing Player.
        Player& get_player();


        void set_game(GameType game);
        GameType get_game() const;

        bool set_wager(int wager);
        int get_wager() const;

         // Single-action games
        GameResult playGame(int input);

        // Multi-step Find the Ace
        bool startFindTheAce(int initialWager);
        bool selectFindTheAceCard(int choice);
        bool changeFindTheAceSelection();
        bool increaseFindTheAceWager(int amount);
        GameResult finishFindTheAce();

        FindTheAceState getFindTheAceState() const;

        int getFindTheAceSelectedPosition() const;

        int getFindTheAceRevealedQueenPosition() const;

        int getFindTheAceInitialWager() const;

        int getFindTheAceAdditionalWager() const;

};



#endif
