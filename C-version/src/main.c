
// Header Files.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Custom_function.h"
#include "player.h"
#include "storage.h"
#include "game.h"
#include "input.h"
#include "ui.h"

int main()
{
    int choice = 0;    // choice that user want.
    int last_game = 0; // last game refer to the previous game selected.

    srand(time(0));

    if (get_player_data() == -1)
    {
        register_new_player();
    }

    while (choice != 7)
    {
        printf("--=[ Game of Chance Menu ]=--\n");
        printf("1 - Play the Pick a Number game\n");
        printf("2 - Play the No Match Dealer game\n");
        printf("3 - Play the Find the Ace game\n");
        printf("4 - View current high score\n");
        printf("5 - Change your user name\n");
        printf("6 - Reset your account at 100 credits\n");
        printf("7 - Quit\n");
        printf("[Name: %s]\n", player.name);
        printf("[You have %d credits] ->  ", player.credits);

        // improved one
        int input_status;
        input_status = input_integer("Select an option: ", &choice);

        if (input_status == -1)
            break;
        if (input_status == 0)
        {
            printf("\n[!!] Invalid input. Please enter a number.\n\n");
            continue;
        }
        if ((choice < 1) || (choice > 7))
        {
            printf("\n[!!] The number %d is an invalid selection.\n\n", choice);
        }
        else if (choice < 4)
        {
            if (choice != last_game)
            {
                if (choice == 1) // then point it at the selected game
                    player.current_game = pick_a_number;
                else if (choice == 2)
                    player.current_game = dealer_no_match;
                else
                    player.current_game = find_the_ace;
                last_game = choice;
            }
            play_the_game();
        }
        else if (choice == 4)
        {
            show_highscore();
        }
        else if (choice == 5)
        {
            printf("\nChange user name\n");
            printf("Enter your new name: ");
            input_name();
            printf("Your name has been changed.\n\n");
        }
        else if (choice == 6)
        {
            printf("\nYour account has been reset with 100 credits.\n\n");
            player.credits = 100;
        }
    }
    update_player_data();
    printf("\nThanks for playing! Bye.\n");
    return 0;
}
