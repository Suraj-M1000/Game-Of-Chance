#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include "input.h"
#include "player.h"
#include <string.h>

// Buffer Overflow Free input function

void input_name(void)
{
    char *name_ptr = player.name; // name_ptr = player name's address
    int input_char;               // input_char : store the Decimal value of the char
    size_t count = 0;             // count is use to check does the player.name is getting overflow or not.

    while ((input_char = getchar()) != '\n' && input_char != EOF)
    { // condition check for is the input value is not '\n' or eof
        if (count < sizeof(player.name) - 1)
        {                                       // checking to ensure there is not any buffer overflow.
            name_ptr[count] = (char)input_char; // if there is not any overflow than just add the char with the help of the ASCII table cause
                                                // due the the getchar() this store the decimal value of the char we have input..
            count++;
        }
    }
    name_ptr[count] = '\0';
}

//  -- Improve by AI(Claude + Chatgpt) -- //

/* Read one complete line from standard input. */
static int read_input_line(char *buffer, size_t size)
{
    size_t length;

    if (fgets(buffer, size, stdin) == NULL)
        return -1;

    length = strlen(buffer);

    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    }
    else if (!feof(stdin)) {
        int ch;

        /* Discard the rest of an overlong input line. */
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;

        return 0;
    }

    return 1;
}


int input_integer(const char *prompt, int *value)
{
    char buffer[100];
    char *endptr;
    long result;
    int status;

    printf("%s", prompt);

    status = read_input_line(buffer, sizeof(buffer));

    if (status != 1)
        return status;

    errno = 0;
    result = strtol(buffer, &endptr, 10);

    if (endptr == buffer || errno == ERANGE)
        return 0;

    while (isspace((unsigned char)*endptr))
        endptr++;

    if (*endptr != '\0' || result < INT_MIN || result > INT_MAX)
        return 0;

    *value = (int)result;

    return 1;
}

int input_character(const char *prompt, char *value)
{
    char buffer[100];
    char *ptr;
    int status;

    printf("%s", prompt);

    status = read_input_line(buffer, sizeof(buffer));

    if (status != 1)
        return status;

    ptr = buffer;

    while (isspace((unsigned char)*ptr))
        ptr++;

    if (*ptr == '\0')
        return 0;

    *value = *ptr;
    ptr++;

    while (isspace((unsigned char)*ptr))
        ptr++;

    if (*ptr != '\0')
        return 0;

    return 1;
}
