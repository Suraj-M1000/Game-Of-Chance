#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "input.h"
#include "player.h"
#include "ui.h"
#include "storage.h"
#include "Custom_function.h"

// This function simply awards the jackpot for the Pick a Number game.
void jackpot()
{
    printf("*+*+*+*+*+* JACKPOT *+*+*+*+*+*\n");
    printf("You have won the jackpot of 100 credits!\n");
    player.credits += 100;
}

int take_wager(int available_credits, int previous_wager)
{
    int wager;
    int total_wager;
    int status;

    status = input_integer(
        "How many credits would you like to wager? ",
        &wager
    );

    if (status == -1)
        return -2;

    if (status == 0) {
        printf("Invalid input. Please enter a valid number.\n");
        return -1;
    }

    if (wager < 1) {
        printf("You must wager a positive number!\n");
        return -1;
    }

    total_wager = previous_wager + wager;

    if (total_wager > available_credits) {
        printf("Your total wager of %d is more than you have!\n",
               total_wager);
        printf("You only have %d available credits.\n",
               available_credits);
        return -1;
    }

    return wager;
}
void play_the_game(void)
{
    int play_again = 1; // this controls the while loop.
    char selection;     // This stores the player's response when asked whether they want to play again.

    while (play_again)
    {
        // printf("\n[DEBUG] current_game pointer @ 0x%08x\n", player.current_game);
        if (player.current_game() != -1)
        {
            if (player.credits > player.highscore)
            {
                player.highscore = player.credits;
            }
            printf("\nYou now have %u credits\n", player.credits);
            update_player_data();

            // printf("Would you like to play again? (y/n)  ");
            // selection = '\n';
            // while (selection == '\n'){
            //     scanf("%c", &selection);
            // }

            int status;

            while (1)
            {
                status = input_character(
                    "Would you like to play again? (y/n): ",
                    &selection);

                if (status == -1)
                {
                    play_again = 0;
                    break;
                }

                if (status == 0 ||
                    (selection != 'y' && selection != 'n'))
                {
                    printf("Please enter only y or n.\n");
                    continue;
                }

                if (selection == 'n')
                    play_again = 0;

                break;
            }
        }
        else
        {
            play_again = 0;
        }
    }
}

// Games Function
int pick_a_number(void)
{
    int pick;
    int winning_number;
    int status;

    printf("\n####### Pick a Number ######\n");
    printf("This game costs 10 credits to play.\n");
    printf("Pick a number between 1 and 20.\n\n");

    if (player.credits < 10) {
        printf("You don't have enough credits to play!\n");
        return -1;
    }

    winning_number = (rand() % 20) + 1;

    while (1) {
        status = input_integer("Pick a number between 1 and 20: ",
                               &pick);

        if (status == -1)
            return -1;

        if (status == 0 || pick < 1 || pick > 20) {
            printf("Invalid selection. Choose a number from 1 to 20.\n");
            continue;
        }

        break;
    }

    player.credits -= 10;

    printf("10 credits have been deducted.\n");
    printf("The winning number is %d\n", winning_number);

    if (pick == winning_number)
        jackpot();
    else
        printf("Sorry, you didn't win.\n");

    return 0;
}


int dealer_no_match()
{
    int i, j, numbers[16], wager = -1, match = -1;

    printf("\n::::::: No Match Dealer :::::::\n");
    printf("In this game, you can wager up to all of your credits.\n");
    printf("The dealer will deal out 16 random numbers between 0 and 99.\n");
    printf("If there are no matches among them, you double your money!\n\n");

    if (player.credits == 0)
    {
        printf("You don't have any credits to wager!\n\n");
        return -1;
    }
    while (wager == -1)
    {
        wager = take_wager(player.credits, 0);
        if (wager == -2)
        return -1;
    }
    printf("\t\t::: Dealing out 16 random numbers :::\n");
    for (int i = 0; i < 16; i++)
    {
        numbers[i] = rand() % 100; // Pick a number between 0 and 99.
        printf("%2d\t", numbers[i]);
        if (i % 8 == 7)
        {
            printf("\n");
        }
    }
    for (i = 0; i < 15; i++)
    {              // let say i = 1  ;
        j = i + 1; // then j will be 1 + 1 = 2;
        while (j < 16)
        {
            if (numbers[i] == numbers[j])
            {
                match = numbers[i];
            }
            j++;
        }
    }
    if (match != -1)
    {
        printf("The dealer matched the number %d!\n", match);
        printf("You lose %d credits.\n", wager);
        player.credits -= wager;
    }
    else
    {
        printf("There were no matches! You win %d credits!\n", wager);
        player.credits += wager;
    }
    return 0;
}


