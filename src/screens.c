#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "include/raylib.h"
#include "include/raymath.h"

#include "header.h"

#define CAMERA_MOVE_MODIFIER 3  // This is a necessary value to track in order to synchronise mouse position with camera position
#define PLAYER_MOVEMENT_SPEED 50  

// ---- Title -------------

void UpdateTitleScreen(void)
{
    if (IsKeyDown(KEY_Q)) { currentScreen = GAMEPLAY; }

    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle(50,50,50,50,MAROON);
        DrawText(TextFormat("Title Screen. Press Q to change game state to GAMEPLAY ==== %d", 13), 20, 35, 15, BLACK);
        DrawFPS(10, 10);
    EndDrawing();
}


// ----- Gameplay ----------

void updateCamera(Camera2D* camera, Vector2* cameraMouseDifference, Rectangle* renderingRect, Vector2 mapLimits);
void updatePlayer(Unit* player);  // temporary function for messing around
void updateUnitsBehaviour(Unit* units, int numberOfUnits);
void updateUnitsMovement(Unit* units, int numberOfUnits);

void UpdateGameplayScreen(void)
{
    // Camera
    Camera2D camera = { .target = (Vector2){0, 0}, .zoom = 1.0f };
    Rectangle renderingRectangle = { 0, 0, (GetScreenWidth() / TILE_SIZE), (GetScreenHeight() / TILE_SIZE) };

    // Timer
    bool pauseState = 0;
    Timer universalTimer = { 0 };
    int ticker = 0; // for counting seconds
    StartTimer(&universalTimer, SECOND);

    // Mouse
    Vector2 mousePosCameraAdjusted = { 0 }; // will allow GUI interaction
    Vector2 mousePosGridLocked = { 0 };  // will allow world data interaction. may not actually be necessary... 
    Vector2 cameraMouseDifference = { 0 };
    Vector2 saveMousePos = { 0 };  // allows us to draw a rectangle on screen as a visual aid when selecting screen elements

    // GUI elements
    int numberOfButtons = 3;
    Rectangle* buttons = initialiseButtons(numberOfButtons);

    Color buttonColour = RED;

    // World elements
    Vector2 mapLimits = { .y = MAP_ROWS,  // rows
                          .x = MAP_COLUMNS  // columns
                        };
    
    int* map = returnMapData(mapLimits);

    UnitArchetype* unitArchetypes = initialiseUnitArchetypes();
    int numberOfUnits = 1;
    Unit* units = initialiseUnitArray(numberOfUnits);
    units[0] = initialiseUnit(unitArchetypes, STANDARD);

    RoomArchetype* roomArchetypes = initialiseRoomArchetypes();
    int numberOfRooms = 0;
    int roomAllowance = 4;  // we will have to increase this as required
    Room* rooms = initialiseRoomData(roomAllowance);
    int placeRoomState = -1;
    
    bool buttonClicked;
    Texture2D dirtTile = LoadTexture("assets/dirtTile.png");

    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        mousePosCameraAdjusted.x = GetMousePosition().x + cameraMouseDifference.x;
        mousePosCameraAdjusted.y = GetMousePosition().y + cameraMouseDifference.y;
        mousePosGridLocked.x = floorf(mousePosCameraAdjusted.x / TILE_SIZE);
        mousePosGridLocked.y = floorf(mousePosCameraAdjusted.y / TILE_SIZE);

        if (!(pauseState)) UpdateTimer(&universalTimer);  // with every single cycle, timer->Lifetime is deducted by GetFrameTime as long as it is greater than 0
        if (TimerDone(&universalTimer)) {
            StartTimer(&universalTimer, SECOND);
            ticker++;
        }

        // Player input
        updateCamera(&camera, &cameraMouseDifference, &renderingRectangle, mapLimits);
        updatePlayer(&units[0]);
        updateUnitsBehaviour(units, numberOfUnits);
        updateUnitsMovement(units, numberOfUnits);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
        {
            saveMousePos = GetMousePosition();
            buttonClicked = false;

            for (int i = 0; i < numberOfButtons; i++) {
                if (CheckCollisionPointRec(GetMousePosition(), buttons[i])) {
                    if (placeRoomState == i) placeRoomState = -1; // if player is clicking on THE SAME button, we toggle place room state. 
                    else placeRoomState = i; // OTHERWISE we switch placeroomstate to the value that corresponds with the new button was clicked

                    buttonClicked = true;
                    break;  // collision detected - stop iterating through buttons
                }
                if (i == numberOfButtons - 1) buttonClicked = false;  // iterated through all buttons and no collision detected
            }
            
            if ((placeRoomState != -1) && !buttonClicked) {  // button click is active
                rooms[numberOfRooms] = initialiseSingleRoom(placeRoomState, roomArchetypes);
                createRoom(rooms, &numberOfRooms, &mousePosGridLocked, map);
            }
        }


        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) 
        {
            Vector2 startCell = { floorf(units[0].position.x / TILE_SIZE), 
                                  floorf(units[0].position.y / TILE_SIZE) };

            if (units[0].waypoint.node != NULL) free(units[0].waypoint.node);
            units[0].waypoint = aStar(startCell, mousePosGridLocked, map);
        }

        if (IsKeyPressed(KEY_P)) { if (pauseState) pauseState = 0; else pauseState = 1; };

        BeginDrawing();

            ClearBackground(RAYWHITE);

            // all world objects should be rendered inside here. whereas GUI should follow, outside
            BeginMode2D(camera);

            DrawRectangleLines(renderingRectangle.x * TILE_SIZE, renderingRectangle.y * TILE_SIZE, renderingRectangle.width * TILE_SIZE, renderingRectangle.height * TILE_SIZE, BLUE);

            DrawTexturePro(dirtTile, (Rectangle){0, 0, dirtTile.width, dirtTile.height}, (Rectangle){250, 250, 250, 250} , (Vector2){0, 0}, 0, WHITE);

            // Rendering only what is inside renderingRectangle location and dimensions
            for (int i = (int){renderingRectangle.y}; i < ((int){renderingRectangle.y} + (int){renderingRectangle.height}); i++) {
                for (int j = (int){renderingRectangle.x}; j < ((int){renderingRectangle.x} + (int){renderingRectangle.width}); j++)
                {
                    if (CheckCollisionPointRec(mousePosGridLocked, (Rectangle){j, i, 1, 1}))
                    {
                        DrawRectangleLines(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, GREEN);
                        // if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        //     map[i * (int){mapLimits.x} + j] = 0;
                        // };
                        // if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                        //     map[i * (int){mapLimits.x} + j] = 2;
                        // };
                    }

                    if (map[i * (int){mapLimits.x} + j] != 0) {
                        DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, MAROON);
                        DrawRectangleLines(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, GRAY);
                        //DrawTexturePro(dirtTile, (Rectangle){0, 0, dirtTile.width, dirtTile.height}, (Rectangle){j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE} , (Vector2){0, 0}, 0, WHITE);

                        if (CheckCollisionPointRec(units[0].position, (Rectangle){j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE})) {
                            DrawRectangleLines(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);
                        }
                    }
                    if (map[i * (int){mapLimits.x} + j] == 2) {
                        DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);
                        DrawRectangleLines(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLACK);
                    }
                }
            }
            
            // DRAWING ROOMS
            if (!IsKeyDown(KEY_SPACE))  // debugging
            {
                for (int i  = 0; i < numberOfRooms; i++) 
                {
                    DrawTexture(rooms[i].consts->atlas, rooms[i].x * TILE_SIZE, rooms[i].y * TILE_SIZE, WHITE); 
                }
                if (placeRoomState != -1) {  // button has been clicked.
                    DrawTexture(roomArchetypes[placeRoomState].atlas, 
                    (mousePosGridLocked.x - (roomArchetypes[placeRoomState].atlas.width / TILE_SIZE) / 2) * TILE_SIZE,
                    (mousePosGridLocked.y - (roomArchetypes[placeRoomState].atlas.height / TILE_SIZE) / 2) * TILE_SIZE,
                    (Color){255, 255, 255, 190}); 
                }
            }

            // Draw units
            DrawTexture(units[0].consts->atlas, units[0].position.x, units[0].position.y, WHITE);
            if (units[0].selected) DrawRectangleLines(units[0].position.x, units[0].position.y, units[0].sprite.width, units[0].sprite.height, ORANGE);

            // print tiles when waypoint set 
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

            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
            {
                if (saveMousePos.x < GetMousePosition().x) DrawRectangleLines(saveMousePos.x, saveMousePos.y, (GetMousePosition().x - saveMousePos.x), (GetMousePosition().y - saveMousePos.y), YELLOW);
                else DrawRectangleLines(GetMousePosition().x, saveMousePos.y, abs(GetMousePosition().x - saveMousePos.x), (GetMousePosition().y - saveMousePos.y), YELLOW);
            }

            // render buttons
            for (int i = 0; i < numberOfButtons; i++) DrawRectangle(buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height, buttonColour);
            
            DrawFPS(10, 10);
            // DrawText(TextFormat("mousepos= x%f | %f, y%f | %f", mousePosGridLocked.x, mousePosCameraAdjusted.x, mousePosGridLocked.y, mousePosCameraAdjusted.y), 30, 40, 30, LIGHTGRAY);
            DrawText(TextFormat("rooms[0] = x%fy%f", rooms[0].x, rooms[0].y ), 30, 75, 30, LIGHTGRAY);
            DrawText(TextFormat("rooms[1] = x%fy%f", rooms[1].x, rooms[1].y ), 30, 105, 30, LIGHTGRAY);
            DrawText(TextFormat("rooms[2] = x%fy%f", rooms[2].x, rooms[2].y ), 30, 130, 30, LIGHTGRAY);
            DrawText(TextFormat("no. of rooms = %d", numberOfRooms ), 30, 165, 30, LIGHTGRAY);


            if (units[0].waypoint.node != NULL) DrawText(TextFormat("units[i].waypoint.node[units[i].waypoint.numberOfTiles - 1].position.x = %f", units[0].waypoint.node[units[0].waypoint.numberOfTiles - 1].position.x ), 30, 750, 30, BLACK);

        EndDrawing();
    }

    if (units[0].consts->atlas.id) UnloadTexture(units[0].consts->atlas);

    for (int i = 0; i < roomAllowance; i++) if (rooms[i].x) UnloadTexture(rooms[0].consts->atlas);

    UnloadTexture(dirtTile);

    // Free memory
    free(map);
    free(rooms);
    free(unitArchetypes);
    free(roomArchetypes);
    free(buttons);

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
    if (IsKeyDown(KEY_UP)) { player->position.y -= GetFrameTime() * PLAYER_MOVEMENT_SPEED; };
    if (IsKeyDown(KEY_LEFT)) { player->position.x -= GetFrameTime() *  PLAYER_MOVEMENT_SPEED; };
    if (IsKeyDown(KEY_DOWN)) { player->position.y += GetFrameTime() * PLAYER_MOVEMENT_SPEED; };
    if (IsKeyDown(KEY_RIGHT)) { player->position.x += GetFrameTime() * PLAYER_MOVEMENT_SPEED; };
}


