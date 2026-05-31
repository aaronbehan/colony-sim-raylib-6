#include "include/raylib.h"
#include "header.h"
#include <stdio.h>


// Global variables
GameScreen currentScreen = GAMEPLAY;
// GameScreen currentScreen = TITLE;


static void UpdateDrawFrame(void);


int main(void)
{
    const int screenWidth = 1400;
    const int screenHeight = 1000;
    
    InitWindow(screenWidth, screenHeight, "Raylib 6");

    SetTargetFPS(60);

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}


static void UpdateDrawFrame(void)
{
    switch(currentScreen)
    {
        case GAMEPLAY:
        {
            UpdateGameplayScreen();
        } break;
        case TITLE:
        {
            UpdateTitleScreen();
        } break;
    }
}
