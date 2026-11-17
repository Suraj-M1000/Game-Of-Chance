#include "GuiStyle.h"

namespace UI
{
    bool drawButton(
        Rectangle bounds,
        const char* text,
        int fontSize
    )
    {
        Vector2 mousePosition = GetMousePosition();

        bool hovered =
            CheckCollisionPointRec(mousePosition, bounds);

        DrawRectangleRec(
            bounds,
            hovered ? PrimaryHover : Primary
        );

        int textWidth = MeasureText(text, fontSize);

        int textX =
            static_cast<int>(
                bounds.x + (bounds.width - textWidth) / 2
            );

        int textY =
            static_cast<int>(
                bounds.y + (bounds.height - fontSize) / 2
            );

        DrawText(
            text,
            textX,
            textY,
            fontSize,
            Text
        );

        return hovered &&
               IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }


    void drawPanel(Rectangle bounds)
    {
        DrawRectangleRec(bounds, Panel);

        DrawRectangleLinesEx(
            bounds,
            1.0f,
            Border
        );
    }


        void drawInputField(
        Rectangle bounds,
        const char* value,
        bool focused,
        const char* placeholder
    )
    {
        DrawRectangleRec(bounds, InputBackground);

        DrawRectangleLinesEx(
            bounds,
            1.5f,
            focused ? InputFocus : InputBorder
        );

        bool hasValue = value != nullptr && value[0] != '\0';

        const char* displayText =
            hasValue ? value : placeholder;

        Color textColor =
            hasValue ? Text : MutedText;

        int fontSize = 20;

        int textY = static_cast<int>(
            bounds.y + (bounds.height - fontSize) / 2
        );

        DrawText(
            displayText,
            static_cast<int>(bounds.x + 12),
            textY,
            fontSize,
            textColor
        );
    }
}
