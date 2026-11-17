# Game of Chance --- C Version

A Linux/POSIX command-line game developed in C. This version focuses on
modular program design, low-level file handling, runtime function
pointers, and practical security improvements.

## Overview

The C implementation provides a terminal-based **Game of Chance**
application. A player can register an account, play three games, manage
their credits, view the high score, and retain account data between
application runs.

This is the standalone CLI implementation. The separate C++ version is
intended to be a redesigned GUI application; it is not part of this C
version.

## Features

-   Player registration using the current Linux user ID.
-   Persistent player information, including name, credits, and high
    score.
-   Three playable games:
    -   **Pick a Number** --- choose a number and try to win the
        jackpot.
    -   **No Match Dealer** --- wager credits on a set of randomly dealt
        numbers containing no matches.
    -   **Find the Ace** --- choose between three cards, with an
        opportunity to change the selection or increase the wager.
-   Credit and wager handling.
-   High-score display.
-   Change player name and reset account credits.
-   Save player progress so it can be loaded on a later run.

## Project Structure

``` text
C-version/
├── data/
│   └── chance.data
├── include/
│   ├── Custom_function.h
│   ├── game.h
│   ├── input.h
│   ├── player.h
│   ├── storage.h
│   └── ui.h
├── src/
│   ├── Custom_function.c
│   ├── game.c
│   ├── input.c
│   ├── main.c
│   ├── player.c
│   ├── storage.c
│   └── ui.c
├── Makefile
└── game
```

### Module responsibilities

  -----------------------------------------------------------------------
  Module                              Responsibility
  ----------------------------------- -----------------------------------
  `main.c`                            Application flow, menu handling,
                                      and game selection

  `player.c` / `player.h`             Runtime player state and its
                                      declaration

  `game.c` / `game.h`                 Game logic, wagers, and
                                      game-related operations

  `storage.c` / `storage.h`           Loading, registering, and updating
                                      player records

  `input.c` / `input.h`               Player name and validated user
                                      input

  `ui.c` / `ui.h`                     Terminal presentation, cards, and
                                      high-score display

  `Custom_function.c` /               Shared utility functions, including
  `Custom_function.h`                 error handling and allocation
                                      support

  `data/`                             Local persistent player data
  -----------------------------------------------------------------------

The headers contain declarations and interfaces, while the corresponding
source files contain the implementations. This keeps the application
separated into focused modules rather than placing all functionality in
one large source file.

## Design and Implementation

### Modular C design

The original single-file program was reorganized into separate modules
for the application flow, player state, game logic, input, storage, and
terminal presentation. Header guards and declarations help define the
interfaces between these modules.

### Linux/POSIX system programming

The application uses Linux/POSIX facilities for user identification and
file operations. Player records are stored locally in
`data/chance.data`.

The storage module uses file descriptors and low-level operations such
as `open()`, `read()`, `write()`, `lseek()`, and `close()` to manage
player data.

### Persistent data separated from runtime state

The runtime player structure contains application state, including the
selected game function pointer. The persistent `player_record` structure
contains only the fields that need to be saved: user ID, credits, high
score, and player name.

This separation avoids writing runtime-only data, such as a function
pointer, into the data file. When a player record is loaded, its
persistent fields are copied into the runtime player state.

### Function pointers for game selection

The application uses a function pointer to select and invoke one of the
known game functions. The pointer is assigned by the program based on
the player's menu selection; it is runtime state rather than persisted
player data.

## Security Review and Improvements

The C implementation was reviewed and improved with attention to common
risks in low-level C programs.

-   **Bounded name input:** name input was changed to avoid writing
    beyond the fixed-size player name buffer.
-   **Input validation:** unchecked numeric and character input
    operations were reviewed and replaced with validated input handling.
    Numeric input is parsed and checked before being used by the
    application.
-   **File-operation checks:** file opening, reading, and writing are
    checked for errors, and file descriptors are closed on the handled
    error paths.
-   **Persistent record design:** the on-disk record was separated from
    the runtime player structure so runtime-only fields are not written
    to the file.
-   **Function-pointer review:** game function pointers are assigned to
    known functions in the program and are not loaded from the player
    data file.
-   **Randomness review:** the use of `rand()` and time-based seeding
    was reviewed for predictability.

These changes are intended as practical hardening for this educational
command-line application. They do not constitute a formal security
certification or a guarantee that the program is free of
vulnerabilities.

## Build and Run

Run these commands from the `C-version` directory in a Linux environment
with GCC installed:

``` bash
gcc -Wall -Wextra -I include src/*.c -o game
./game
```

If using the provided Makefile, you can also build with:

``` bash
make
```

Then run:

``` bash
./game
```

The application stores its player data under `data/chance.data`. Run it
from the `C-version` directory so the relative data-file path resolves
as expected.

## What This Version Demonstrates

This C implementation demonstrates:

-   Procedural programming and modular C project organization.
-   Linux/POSIX file handling using file descriptors.
-   Runtime function pointers for selecting game functions.
-   Separation of persistent records from runtime application state.
-   Input validation and bounded buffer handling.
-   Basic security review and hardening of a C application.
-   A working command-line application with persistent player progress.

## Project Direction

The C version is the completed command-line implementation. A separate
C++ version is planned as a redesign of the same game, with an
object-oriented game engine and a graphical user interface. The C++
implementation will have its own architecture rather than being a
line-by-line translation of this code.
