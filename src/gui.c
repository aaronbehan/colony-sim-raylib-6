#include "stdio.h"
#include "stdlib.h"

#include "include/raylib.h"
#include "header.h"



static Texture2D buttonTexture;

void drawGuiElement(void)
{


    // DrawTextureRec(buttonQ, sourceRec, (Vector2){ btnBounds.x, btnBounds.y }, WHITE); // Draw button frame
}



void GuiInit(void)
{
    buttonTexture = LoadTexture("button.png");
}

void GuiDraw(void)
{
    DrawTexture(buttonTexture, 100, 100, WHITE);
}

void GuiShutdown(void)
{
    UnloadTexture(buttonTexture);
}