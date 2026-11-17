#include "GuiApplication.h"
#include <string>
#include "Storage.h"
#include <stdexcept>
#include <utility>
#include "GuiStyle.h"

GuiApplication::GuiApplication(const std::string& filePath)
    : session(nullptr),
      dataFile(filePath),
      currentScreen(GuiScreen::Welcome)
{
}

void GuiApplication::run()
{
    InitWindow(900, 600, "Game of Chance");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(UI::Background);

        // These screens require an authenticated player
    bool requiresSession =
        currentScreen == GuiScreen::MainMenu ||
        currentScreen == GuiScreen::GameSelection ||
        currentScreen == GuiScreen::PickANumber ||
        currentScreen == GuiScreen::DealerNoMatch ||
        currentScreen == GuiScreen::FindTheAce ||
        currentScreen == GuiScreen::Profile;

    if (requiresSession && (!session || !session->is_active()))
    {
        currentScreen = GuiScreen::Welcome;
    }

        switch (currentScreen)
{
    case GuiScreen::Welcome:
        drawWelcome();
        break;

    case GuiScreen::Register:
        drawRegister();
        break;

    case GuiScreen::Login:
        drawLogin();
        break;

    case GuiScreen::MainMenu:
        drawMainMenu();
        break;

    case GuiScreen::GameSelection:
        drawGameSelection();
        break;

    case GuiScreen::PickANumber:
        drawPickANumber();
        break;

    case GuiScreen::DealerNoMatch:
        drawDealerNoMatch();
        break;

    case GuiScreen::FindTheAce:
        drawFindTheAce();
        break;

    case GuiScreen::Profile:
        drawProfile();
        break;
}

        EndDrawing();
    }

    // Save active player's data before application exits
if (session && session->is_active())
{
    if (!Storage::save_player(session->get_player(), dataFile))
    {
        TraceLog(
            LOG_ERROR,
            "Failed to save player data on application exit."
        );
    }
}
    CloseWindow();
}

void GuiApplication::drawMainMenu()
{
    // Defensive check: MainMenu requires an active session
    if (!session || !session->is_active())
    {
        currentScreen = GuiScreen::Welcome;
        return;
    }

    Rectangle panel = {220, 30, 520, 500};
    UI::drawPanel(panel);

    DrawText("GAME OF CHANCE", 300, 60, 40, UI::Text);

    DrawText(
        TextFormat("Welcome, %s",
            session->get_player().get_name().c_str()),
        320, 130, 25, UI::MutedText
    );

    DrawText(
        TextFormat("Credits: %d",
            session->get_player().get_credits()),
        320, 180, 22, UI::MutedText
    );

    DrawText(
        TextFormat("Highscore: %d",
            session->get_player().get_highscore()),
        320, 220, 22, UI::MutedText
    );

    // PLAY button
    Rectangle playButton = {350, 290, 200, 55};

    if (UI::drawButton(playButton, "PLAY", 25))
    {
        currentScreen = GuiScreen::GameSelection;
        return;
    }

    // PROFILE button
    Rectangle profileButton = {350, 370, 200, 50};

    if (UI::drawButton(profileButton, "PROFILE", 22))
    {
        currentScreen = GuiScreen::Profile;
        return;
    }

    // LOGOUT button
    Rectangle logoutButton = {350, 440, 200, 50};

    if (UI::drawButton(logoutButton, "LOGOUT", 22))
    {
        logoutPlayer();
        return;
    }


    if (!authMessage.empty())
    {
        DrawText(authMessage.c_str(), 250, 520, 18, UI::Danger);
    }
}




