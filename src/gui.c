#include "stdio.h"
#include "stdlib.h"

#include "include/raylib.h"
#include "header.h"


// static Texture2D buttonTexture;


Rectangle* initialiseButtons(int numberOfButtons)
{
    Rectangle* roomCreationButtons = malloc(sizeof(Rectangle) * (numberOfButtons));

    roomCreationButtons[0] = (Rectangle){GetScreenWidth() - 150, 10, 80, 80};
    roomCreationButtons[1] = (Rectangle){GetScreenWidth() - 150, 150, 80, 80};
    roomCreationButtons[2] = (Rectangle){GetScreenWidth() - 150, 290, 80, 80};

    return roomCreationButtons;
}

void drawGuiElement(void)
{
    // DrawTextureRec(buttonQ, sourceRec, (Vector2){ btnBounds.x, btnBounds.y }, WHITE); // Draw button frame
}

void GuiInit(void)
{
    // buttonTexture = LoadTexture("Untitled.png");
}

void GuiDraw(void)
{
    // DrawTexture(buttonTexture, 100, 100, WHITE);
}

void GuiShutdown(void)
{
    // UnloadTexture(buttonTexture);
}