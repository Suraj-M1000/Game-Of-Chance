#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "player.h"
#include "storage.h"
#include "input.h"
#include "Custom_function.h"

#define DATAFILE "data/chance.data"

int get_player_data()
{
    int fd;                      // fd: file descriptor - In C, when file open with open(), the operating system returns an integer representing that opened file.
    int uid;                     // Holds the current operating-system user ID
    ssize_t read_bytes;          // Stores how many bytes were read
    struct player_record record; // Temporarily holds a player record read from the file

    uid = getuid(); // Unix/Linux system function that returns the user ID of the process's real user.

    fd = open(DATAFILE, O_RDONLY); // DATAFILE : File path, O_RDONLY : Open for reading only
                                   //    O_WRONLY: write only
                                   //    O_RDWR — read and write
    if (fd == -1)
    { //  Can't open the file, maybe it doesn't exist
        return -1;
    }
    read_bytes = read(fd, &record, sizeof(record)); // fd tells Which opened file to read, &entry tells Memory address where the data should be stored
                                                    // sizeof(struct user) tells how much	Number of bytes requested.
    // This loop is for Searching for the correct player
    while (record.uid != uid && read_bytes == (ssize_t)sizeof(record))
    {
        read_bytes = read(fd, &record, sizeof(record));
    }
    close(fd);
    if (read_bytes != (ssize_t)sizeof(record)) // This means that the end of file was reached.
        return -1;

    player.uid = record.uid;
    player.credits = record.credits;
    player.highscore = record.highscore;
    strcpy(player.name, record.name); // Copy the read entry into the player struct.
    return 1;                         // Return a success.
}

// This is the new user registration function.
// It will create a new player account and append it to the file
void register_new_player()
{
    int fd; // file descriptor.
    ssize_t written;
    struct player_record record;

    printf("-=-={ New Player Registration }=-=-\n");
    printf("Enter your name: ");
    input_name();

    player.uid = getuid();
    player.highscore = player.credits = 100;

    /* Copy only persistent data into the record */
    record.uid = player.uid;
    record.credits = player.credits;
    record.highscore = player.highscore;
    strcpy(record.name, player.name);

    fd = open(DATAFILE, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR); // S_IRUST - File owner can read, S_IWUSR	- File owner can write
    if (fd == -1)
        fatal("in register_new_player() while opening file");

    written = write(fd, &record, sizeof(record));

    if (written != (ssize_t)sizeof(record))
    {
        close(fd);
        fatal("in register_new_player() while writing file");
    }
    printf("\nWelcome to the Game of Chance %s.\n", player.name);
    printf("You have been given %u credits.\n", player.credits);
}

void update_player_data()
{
    int fd, read_uid; // fd : file descriptor, read_uid is for UID read from the file,
    struct player_record record;
    ssize_t read_bytes;
    ssize_t written;

    fd = open(DATAFILE, O_RDWR);

    if (fd == -1) // If open fails here, something is really wrong.
        fatal("in update_player_data() while opening file");

    while (1)
    {
        read_bytes = read(fd, &record, sizeof(record));

        if (read_bytes == 0)
        {
            close(fd);
            fatal("Player record not found");
        }
        if (read_bytes == -1)
        {
            close(fd);
            fatal("in update_player_data() while reading file");
        }
        if (read_bytes != (ssize_t)sizeof(record))
        {
            close(fd);
            fatal("Corrupted player record");
        }
        read_uid = record.uid;
        if (read_uid == player.uid)
            break;
    }
    /* Move file position back to the beginning of this record */
    lseek(fd, -(off_t)sizeof(record), SEEK_CUR);

    /* Update record from current player state */
    record.uid = player.uid;
    record.credits = player.credits;
    record.highscore = player.highscore;
    strcpy(record.name, player.name);

    written = write(fd, &record, sizeof(record));
    if (written != (ssize_t)sizeof(record))
    {
        close(fd);
        fatal("in update_player_data() while writing file");
    }
    close(fd);
}