void GuiApplication::drawGameSelection()
{

    Rectangle panel = {220, 45, 510, 470};
    UI::drawPanel(panel);

    DrawText("SELECT YOUR GAME", 300, 70, 35, UI::Text);

    // Pick A Number
    Rectangle pickButton = {300, 160, 300, 55};

    if (UI::drawButton(pickButton, "PICK A NUMBER", 22))
    {
        GameEngine* engine = session->get_engine();

        if (engine != nullptr)
        {
            engine->set_game(GameType::PickANumber);

            wagerInput = "10";
            selectedNumber = 1;
            hasPlayed = false;
            resultMessage.clear();

            currentScreen = GuiScreen::PickANumber;
        }
    }

    // Dealer No Match
    Rectangle dealerButton = {300, 240, 300, 55};

    if (UI::drawButton(dealerButton, "DEALER NO MATCH", 22))
    {
        GameEngine* engine = session->get_engine();

        if (engine != nullptr)
        {
            engine->set_game(GameType::DealerNoMatch);

            wagerInput = "10";
            hasPlayed = false;
            resultMessage.clear();

            currentScreen = GuiScreen::DealerNoMatch;
        }
    }

    // Find The Ace
    Rectangle aceButton = {300, 320, 300, 55};

    if (UI::drawButton(aceButton, "FIND THE ACE", 22))
    {
        aceInitialWagerInput = "10";
        aceAdditionalWagerInput = "10";
        aceResultMessage.clear();

        currentScreen = GuiScreen::FindTheAce;
    }

    // Back button
    Rectangle backButton = {350, 430, 200, 45};

    if (UI::drawButton(backButton, "BACK", 20))
    {
        currentScreen = GuiScreen::MainMenu;
    }
}




void GuiApplication::drawPickANumber()
{

    Rectangle panel = {150, 85, 600, 485};
    UI::drawPanel(panel);

    DrawText("PICK A NUMBER", 320, 40, 35, UI::Text);

    // Get the backend engine
    GameEngine* engine = session->get_engine();

    if (engine == nullptr)
    {
        DrawText("Session is inactive.", 320, 280, 22, UI::Danger);
        return;
    }

    // Display player credits
    DrawText(
        TextFormat("Credits: %d", session->get_player().get_credits()),
        700, 30, 20, UI::Success
    );

    // ========================================
    // WAGER INPUT
    // ========================================

    DrawText("Enter your wager:", 200, 120, 22, UI::MutedText);

    Rectangle wagerBox = {415, 110, 200, 45};

    UI::drawInputField(
        wagerBox,
        wagerInput.c_str(),
        false,
        "Enter Wager"
    );

    DrawText("Type a number using your keyboard.", 200, 165, 16, GRAY);

    // Accept only numeric characters
    int character = GetCharPressed();

    while (character > 0)
    {
        if (character >= '0' &&
            character <= '9' &&
            wagerInput.length() < 6)
        {
            wagerInput += static_cast<char>(character);
        }

        character = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !wagerInput.empty())
    {
        wagerInput.pop_back();
    }

    // ========================================
    // NUMBER SELECTION
    // ========================================

    DrawText("Select a number (1 - 10):", 300, 220, 22, UI::Text);

    Vector2 mousePosition = GetMousePosition();

    for (int i = 1; i <= 10; i++)
    {
        int column = (i - 1) % 5;
        int row = (i - 1) / 5;

        Rectangle numberButton = {
            200.0f + column * 110,
            270.0f + row * 75,
            80,
            55
        };

        bool hovered = CheckCollisionPointRec(
            mousePosition,
            numberButton
        );

        Color buttonColor = UI::Primary;

        if (selectedNumber == i)
            buttonColor = UI::Success;
        else if (hovered)
            buttonColor = UI::PrimaryHover;

        DrawRectangleRec(numberButton, buttonColor);

        DrawText(
            TextFormat("%d", i),
            static_cast<int>(numberButton.x + 30),
            static_cast<int>(numberButton.y + 15),
            25,
            RAYWHITE
        );

        if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            selectedNumber = i;
            hasPlayed = false;
            resultMessage.clear();
        }
    }

    DrawText(
        TextFormat("Selected number: %d", selectedNumber),
        330, 435, 20, UI::MutedText
    );

    // ========================================
    // PLAY BUTTON
    // ========================================


    // PLAY BUTTON
    Rectangle playButton = {350, 480, 200, 50};

    if (UI::drawButton(playButton, "PLAY", 25))
    {
        int wager = 0;

        for (char digit : wagerInput)
        {
            wager = wager * 10 + (digit - '0');
        }

        if (!engine->set_wager(wager))
        {
            resultMessage = "Invalid wager! Check your credits.";
            hasPlayed = false;
        }
        else
        {
            GameResult result = engine->playGame(selectedNumber);

            switch (result)
            {
                case GameResult::Win:
                    resultMessage = "You WIN!";
                    hasPlayed = true;
                    break;

                case GameResult::Lose:
                    resultMessage = "You LOST!";
                    hasPlayed = true;
                    break;

                case GameResult::InvalidInput:
                    resultMessage = "Invalid number!";
                    hasPlayed = false;
                    break;

                case GameResult::InvalidWager:
                    resultMessage = "Invalid wager!";
                    hasPlayed = false;
                    break;
            }
        }
    }


    // ========================================
    // RESULT
    // ========================================

    if (!resultMessage.empty())
    {
        DrawText(
            resultMessage.c_str(),
            350, 545, 22,
            hasPlayed
                ? (resultMessage == "You WIN!" ? GREEN : UI::Danger)
                : ORANGE
        );
    }

    // ========================================
    // BACK BUTTON
    // ========================================


    // BACK BUTTON
    Rectangle backButton = {30, 30, 100, 40};

    if (UI::drawButton(backButton, "BACK", 20))
    {
        currentScreen = GuiScreen::GameSelection;
    }

}



