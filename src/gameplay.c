#include <stdlib.h>

#include "include/raylib.h"
#include "include/raymath.h"

#include "header.h"

#define CAMERA_MOVE_MODIFIER 3  // This is a necessary value to track in order to synchronise mouse position with camera position
#define PLAYER_MOVEMENT_SPEED 3  // temporary just so i can easily mess around

// Global Variables -----


// ----------------------

void updateCamera(Camera2D* camera, Vector2* cameraMouseDifference, Rectangle* renderingRect, Vector2 mapLimits);
void updatePlayer(Unit* player);  // temporary function for messing around
void updateUnits(Unit* units, int numberOfUnits);


void UpdateGameplayScreen(void)
{
    // Camera
    Camera2D camera = {
    .target = (Vector2){0, 0},
    .zoom = 1.0f
    };

    Rectangle renderingRectangle = (Rectangle){ 0, 0, (GetScreenWidth() / TILE_SIZE), (GetScreenHeight() / TILE_SIZE) };

    // Timer
    bool pauseState = 0;
    Timer timer = { 0 };
    int ticker = 0; // for counting seconds
    StartTimer(&timer, SECOND);

    Vector2 mousePosition = { 0 }; // will allow GUI interaction
    Vector2 mousePositionGridLocked = { 0 };  // will allow world data interaction. may not actually be necessary... 
    Vector2 cameraMouseDifference = { 0, 0 };
    float wheel = 0;

    // Determining tile data
    Vector2 mapLimits = {
        .y = MAP_ROWS,  // rows
        .x = MAP_COLUMNS  // columns
    };
    
    int* map = returnMapData(mapLimits);

    int numberOfUnits = 1;
    Unit* units = returnUnitData();

    // maybe just load absolutely all textures here and put them into a dynamic array, then whenever we want to use them, we just feed the entire array into the function

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        mousePosition.x = GetMousePosition().x + cameraMouseDifference.x;
        mousePosition.y = GetMousePosition().y + cameraMouseDifference.y;
        mousePositionGridLocked.x = floorf(mousePosition.x / TILE_SIZE);
        mousePositionGridLocked.y = floorf(mousePosition.y / TILE_SIZE);

        // Pause 
        if (!(pauseState)) UpdateTimer(&timer);  // with every single cycle, timer->Lifetime is deducted by GetFrameTime as long as it is greater than 0
        if (TimerDone(&timer)) {  
            StartTimer(&timer, SECOND);
            ticker++; // we have a ticker because apparently why not
        }

        // Player input
        updateCamera(&camera, &cameraMouseDifference, &renderingRectangle, mapLimits);
        updatePlayer(&units[0]);
        updateUnits(units, numberOfUnits);


        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) 
        {
            Vector2 startCell = {
                floorf(units[0].position.x / TILE_SIZE),
                floorf(units[0].position.y / TILE_SIZE)
            };

            if (units[0].waypoint.node != NULL) free(units[0].waypoint.node);
            
            units[0].waypoint = aStar(startCell, mousePositionGridLocked, map);
        }

        if (IsKeyPressed(KEY_P)) { if (pauseState) pauseState = 0; else pauseState = 1; };

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

            DrawRectangleLines(renderingRectangle.x * TILE_SIZE, renderingRectangle.y * TILE_SIZE, renderingRectangle.width * TILE_SIZE, renderingRectangle.height * TILE_SIZE, BLUE);

            DrawText("hello", 20, 20, 20, BLACK);

            // Rendering only what is inside renderingRectangle location and dimensions
            for (int i = (int){renderingRectangle.y}; i < ((int){renderingRectangle.y} + (int){renderingRectangle.height}); i++) {
                for (int j = (int){renderingRectangle.x}; j < ((int){renderingRectangle.x} + (int){renderingRectangle.width}); j++)
                {
                    if (CheckCollisionPointRec(mousePositionGridLocked, (Rectangle){j, i, 1, 1}))
                    {
                        DrawRectangleLines(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, GREEN);
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            map[i * (int){mapLimits.x} + j] = 0;
                        };
                        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                            map[i * (int){mapLimits.x} + j] = 2;
                        };
                    }

                    if (map[i * (int){mapLimits.x} + j] != 0) {
                        DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, MAROON);
                        DrawRectangleLines(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLACK);

                        if (CheckCollisionPointRec(units[0].position, (Rectangle){j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE})) {
                            DrawRectangleLines(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);
                        }
                    }
                }
            }

            DrawCircle(units[0].position.x, units[0].position.y, TILE_SIZE / 4, BLUE);

            // print tiles 
            if (units[0].waypoint.numberOfTiles != 0) 
            {
                for (int i = 0; i < units[0].waypoint.numberOfTiles; i++)
                {
                    DrawCircle(units[0].waypoint.node[i].position.x * TILE_SIZE, units[0].waypoint.node[i].position.y * TILE_SIZE, TILE_SIZE / 4, BROWN);
                    DrawText(TextFormat("units[0].waypoint = x%fy%f", units[0].waypoint.node[i].position.x, units[0].waypoint.node[i].position.y), 20, (i * 21), 20, LIGHTGRAY);
                }
            }
            else 
            {
                DrawText(TextFormat("units[0].waypoint number of tiles = x%fy%f", units[0].waypoint.numberOfTiles), 20, (21), 20, LIGHTGRAY);
            }
            
            EndMode2D();
            
            DrawFPS(10, 10);
            // DrawText(TextFormat("mouse pos= x%fy%f", mousePosition.x, mousePosition.y), 30, 40, 30, LIGHTGRAY);
            // DrawText(TextFormat("mouse pos GL = x%fy%f", mousePositionGridLocked.x, mousePositionGridLocked.y), 30, 75, 30, LIGHTGRAY);
            if (units[0].waypoint.node != NULL) DrawText(TextFormat("units[i].waypoint.node[units[i].waypoint.numberOfTiles - 1].position.x = %f", units[0].waypoint.node[units[0].waypoint.numberOfTiles - 1].position.x ), 30, 750, 30, BLACK);

        EndDrawing();
    }

    // Free memory
    free(map);

    if (units[0].waypoint.node != NULL) free(units[0].waypoint.node);
    free(units);
}

