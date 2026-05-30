#include "stdio.h"
#include "stdlib.h"

#include "include/raylib.h"
#include "header.h"



void UpdateTitleScreen(void)
{
    if (IsKeyDown(KEY_Q))
    {
        currentScreen = GAMEPLAY;
    }

    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawRectangle(50,50,50,50,MAROON);

        DrawText(TextFormat("Title Screen. Press Q to change game state to GAMEPLAY ==== %d", 13), 20, 35, 15, BLACK);
        
        DrawFPS(10, 10);

    EndDrawing();
}