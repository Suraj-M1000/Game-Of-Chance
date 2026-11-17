#ifndef GUISTYLE_H
#define GUISTYLE_H

#include "raylib.h"

namespace UI
{
    inline constexpr Color Background =
        {18, 22, 30, 255};

    inline constexpr Color Panel =
        {28, 34, 45, 255};

    inline constexpr Color Primary =
        {45, 85, 160, 255};

    inline constexpr Color PrimaryHover =
        {65, 110, 200, 255};

    inline constexpr Color Success =
        {45, 150, 90, 255};

    inline constexpr Color Danger =
        {180, 60, 65, 255};

    inline constexpr Color Text =
        {235, 238, 245, 255};

    inline constexpr Color MutedText =
        {165, 172, 185, 255};

    inline constexpr Color Border =
        {55, 65, 82, 255};

    inline constexpr Color InputBackground =
        {35, 42, 55, 255};

    inline constexpr Color InputBorder =
        {75, 87, 105, 255};

    inline constexpr Color InputFocus =
        {75, 125, 210, 255};


    bool drawButton(
        Rectangle bounds,
        const char* text,
        int fontSize = 22
    );

    void drawInputField(
    Rectangle bounds,
    const char* value,
    bool focused,
    const char* placeholder = ""
    );

    void drawPanel(Rectangle bounds);
}

#endif