void GuiApplication::drawDealerNoMatch()
{
    Rectangle panel = {150, 85, 600, 420};
    UI::drawPanel(panel);
    DrawText("DEALER NO MATCH", 280, 40, 35, UI::Text);

    GameEngine* engine = session->get_engine();

    if (engine == nullptr)
    {
        DrawText("Session is inactive.", 320, 280, 22, UI::Danger);
        return;
    }

    // Player credits
    DrawText(
        TextFormat("Credits: %d", session->get_player().get_credits()),
        700, 30, 20, UI::Success
    );

    // Game instructions
    DrawText(
        "The dealer generates 16 random numbers.",
        210, 130, 22, UI::MutedText
    );

    DrawText(
        "If any numbers match, you lose.",
        260, 170, 20, UI::MutedText
    );

    DrawText(
        "If all numbers are unique, you win!",
        250, 205, 20, UI::Success
    );

    // Wager input
    DrawText("Enter your wager:", 250, 280, 22, UI::MutedText);

    Rectangle wagerBox = {460, 270, 200, 45};

    UI::drawInputField(
    wagerBox,
    wagerInput.c_str(),
    false,
    "Enter Wager"
    );

    // Keyboard input
    int character = GetCharPressed();

    while (character > 0)
    {
        if (character >= '0' &&
            character <= '9' &&
            wagerInput.length() < 6)
        {
            wagerInput += static_cast<char>(character);
        }

        character = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !wagerInput.empty())
    {
        wagerInput.pop_back();
    }

    // Vector2 mousePosition = GetMousePosition();


    // Play button
    Rectangle playButton = {350, 370, 200, 55};

    if (UI::drawButton(playButton, "PLAY", 25))
    {
        int wager = 0;

        for (char digit : wagerInput)
        {
            wager = wager * 10 + (digit - '0');
        }

        if (!engine->set_wager(wager))
        {
            resultMessage = "Invalid wager! Check your credits.";
            hasPlayed = false;
        }
        else
        {
            GameResult result = engine->playGame(0);

            switch (result)
            {
                case GameResult::Win:
                    resultMessage = "You WIN!";
                    hasPlayed = true;
                    break;

                case GameResult::Lose:
                    resultMessage = "You LOST!";
                    hasPlayed = true;
                    break;

                case GameResult::InvalidInput:
                    resultMessage = "Invalid input!";
                    hasPlayed = false;
                    break;

                case GameResult::InvalidWager:
                    resultMessage = "Invalid wager!";
                    hasPlayed = false;
                    break;
            }
        }
    }


    // Display result
    if (!resultMessage.empty())
    {
        Color resultColor = ORANGE;

        if (hasPlayed)
        {
            resultColor =
                resultMessage == "You WIN!" ? GREEN : RED;
        }

        DrawText(
            resultMessage.c_str(),
            320, 465, 25, resultColor
        );
    }

    // Back button

    // Back button
    Rectangle backButton = {30, 30, 100, 40};

    if (UI::drawButton(backButton, "BACK", 20))
    {
        currentScreen = GuiScreen::GameSelection;
    }

}



