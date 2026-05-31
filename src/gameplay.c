#include <stdlib.h>

#include "include/raylib.h"
#include "include/raymath.h"

#include "header.h"

#define CAMERA_MOVE_MODIFIER 50  // This is a necessary value to track in order to synchronise mouse position with camera position
#define PLAYER_MOVEMENT_SPEED 3  // temporary just so i can easily mess around

// Global Variables -----


// ----------------------

void updateCamera(Camera2D* camera, Vector2* cameraMouseDifference);
void updatePlayer(Unit* player);  // temporary function for messing around

void UpdateGameplayScreen(void)
{
    // Camera
    Camera2D camera = 
    {
    .target = (Vector2){0, 0},
    .zoom = 1.0f
    };

    Rectangle renderingRectangle = (Rectangle){0, 0,  32, 64};  // we are saying that we want it to start rendering the tile at coordinate 0,0 and we want it to render EXACTLY 64 tiles

    // Timer
    bool pauseState = 0;
    Timer timer = { 0 };
    int ticker = 0; // for counting seconds
    StartTimer(&timer, SECOND);

    Vector2 mousePosition = { 0 };
    Vector2 cameraMouseDifference = { 0, 0 };
    float wheel = 0;

    // Determining tile data
    int rows = 20;
    int columns = 160;
    int* map = returnMapData(rows, columns);

    Unit* units = returnUnitData(); 

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Pause 
        if (!(pauseState)) UpdateTimer(&timer);  // with every single cycle, timer->Lifetime is deducted by GetFrameTime as long as it is greater than 0
        if (TimerDone(&timer)) {  
            StartTimer(&timer, SECOND);
            ticker++; // we have a ticker because apparently why not
        }

        // Player input
        updateCamera(&camera, &cameraMouseDifference);
        updatePlayer(&units[0]);
        if (IsKeyPressed(KEY_P)) { if (pauseState) pauseState = 0; else pauseState = 1; };
        if (IsKeyPressed(KEY_Q)) { renderingRectangle.x --; };
        if (IsKeyPressed(KEY_E)) { renderingRectangle.x ++; };

            
        mousePosition.x = GetMousePosition().x + cameraMouseDifference.x;
        mousePosition.y = GetMousePosition().y + cameraMouseDifference.y;
        wheel = GetMouseWheelMove();

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

            DrawRectangleLines(renderingRectangle.x * TILE_SIZE, renderingRectangle.y * TILE_SIZE, renderingRectangle.width * TILE_SIZE, renderingRectangle.height * TILE_SIZE, BLUE);

            DrawText("hello", 20, 20, 20, BLACK);

            //   simply start the for loop at the coordinate where the rendering rectangle is. 
            // We are only rendering stuff that is inside the renderRectangle
            for (int i = 0; i < rows; i++) {
                for (int j = (int){renderingRectangle.x}; j < ((int){renderingRectangle.x} + (int){renderingRectangle.width}); j++)
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
                        if (CheckCollisionPointRec(units[0].position, (Rectangle){j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE}))
                        {
                            DrawRectangleLines(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);
                        }
                    }
                }
            }

            DrawCircle(units[0].position.x, units[0].position.y, TILE_SIZE / 4, BLUE);
            EndMode2D();
            
            DrawFPS(10, 10);
            // DrawText(TextFormat("mouseposition= x%fy%f", mousePosition.x, mousePosition.y), 30, 140, 30, LIGHTGRAY);
            // DrawText(TextFormat("cameramousedifference= x%fy%f", cameraMouseDifference.x, cameraMouseDifference.y), 30, 170, 30, LIGHTGRAY);
            DrawText(TextFormat("renderingRectangle.x(int)= %d", (int){renderingRectangle.x}), 30, 170, 30, LIGHTGRAY);
            DrawText(TextFormat("renderingRectangle.width)= %d", (int){renderingRectangle.width}), 30, 210, 30, LIGHTGRAY);
            // DrawText(TextFormat("ticker= %d", ticker), 30, 170, 30, LIGHTGRAY);
            // DrawText(TextFormat("pauseState= %d", pauseState), 30, 210, 30, LIGHTGRAY);

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
    if (IsKeyDown(KEY_UP)) { player->position.y = player->position.y - PLAYER_MOVEMENT_SPEED; };
    if (IsKeyDown(KEY_LEFT)) { player->position.x = player->position.x - PLAYER_MOVEMENT_SPEED; };
    if (IsKeyDown(KEY_DOWN)) { player->position.y = player->position.y + PLAYER_MOVEMENT_SPEED; };
    if (IsKeyDown(KEY_RIGHT)) { player->position.x = player->position.x + PLAYER_MOVEMENT_SPEED; };
}