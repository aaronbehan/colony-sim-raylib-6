#include <stdlib.h>

#include "include/raylib.h"
#include "include/raymath.h"

#include "header.h"

#define TILE_SIZE 16
#define CAMERA_MOVE_MODIFIER 50  // This is a necessary value to track in order to synchronise mouse position with camera position
#define PLAYER_MOVEMENT_SPEED 6  // temporary just so i can easily mess around

// Global Variables -----


// ----------------------

void updateCamera(Camera2D* camera, Vector2* cameraMouseDifference);
void updatePlayer(Unit* player);  // temporary function for messing around

void UpdateGameplayScreen(void)
{
    Camera2D camera = 
    {
    .target = (Vector2){0, 0},
    .zoom = 1.0f
    };

    int pauseState = 0;
    Timer timer = { 0 };
    int ticker = 0; // for counting seconds
    StartTimer(&timer, SECOND);

    Vector2 mousePosition = { 0 };
    Vector2 cameraMouseDifference = { 0, 0 };
    float wheel = 0;

    // Determining tile data
    int rows = 20;
    int columns = 20;
    int* map = returnMapData(rows, columns);

    Unit* units = returnUnitData();  // lets see if we can spawn him above the blocks, make him fall and land on the tiles

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (!(pauseState)) UpdateTimer(&timer);  // with every single cycle, timer->Lifetime is deducted by GetFrameTime as long as it is greater than 0
        if (TimerDone(&timer)) {  
            StartTimer(&timer, SECOND);
            ticker++; // we have a ticker because apparently why not
        }

        updateCamera(&camera, &cameraMouseDifference);
        updatePlayer(&units[0]);
        if (IsKeyPressed(KEY_P)) // Pausing
        { if (pauseState) pauseState = 0;
        else pauseState = 1; };
            
        mousePosition.x = GetMousePosition().x + cameraMouseDifference.x;
        mousePosition.y = GetMousePosition().y + cameraMouseDifference.y;

        wheel = GetMouseWheelMove();

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

            DrawText("hello", 20, 20, 20, BLACK);

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++)
                {
                    if (map[i * columns + j] == 1) 
                    {
                        DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, MAROON);
                        DrawRectangleLines(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLACK);

                        if (CheckCollisionPointRec(mousePosition, (Rectangle){j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE}))
                        {
                            DrawRectangleLines(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, GREEN);
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
                            {
                                map[i * columns + j] = 0;
                            };
                        }
                    }
                }
            }

            DrawCircle(units[0].position.x, units[0].position.y, TILE_SIZE, BLUE);
            EndMode2D();
            
            DrawFPS(10, 10);
            // DrawText(TextFormat("mouseposition= x%fy%f", mousePosition.x, mousePosition.y), 30, 140, 30, LIGHTGRAY);
            // DrawText(TextFormat("cameramousedifference= x%fy%f", cameraMouseDifference.x, cameraMouseDifference.y), 30, 170, 30, LIGHTGRAY);
            DrawText(TextFormat("ticker= %d", ticker), 30, 170, 30, LIGHTGRAY);
            DrawText(TextFormat("pauseState= %d", pauseState), 30, 210, 30, LIGHTGRAY);

        EndDrawing();
    }

    // Free memory
    free(map);
    free(units);
}

void updateCamera(Camera2D* camera, Vector2* cameraMouseDifference)
{
    if (IsKeyPressed(KEY_W)) {
        camera->target.y = camera->target.y - CAMERA_MOVE_MODIFIER; 
        cameraMouseDifference->y = cameraMouseDifference->y - CAMERA_MOVE_MODIFIER;
    };
    if (IsKeyPressed(KEY_A)) {
        camera->target.x = camera->target.x - CAMERA_MOVE_MODIFIER; 
        cameraMouseDifference->x = cameraMouseDifference->x - CAMERA_MOVE_MODIFIER;

    };
    if (IsKeyPressed(KEY_S)) {
        camera->target.y = camera->target.y + CAMERA_MOVE_MODIFIER;
        cameraMouseDifference->y = cameraMouseDifference->y + CAMERA_MOVE_MODIFIER;

    };
    if (IsKeyPressed(KEY_D)) {
        camera->target.x = camera->target.x + CAMERA_MOVE_MODIFIER;
        cameraMouseDifference->x = cameraMouseDifference->x + CAMERA_MOVE_MODIFIER;

    };
    if (IsKeyPressed(KEY_SPACE)) {
        camera->target = (Vector2){ 0 }; 
        cameraMouseDifference->x = 0;
        cameraMouseDifference->y = 0;
    };
}

void updatePlayer(Unit* player)
{
    if (IsKeyPressed(KEY_UP)) { player->position.y = player->position.y - PLAYER_MOVEMENT_SPEED; };
    if (IsKeyPressed(KEY_LEFT)) { player->position.x = player->position.x - PLAYER_MOVEMENT_SPEED; };
    if (IsKeyPressed(KEY_DOWN)) { player->position.y = player->position.y + PLAYER_MOVEMENT_SPEED; };
    if (IsKeyPressed(KEY_RIGHT)) { player->position.x = player->position.x + PLAYER_MOVEMENT_SPEED; };
}