void updateUnitsBehaviour(Unit* units, int numberOfUnits)
{

    // if unit is hungry, eat until ?%
    // if unit is thirsty, drink until ?%
    // 
    // if unit has a work order, work

}


void updateUnitsMovement(Unit* units, int numberOfUnits) 
{
    float step = GetFrameTime() * PLAYER_MOVEMENT_SPEED;

    for (int i = 0; i < numberOfUnits; i++)
    {
        // Ignoring units that do not have a path to follow
        if (!(units[i].waypoint.numberOfTiles)) continue;

        if (units[i].position.x < (units[i].waypoint.node[units[i].waypoint.numberOfTiles - 1].position.x ) * TILE_SIZE ) {
            units[i].position.x += step; }
        if (units[i].position.x > (units[i].waypoint.node[units[i].waypoint.numberOfTiles - 1].position.x ) * TILE_SIZE ) {
            units[i].position.x -= step; }
        if (units[i].position.y < (units[i].waypoint.node[units[i].waypoint.numberOfTiles - 1].position.y ) * TILE_SIZE ) {
            units[i].position.y += step; }
        if (units[i].position.y > (units[i].waypoint.node[units[i].waypoint.numberOfTiles - 1].position.y ) * TILE_SIZE ) {
            units[i].position.y -= step; }
        
        // Recognise whether unit has reached its target, in which case, decriment numberOfTiles
        if ((fabsf(units[i].position.x - units[i].waypoint.node[units[i].waypoint.numberOfTiles - 1].position.x * TILE_SIZE) < step)
        && (fabsf(units[i].position.y - units[i].waypoint.node[units[i].waypoint.numberOfTiles - 1].position.y * TILE_SIZE) < step))
        {
            units[i].waypoint.numberOfTiles--;
        }

    }
}