void GuiApplication::drawFindTheAce()
{

    Rectangle panel = {135, 85, 600, 460};
    UI::drawPanel(panel);

    DrawText("FIND THE ACE", 330, 35, 35, UI::Text);

    GameEngine* engine = session->get_engine();

    if (engine == nullptr)
    {
        DrawText("Session is inactive.", 320, 280, 22, RED);
        return;
    }

    Vector2 mousePosition = GetMousePosition();

    // Display credits
    DrawText(
        TextFormat("Credits: %d", session->get_player().get_credits()),
        700, 30, 20, UI::Success
    );

    FindTheAceState state = engine->getFindTheAceState();

    // ========================================
    // INITIAL SCREEN / NEW ROUND
    // ========================================

    if (state == FindTheAceState::NotStarted ||
        state == FindTheAceState::Resolved)
    {
        if (state == FindTheAceState::Resolved)
        {
            DrawText(
                aceResultMessage.c_str(),
                300, 120, 28,
                aceResultMessage == "You WIN!" ? GREEN : RED
            );

            DrawText(
                "Start another round?",
                350, 165, 20, UI::MutedText
            );
        }
        else
        {
            DrawText(
                "Find the Ace hidden behind three cards.",
                230, 130, 22, UI::MutedText
            );
        }

        DrawText("Initial wager:", 270, 250, 22, UI::MutedText);

        Rectangle wagerBox = {450, 240, 200, 45};

        UI::drawInputField(
        wagerBox,
        aceInitialWagerInput.c_str(),
        false,
        "Enter Wager"
    );

        // Keyboard input
        int character = GetCharPressed();

        while (character > 0)
        {
            if (character >= '0' &&
                character <= '9' &&
                aceInitialWagerInput.length() < 6)
            {
                aceInitialWagerInput +=
                    static_cast<char>(character);
            }

            character = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) &&
            !aceInitialWagerInput.empty())
        {
            aceInitialWagerInput.pop_back();
        }

        Rectangle startButton = {350, 340, 200, 55};


        const char* buttonLabel =
            state == FindTheAceState::Resolved
                ? "PLAY AGAIN"
                : "START ROUND";

        if (UI::drawButton(startButton, buttonLabel, 22))
        {
            int wager = 0;

            for (char digit : aceInitialWagerInput)
            {
                wager = wager * 10 + (digit - '0');
            }

            if (engine->startFindTheAce(wager))
            {
                aceResultMessage.clear();
                aceAdditionalWagerInput = "10";
            }
            else
            {
                aceResultMessage =
                    "Invalid wager! Check your credits.";
            }
        }

    }

    // ========================================
    // INITIAL CARD SELECTION
    // ========================================

    else if (state == FindTheAceState::WaitingForInitialPick)
    {
        DrawText(
            "Choose one of the three cards.",
            290, 130, 22, UI::MutedText
        );

        for (int i = 0; i < 3; i++)
        {
            Rectangle card = {
                230.0f + i * 160,
                220,
                120,
                180
            };

            bool hovered = CheckCollisionPointRec(
                mousePosition,
                card
            );

            DrawRectangleRec(
                card,
                hovered ? UI::PrimaryHover : UI::Primary
            );

            DrawRectangleLinesEx(card, 3, BLACK);

            DrawText(
                TextFormat("CARD %d", i + 1),
                static_cast<int>(card.x + 20),
                static_cast<int>(card.y + 75),
                22,
                RAYWHITE
            );

            if (hovered &&
                IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                engine->selectFindTheAceCard(i + 1);
            }
        }
    }

    // ========================================
    // PLAYER DECISION
    // ========================================

    else if (state == FindTheAceState::WaitingForDecision)
    {
        DrawText(
            "The Queen has been revealed!",
            290, 120, 24, UI::Success
        );

        int selectedPosition =
            engine->getFindTheAceSelectedPosition();

        int queenPosition =
            engine->getFindTheAceRevealedQueenPosition();

        // Display the three cards
        for (int i = 0; i < 3; i++)
        {
            Rectangle card = {
                230.0f + i * 160,
                190,
                120,
                160
            };

            Color cardColor = UI::Primary;
            const char* cardLabel = "HIDDEN";

            if (i == queenPosition)
            {
                cardColor = PURPLE;
                cardLabel = "QUEEN";
            }
            else if (i == selectedPosition)
            {
                cardColor = UI::Success;
                cardLabel = "YOUR PICK";
            }

            DrawRectangleRec(card, cardColor);
            DrawRectangleLinesEx(card, 3, BLACK);

            DrawText(
                cardLabel,
                static_cast<int>(card.x + 10),
                static_cast<int>(card.y + 65),
                18,
                RAYWHITE
            );

            DrawText(
                TextFormat("CARD %d", i + 1),
                static_cast<int>(card.x + 25),
                static_cast<int>(card.y + 105),
                16,
                RAYWHITE
            );
        }

        // Change selection button

        Rectangle changeButton = {150, 410, 250, 55};

        if (UI::drawButton(changeButton, "CHANGE CARD", 20))
        {
            if (engine->changeFindTheAceSelection())
            {
                GameResult result = engine->finishFindTheAce();

                aceResultMessage =
                    result == GameResult::Win
                        ? "You WIN!"
                        : "You LOST!";
            }
        }


        // Additional wager input
        DrawText(
            "Or increase your wager:",
            470, 385, 18, UI::MutedText
        );

        Rectangle additionalWagerBox = {
            470, 415, 200, 40
        };

        UI::drawInputField(
            additionalWagerBox,
            aceAdditionalWagerInput.c_str(),
            false,
            "Enter Wager"
        );

        // Keyboard input
        int character = GetCharPressed();

        while (character > 0)
        {
            if (character >= '0' &&
                character <= '9' &&
                aceAdditionalWagerInput.length() < 6)
            {
                aceAdditionalWagerInput +=
                    static_cast<char>(character);
            }

            character = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) &&
            !aceAdditionalWagerInput.empty())
        {
            aceAdditionalWagerInput.pop_back();
        }


        Rectangle increaseButton = {470, 475, 200, 50};

        if (UI::drawButton(increaseButton, "INCREASE WAGER", 18))
        {
            int additionalWager = 0;

            for (char digit : aceAdditionalWagerInput)
            {
                additionalWager =
                    additionalWager * 10 + (digit - '0');
            }

            if (engine->increaseFindTheAceWager(additionalWager))
            {
                GameResult result = engine->finishFindTheAce();

                aceResultMessage =
                    result == GameResult::Win
                        ? "You WIN!"
                        : "You LOST!";
            }
            else
            {
                aceResultMessage =
                    "Invalid additional wager!";
            }
        }


        // Display validation message without hiding the decision screen
        if (aceResultMessage == "Invalid additional wager!")
        {
            DrawText(
                aceResultMessage.c_str(),
                470, 540, 18, RED
            );
        }
    }

    // ========================================
    // BACK BUTTON
    // ========================================


    Rectangle backButton = {30, 30, 100, 40};

    if (UI::drawButton(backButton, "BACK", 20))
    {
        currentScreen = GuiScreen::GameSelection;
    }

}

