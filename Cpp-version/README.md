# Game of Chance — C++ Version

A C++17 desktop game application with a graphical user interface developed using Raylib. This version is a redesign of the original C command-line implementation.

## Overview

The C++ version of Game of Chance was developed to extend the original project using object-oriented programming and graphical application development.

Rather than translating the C implementation line by line, this version was redesigned using classes, encapsulation, composition, and separation between the game backend and graphical interface.

The application allows players to register, log in, play three games, manage credits, view their profile, and retain player information between application runs.

## Features

- Player registration and login using a player UID.
- Player credit and high-score management.
- Three playable games:
  - **Pick A Number** — Guess the randomly generated number.
  - **Dealer No Match** — Win by generating a collection of numbers without duplicates.
  - **Find The Ace** — Select a card and decide whether to change the selection or increase the wager.
- Game-specific wager handling and result settlement.
- Player profile and session management.
- Local file-based player persistence.
- Raylib-based graphical interface.
- Interactive buttons, input fields, and screen navigation.

## Technologies Used

| Technology | Purpose |
|---|---|
| C++17 | Application and game logic |
| Raylib | Graphical user interface |
| GCC / G++ | Compilation |
| MSYS2 UCRT64 | Windows development environment |
| C++ Standard Library | Containers, random number generation, strings, smart pointers, and file streams |
| Text files | Player data persistence |

## Application Architecture

The application separates graphical presentation from the game backend.

```text
Raylib GUI
    │
    ▼
GuiApplication
    │
    ▼
PlayerSession
    │
    ├── Player
    │
    └── GameEngine
          │
          ├── PickANumber
          ├── DealerNoMatch
          └── FindTheAce

Storage
    │
    ▼
data/players.txt
```

### Main Components

| Component | Responsibility |
|---|---|
| `GuiApplication` | Handles GUI screens, user interactions, and application navigation |
| `GuiStyle` | Contains reusable GUI styling and drawing functions |
| `Player` | Maintains player information, credits, and high score |
| `PlayerSession` | Manages the active player session and owns the game engine |
| `GameEngine` | Handles game selection, wager validation, gameplay coordination, and credit settlement |
| Game classes | Implement the individual game logic |
| `Storage` | Saves and loads player information |

The GUI communicates with the backend through `PlayerSession` and `GameEngine`. The backend does not depend on Raylib.

## Project Structure

```text
Cpp-version/
│
├── README.md
│
├── data/
│   └── players.txt
│
├── Gui/
│   ├── GuiApplication.cpp
│   ├── GuiApplication.h
│   ├── GuiStyle.cpp
│   ├── GuiStyle.h
│   └── main.cpp
│
├── include/
│   ├── Application.h
│   ├── DealerNoMatch.h
│   ├── FindTheAce.h
│   ├── GameEngine.h
│   ├── GameResult.h
│   ├── PickANumber.h
│   ├── Player.h
│   ├── PlayerSession.h
│   └── Storage.h
│
└── src/
    ├── Application.cpp
    ├── DealerNoMatch.cpp
    ├── FindTheAce.cpp
    ├── GameEngine.cpp
    ├── main.cpp
    ├── PickANumber.cpp
    ├── Player.cpp
    ├── PlayerSession.cpp
    └── Storage.cpp
```

## Build and Run

### Requirements

- Windows
- MSYS2 UCRT64 environment
- GCC/G++ compiler
- C++17 support
- Raylib installed for the graphical application

### 1. Console Application

Open the MSYS2 UCRT64 terminal or a configured terminal and navigate to the `Cpp-version` directory.

Compile:

```bash
g++ -Wall -Wextra -std=c++17 \
-I include \
src/main.cpp \
src/Application.cpp \
src/Player.cpp \
src/PlayerSession.cpp \
src/GameEngine.cpp \
src/PickANumber.cpp \
src/DealerNoMatch.cpp \
src/FindTheAce.cpp \
src/Storage.cpp \
-o game.exe
```

Run:

```bash
./game.exe
```

### 2. Graphical Application

Compile the Raylib GUI application:

```bash
g++ -Wall -Wextra -std=c++17 \
-I include \
-I Gui \
Gui/main.cpp \
Gui/GuiApplication.cpp \
Gui/GuiStyle.cpp \
src/Player.cpp \
src/PlayerSession.cpp \
src/GameEngine.cpp \
src/PickANumber.cpp \
src/DealerNoMatch.cpp \
src/FindTheAce.cpp \
src/Storage.cpp \
-o gui.exe \
-IC:/msys64/ucrt64/include \
-LC:/msys64/ucrt64/lib \
-lraylib \
-lopengl32 \
-lgdi32 \
-lwinmm
```

Run:

```bash
./gui.exe
```

Run the applications from the `Cpp-version` directory so that the relative path to `data/players.txt` resolves correctly.

## Data Persistence

Player information is stored locally in:

```text
data/players.txt
```

The stored record contains:

```text
UID|Name|Credits|Highscore
```

The application uses file-based persistence to load player information and save updated player records.

## Testing

The application was tested through functional and edge-case testing.

Testing covered:

- Player registration and login.
- Wager validation.
- Individual game logic.
- Find The Ace state transitions.
- Credit updates and wager settlement.
- Session handling.
- GUI navigation and interactions.

All seven major edge-case testing groups passed during the final system test.

## AI Assistance & Acknowledgement

AI tools, including **ChatGPT and Claude**, were used as development assistants throughout the C++ implementation.

Their assistance was particularly significant in the design and implementation of the Raylib graphical user interface and its integration with the backend. AI assistance was also used for selected backend components, debugging, architectural discussions, and code review.

The development involved iterative implementation, experimentation, testing, and refinement with AI assistance.

Some implementation details remain areas for further personal study and understanding. Further code review and improvements are part of the project's continued learning journey.

## Known Limitations

- Player login uses a UID-based mechanism and is not a production authentication system.
- Player data is stored locally in a text file rather than a database.
- The application is an educational and portfolio project, not a production gambling platform.

## Future Improvements

Potential improvements include:

- Stronger player authentication.
- More robust data storage.
- Additional games and gameplay features.
- Further code review and architectural refinement.
- Expanded automated testing.
