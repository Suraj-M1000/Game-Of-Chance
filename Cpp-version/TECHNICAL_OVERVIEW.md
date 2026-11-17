# Technical Overview — Game of Chance (C++)

## 1. Application Architecture

The C++ application separates the graphical interface, player management, game logic, and data persistence.

```text
             GUI Application
                    |
                    v
              PlayerSession
                    |
             +------+------+
             |             |
           Player       GameEngine
                           |
                 +---------+---------+
                 |         |         |
              Pick A    Dealer    Find The
              Number   No Match     Ace

        Application Layer
                 |
                 v
               Storage
                 |
                 v
          data/players.txt
```

### Main Components

- **GuiApplication:** Handles graphical screens, navigation, and user interactions.
- **GuiStyle:** Provides reusable GUI styling and drawing functions.
- **Player:** Maintains player information, credits, and high score.
- **PlayerSession:** Owns the active player and associated game engine.
- **GameEngine:** Coordinates game selection, wager validation, gameplay, and credit settlement.
- **Game Classes:** Implement the individual game rules.
- **Storage:** Handles loading and saving player records.

The console and GUI applications use the shared backend rather than maintaining separate game implementations.

## 2. Gameplay Flow

The application follows this general flow:

```text
Launch Application
       |
       v
Register / Login
       |
       v
Create Player Session
       |
       v
Main Menu
       |
       v
Select Game
       |
       v
Enter Wager
       |
       v
Play Game
       |
       v
Calculate Result
       |
       v
Update Player Credits
       |
       v
Return to Menu / Logout
```

### Find The Ace

Find The Ace uses a state-based gameplay flow:

```text
NotStarted
    |
    v
WaitingForInitialPick
    |
    v
WaitingForDecision
    |
    +------ Change Selection ------+
    |                              |
    +------ Increase Wager --------+
                   |
                   v
                Finished
                   |
                   v
                 Resolved
```

The game maintains its internal state, selected card, revealed queen position, and wager information.

## 3. Player Data Persistence

Player information is stored locally in:

`data/players.txt`

The record format is:

```text
UID|Name|Credits|Highscore
```

The `Storage` class provides static operations to save and load player records.

The current implementation uses append-based storage. When loading a player, the application scans records and uses the latest valid matching record.

Persistent player information is kept separate from runtime application state.

## 4. Design Approach

The C++ implementation uses the following design concepts:

- **Encapsulation:** Player data and game state are managed through classes.
- **Composition:** The player session owns a player and a game engine.
- **RAII:** Resource ownership is managed using C++ object lifetimes and smart pointers where appropriate.
- **Separation of Concerns:** GUI presentation is separated from game logic.
- **Standard Library:** Uses containers, strings, random number generation, file streams, and smart pointers.

The project uses a shared backend for both console and graphical application interfaces.

## 5. Development Scope

This is an educational and portfolio project developed to explore C++ application design and graphical development.

It is not intended to be a production gambling platform or a production authentication system.