void GuiApplication::drawProfile()
{

    Rectangle panel = {220, 40, 460, 470};
    UI::drawPanel(panel);

    if (!session || !session->is_active())
    {
        currentScreen = GuiScreen::Welcome;
        return;
    }

    const Player& player = session->get_player();

    DrawText("PLAYER PROFILE", 300, 80, 35, UI::Text);

    DrawText(
        TextFormat("Player ID: %d", player.get_uid()),
        300, 180, 23, UI::MutedText
    );

    DrawText(
        TextFormat("Name: %s", player.get_name().c_str()),
        300, 230, 23, UI::MutedText
    );

    DrawText(
        TextFormat("Credits: %d", player.get_credits()),
        300, 280, 23, UI::MutedText
    );

    DrawText(
        TextFormat("Highscore: %d", player.get_highscore()),
        300, 330, 23, UI::MutedText
    );

    Rectangle backButton = {350, 430, 200, 50};

    if (UI::drawButton(backButton, "BACK", 22))
    {
        currentScreen = GuiScreen::MainMenu;
    }
}

void GuiApplication::drawWelcome()
{
    // Main content panel
    Rectangle panel = {170, 70, 560, 430};
    UI::drawPanel(panel);

    // Application title
    const char* title = "GAME OF CHANCE";
    int titleWidth = MeasureText(title, 40);

    DrawText(
        title,
        (900 - titleWidth) / 2,
        130,
        40,
        UI::Text
    );

    // Supporting text
    const char* subtitle =
        "Welcome! Please choose an option.";

    int subtitleWidth = MeasureText(subtitle, 20);

    DrawText(
        subtitle,
        (900 - subtitleWidth) / 2,
        200,
        20,
        UI::MutedText
    );

    // Register button
    Rectangle registerButton = {350, 270, 200, 55};

    if (UI::drawButton(registerButton, "REGISTER", 22))
    {
        uidInput.clear();
        nameInput.clear();
        authMessage.clear();

        uidFieldActive = true;
        nameFieldActive = false;

        currentScreen = GuiScreen::Register;
    }

    // Login button
    Rectangle loginButton = {350, 350, 200, 55};

    if (UI::drawButton(loginButton, "LOGIN", 22))
    {
        uidInput.clear();
        authMessage.clear();

        uidFieldActive = true;
        nameFieldActive = false;

        currentScreen = GuiScreen::Login;
    }
}

