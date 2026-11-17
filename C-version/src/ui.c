#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "ui.h"
#include "player.h"
#include "Custom_function.h"

#define DATAFILE "data/chance.data" // File to store user data .This program uses this location to store player information.


void show_highscore(){
    int top_score = 0;
    char top_name[100];
    struct user entry;
    int fd;

    printf("\n====================| HIGH SCORE |====================\n");
    fd = open(DATAFILE, O_RDONLY);
    if(fd == -1)
        fatal("in show_highscore() while opening file");

    while (read(fd, &entry, sizeof(struct user)) > 0){                   // Loop until end of file.
        if (entry.highscore > top_score){
            top_score = entry.highscore;
            strcpy(top_name, entry.name);
        }
    }
    close(fd);
    if(top_score > player.highscore)
        printf("%s has the high score of %u\n", top_name, top_score);
    else
        printf("You currently have the high score of %u credits!\n", player.highscore);
    printf("======================================================\n\n");
}


void print_cards(char *message, char *cards, int user_pick){    // message - C string, cards - address of the first index value, user_pick - user prefernce
    int i;

    printf("\n\t*** %s ***\n", message);
    printf("      \t._.\t._.\t._.\n");
    printf("Cards:\t|%c|\t|%c|\t|%c|\n\t", cards[0], cards[1], cards[2]);
    if (user_pick == -1){
        printf(" 1 \t 2 \t 3\n");
    }else {
        for(i=0; i < user_pick; i++)
        printf("\t");
      printf(" ^-- your pick\n");
   }
}