void updateCamera(Camera2D* camera, Vector2* cameraMouseDifference, Rectangle* renderingRect, Vector2 mapLimits)
{
    if (IsKeyPressed(KEY_W)) {
        if ((renderingRect->y - CAMERA_MOVE_MODIFIER) <= 0) 
        {  // we want to prevent the user from moving left if they are already at the edge of map. otherwise segfault
            camera->target = (Vector2){ 0 }; 
            cameraMouseDifference->y = 0;
            renderingRect->y = 0;
        }  
        else 
        {
            camera->target.y = camera->target.y - (TILE_SIZE * CAMERA_MOVE_MODIFIER); 
            cameraMouseDifference->y = cameraMouseDifference->y - (TILE_SIZE * CAMERA_MOVE_MODIFIER);
            renderingRect->y = renderingRect->y - CAMERA_MOVE_MODIFIER;
        }
    }; 
    if ((IsKeyPressed(KEY_A))) {
        if ((renderingRect->x - CAMERA_MOVE_MODIFIER) <= 0) 
        {  // we want to prevent the user from moving left if they are already at the edge of map. otherwise segfault
            camera->target = (Vector2){ 0 }; 
            cameraMouseDifference->x = 0;
            renderingRect->x = 0;
        }  
        else 
        {
            camera->target.x = camera->target.x - (TILE_SIZE * CAMERA_MOVE_MODIFIER); 
            cameraMouseDifference->x = cameraMouseDifference->x - (TILE_SIZE * CAMERA_MOVE_MODIFIER);
            renderingRect->x = renderingRect->x - CAMERA_MOVE_MODIFIER;
        }
    };
    if (IsKeyPressed(KEY_S)) {
        camera->target.y = camera->target.y + (TILE_SIZE * CAMERA_MOVE_MODIFIER);
        cameraMouseDifference->y = cameraMouseDifference->y + (TILE_SIZE * CAMERA_MOVE_MODIFIER);
        renderingRect->y = renderingRect->y + CAMERA_MOVE_MODIFIER;
    };
    // requires a rethink. the logic should be if D key pressed. if rect.x + rect.width exceeds map limits after performing the rect/camera adjust calc, adjust the position of camera and rect to map limits - rect.width and map limits - camera width?
    // else: as normal
    if (IsKeyPressed(KEY_D)) { // && ((renderingRect->x + renderingRect->width) + TILE_SIZE * CAMERA_MOVE_MODIFIER) < (mapLimits.x * TILE_SIZE)) {
        // if (((renderingRect->x + renderingRect->width) + CAMERA_MOVE_MODIFIER) >= mapLimits.x) 
        // {
        //     // something very wrong with the below logic !!!!!!!!!!!!!!!!!!!!!!!!!
        //     camera->target.x = mapLimits.x - renderingRect->width;
        //     // cameraMouseDifference->x = cameraMouseDifference->x + (TILE_SIZE * CAMERA_MOVE_MODIFIER);
        //     renderingRect->x = mapLimits.x - renderingRect->width;
        // }
        // else 
        // {
            camera->target.x = camera->target.x + (TILE_SIZE * CAMERA_MOVE_MODIFIER);
            cameraMouseDifference->x = cameraMouseDifference->x + (TILE_SIZE * CAMERA_MOVE_MODIFIER);
            renderingRect->x = renderingRect->x + CAMERA_MOVE_MODIFIER;
        // }
    };
}

void updatePlayer(Unit* player)
{
    if (IsKeyDown(KEY_UP)) { player->position.y = player->position.y - PLAYER_MOVEMENT_SPEED; };
    if (IsKeyDown(KEY_LEFT)) { player->position.x = player->position.x - PLAYER_MOVEMENT_SPEED; };
    if (IsKeyDown(KEY_DOWN)) { player->position.y = player->position.y + PLAYER_MOVEMENT_SPEED; };
    if (IsKeyDown(KEY_RIGHT)) { player->position.x = player->position.x + PLAYER_MOVEMENT_SPEED; };
}


void updateUnits(Unit* units, int numberOfUnits) 
{
    // position.x += (GetFrameTime() * PLAYER_MOVEMENT_SPEED)

    for (int i = 0; i < numberOfUnits; i++)
    {
        if (!(units[i].waypoint.node)) continue;

        if (floorf(units[i].waypoint.node[units[i].waypoint.numberOfTiles - 1].position.x ))
        {
            
        }

    }

    // if (units[0].position.x < units[0].waypoint.node->position.x) units[i].position.x += PLAYER_MOVEMENT_SPEED;
    // if (units[0].position.x > units[i].waypoint.x) units[i].position.x -= units[i].archetype->speed;
    // if (units[0].position.y < units[i].waypoint.z) units[i].position.z += units[i].archetype->speed;
    // if (units[0].position.y > units[i].waypoint.z) units[i].position.z -= units[i].archetype->speed;

}