int find_the_ace()
{
    int i, ace;                                                    // here i is use for the iteration, ace is use to store the value which tell that where exactly is ace card place going to be.
    int invalid_choice, pick = -1, wager_one = -1, wager_two = -1; // invalid_choice : check that use made while this function is valid or not. pick going to store the value that player think the ace card will be at
                                                                   // wager_one : this going to store the fiirst wage value that player bet at the start of this game,
                                                                   // wager_two: is going to store the wager value if user lose the first round and want to add more wage
    char choice_two, cards[3] = {'X', 'X', 'X'};                   // choice_two: use to store the decision value of the player (want to add more wager or piick the card)

    ace = rand() % 3; // Place the ace randomly.                        // use to randomly generate the value between 0 - 2 which tell where the ace card going to be.
    printf("******* Find the Ace *******\n");
    printf("In this game, you can wager up to all of your credits.\n");
    printf("Three cards will be dealt out, two queens and one ace.\n");
    printf("If you find the ace, you will win your wager.\n");
    printf("After choosing a card, one of the queens will be revealed.\n");
    printf("At this point, you may either select a different card or\n");
    printf("increase your wager.\n\n");

    if (player.credits == 0)
    { // check whether player is poor or not
        printf("You don't have any credits to wager!\n\n");
        return -1;
    }

    while (wager_one == -1){ // Loop until valid wager is made.       // taking the first wage on which the user want to bet
        wager_one = take_wager(player.credits, 0);
        if (wager_one  == -2)
        return -1;
    }


    print_cards("Dealing cards", cards, -1); // this run the print card function which initally all card going to be X which mean none of the card has been choosen and has to be choose
    pick = -1;

    while ((pick < 1) || (pick > 3))
    { // this while is use to get the pick the card value like which card do u think going to be the ace card.
        printf("Select a card: 1, 2, or 3  ");
        scanf("%d", &pick);
    }

    pick--; // Adjust the pick since card numbering starts at 0.     // ok why this? see the pick = -1 and let say user thinks that 2 card hold the ace card. so pick will be 2 but it should be 1 cause it wwas -1...so that why subtracting it by 1(--)

    i = 0;
    while (i == ace || i == pick)
    { // Start at card 0 and keep moving forward until I find a card that is neither the Ace nor the player's selected card.
        i++;
    }
    cards[i] = 'Q';
    print_cards("Revealing a queen", cards, pick);

    invalid_choice = 1;

    while (invalid_choice)
    { // Continue while invalid_choice is non-zero.
        printf("Would you like to:\n[c]hange your pick\tor\t[i]ncrease your wager?\n");
        printf("Select c or i:  ");
        choice_two = '\n';
        while (choice_two == '\n')    // Flush extra newlines.
            scanf("%c", &choice_two); // getting the value to decide what user want to pick or increase tha wage

        if (choice_two == 'i')
        { // got this one
            invalid_choice = 0;
            while (wager_two == -1){ // Loop until valid second wager is made.
                wager_two = take_wager(player.credits, wager_one);
                if (wager_two == -2)
                    return -1;
            }
        }
        if (choice_two == 'c')
        { // but not this one
            i = 0;
            invalid_choice = 0;
            while (i == pick || cards[i] == 'Q')
            {
                i++;
            }
            pick = i;
            printf("Your card pick has been changed to card %d\n", pick + 1);
        }
    }
    for (i = 0; i < 3; i++)
    { // Reveal all of the cards.
        if (ace == i)
            cards[i] = 'A';
        else
            cards[i] = 'Q';
    }
    print_cards("End result", cards, pick);

    if (pick == ace)
    { // Handle win.
        printf("You have won %d credits from your first wager\n", wager_one);
        player.credits += wager_one;
        if (wager_two != -1)
        {
            printf("and an additional %d credits from your second wager!\n", wager_two);
            player.credits += wager_two;
        }
    }
    else
    { // Handle loss.
        printf("You have lost %d credits from your first wager\n", wager_one);
        player.credits -= wager_one;
        if (wager_two != -1)
        {
            printf("and an additional %d credits from your second wager!\n", wager_two);
            player.credits -= wager_two;
        }
    }
    return 0;
}
