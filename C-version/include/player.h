#ifndef PLAYER_H
#define PLAYER_H

struct user
{
    int uid;
    int credits;
    int highscore;
    char name[100];        // till this all are the sturct members
    int (*current_game)(void); // this is not an normal variable but an Function pointer - this variable stores the address of a function.
                           //  int : this function return an integer value,
                           //  (*current_game) : current_game is a pointer.
                           //  () : The pointed-to function takes unspecified arguments in this old C declaration.
};


/* Data that is actually stored in the file */
struct player_record {    // This sturcture is use for the storing the date while registing the user
    int uid;
    int credits;
    int highscore;
    char name[100];
};

extern struct user player;

#endif