void GuiApplication::drawRegister()
{

    Rectangle panel = {220, 55, 550, 500};
    UI::drawPanel(panel);

    DrawText("PLAYER REGISTRATION", 280, 80, 35, UI::Text);

    Vector2 mousePosition = GetMousePosition();

    // UID input field
    Rectangle uidField = {300, 180, 300, 45};

    bool uidHovered = CheckCollisionPointRec(mousePosition, uidField);

    DrawText("Player ID:", 300, 150, 20, UI::MutedText);

    UI::drawInputField(
        uidField,
        uidInput.c_str(),
        uidFieldActive,
        "Enter Player ID"
    );

    if (uidHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        uidFieldActive = true;
        nameFieldActive = false;
    }

    // Name input field
    Rectangle nameField = {300, 280, 300, 45};

    bool nameHovered =
        CheckCollisionPointRec(mousePosition, nameField);

    DrawText("Player Name:", 300, 250, 20, UI::MutedText);

    UI::drawInputField(
        nameField,
        nameInput.c_str(),
        nameFieldActive,
        "Enter Player Name"
    );

    if (nameHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        uidFieldActive = false;
        nameFieldActive = true;
    }

    // Keyboard input
    int key = GetCharPressed();

    while (key > 0)
    {
        if (uidFieldActive && key >= '0' && key <= '9')
        {
            if (uidInput.length() < 9)
                uidInput += static_cast<char>(key);
        }
        else if (nameFieldActive &&
                 key >= 32 && key <= 126 &&
                 key != '|')
        {
            if (nameInput.length() < 30)
                nameInput += static_cast<char>(key);
        }

        key = GetCharPressed();
    }

    // Backspace
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        if (uidFieldActive && !uidInput.empty())
            uidInput.pop_back();

        else if (nameFieldActive && !nameInput.empty())
            nameInput.pop_back();
    }


    // Register button
    Rectangle registerButton = {350, 380, 200, 50};

    if (UI::drawButton(registerButton, "REGISTER", 22))
    {
        registerPlayer();
    }



    // Back button
    Rectangle backButton = {350, 450, 200, 45};

    if (UI::drawButton(backButton, "BACK", 20))
    {
        currentScreen = GuiScreen::Welcome;
    }


    // Validation / operation message
    if (!authMessage.empty())
    {
        DrawText(
            authMessage.c_str(),
            250,
            520,
            18,
            RED
        );
    }
}

void GuiApplication::registerPlayer()
{
    // 1. Validate UID
    if (uidInput.empty())
    {
        authMessage = "Please enter a Player ID.";
        return;
    }

    if (nameInput.empty())
    {
        authMessage = "Please enter a Player Name.";
        return;
    }

    int uid = 0;

    try
    {
        size_t position = 0;
        uid = std::stoi(uidInput, &position);

        if (position != uidInput.length() || uid <= 0)
        {
            authMessage = "Invalid Player ID.";
            return;
        }
    }
    catch (const std::exception&)
    {
        authMessage = "Invalid Player ID.";
        return;
    }

    // 2. Check whether UID already exists
    Player existingPlayer;

    if (Storage::load_player(uid, existingPlayer, dataFile))
    {
        authMessage = "This Player ID is already registered.";
        return;
    }

    // 3. Create a new player
    try
    {
        Player newPlayer(uid, nameInput, 100);

        // 4. Save player
        if (!Storage::save_player(newPlayer, dataFile))
        {
            authMessage = "Could not save player data.";
            return;
        }

        // 5. Start player session
        session = std::make_unique<PlayerSession>(newPlayer);

        authMessage.clear();
        currentScreen = GuiScreen::MainMenu;
    }
    catch (const std::exception& error)
    {
        authMessage = error.what();
    }
}



void GuiApplication::drawLogin()
{

    Rectangle panel = {220, 40, 460, 470};
    UI::drawPanel(panel);

    DrawText("PLAYER LOGIN", 330, 100, 35, UI::Text);

    Vector2 mousePosition = GetMousePosition();

    // UID input field
    Rectangle uidField = {300, 220, 300, 45};

    bool uidHovered =
        CheckCollisionPointRec(mousePosition, uidField);

    DrawText("Player ID:", 300, 150, 20, UI::MutedText);

    UI::drawInputField(
        uidField,
        uidInput.c_str(),
        uidFieldActive,
        "Enter Player ID"
    );

    if (uidHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        uidFieldActive = true;
        nameFieldActive = false;
    }

    // Keyboard input
    int key = GetCharPressed();

    while (key > 0)
    {
        if (uidFieldActive && key >= '0' && key <= '9')
        {
            if (uidInput.length() < 9)
                uidInput += static_cast<char>(key);
        }

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) &&
        uidFieldActive &&
        !uidInput.empty())
    {
        uidInput.pop_back();
    }


    // Login button
    Rectangle loginButton = {350, 320, 200, 50};

    if (UI::drawButton(loginButton, "LOGIN", 22))
    {
        loginPlayer();
    }



    // Back button
    Rectangle backButton = {350, 400, 200, 45};

    if (UI::drawButton(backButton, "BACK", 20))
    {
        currentScreen = GuiScreen::Welcome;
    }


    // Validation / operation message
    if (!authMessage.empty())
    {
        DrawText(
            authMessage.c_str(),
            250,
            520,
            18,
            UI::Danger
        );
    }
}

void GuiApplication::loginPlayer()
{
    // 1. Validate UID
    if (uidInput.empty())
    {
        authMessage = "Please enter your Player ID.";
        return;
    }

    int uid = 0;

    try
    {
        size_t position = 0;
        uid = std::stoi(uidInput, &position);

        if (position != uidInput.length() || uid <= 0)
        {
            authMessage = "Invalid Player ID.";
            return;
        }
    }
    catch (const std::exception&)
    {
        authMessage = "Invalid Player ID.";
        return;
    }

    // 2. Load player from storage
    Player loadedPlayer;

    if (!Storage::load_player(uid, loadedPlayer, dataFile))
    {
        authMessage = "Player ID not found. Please register first.";
        return;
    }

    // 3. Create player session
    session = std::make_unique<PlayerSession>(loadedPlayer);

    // 4. Clear authentication state
    authMessage.clear();
    uidInput.clear();

    // 5. Navigate to main menu
    currentScreen = GuiScreen::MainMenu;
}

void GuiApplication::logoutPlayer()
{
    // Ensure a player session exists
    if (!session || !session->is_active())
    {
        currentScreen = GuiScreen::Welcome;
        session.reset();
        return;
    }

    // Save the player's latest state
    if (!Storage::save_player(session->get_player(), dataFile))
    {
        authMessage = "Could not save player data. Logout cancelled.";
        return;
    }

    // End the active session
    session->end_session();

    // Destroy the session
    session.reset();

    // Clear authentication inputs
    uidInput.clear();
    nameInput.clear();

    authMessage.clear();

    // Return to welcome screen
    currentScreen = GuiScreen::Welcome;
}
