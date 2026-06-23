#include <stdlib.h>
#include <string.h> // for memcpy
#include <math.h>

#include "include/raylib.h"

#include "header.h"


RoomArchetype* initialiseRoomArchetypes(void) 
{

    RoomArchetype dataStack[] = {

        {
            .texturePath = "assets/testRoom1.png",
            .name = "Test Room 1",
        },

        {
            .texturePath = "assets/testRoom2.png",
            .name = "Test Room 2"
        },

        {
            .texturePath = "assets/testRoom3.png",
            .name = "Test Room 3"
        }
    };

    int sumOfArchetypes = sizeof(dataStack) / sizeof(dataStack[0]);  // Total number of bytes comprising dataStack divided by number of bytes comprising one item in the array

    RoomArchetype* dataHeap = (RoomArchetype*)malloc(sizeof(RoomArchetype) * (sumOfArchetypes));
    memcpy(dataHeap, dataStack, sizeof(dataStack));

    for (int i = 0; i < sumOfArchetypes; i++) 
    {   // remember that atlas is a 2D texture, which already contains height/width data. then just divide by TILE_SIZE for its tile based dimensions 
        dataHeap[i].atlas = LoadTexture(dataHeap[i].texturePath);
    }

    return dataHeap;
}


Room* initialiseRoomData(int numberOfRooms) 
{
    Room *roomData = calloc(numberOfRooms, sizeof(Room)); 

    return roomData;
}


Room initialiseSingleRoom(int roomType, RoomArchetype* roomArchetypes)  // maybe just pass in the archetype itself, not the whole array
{
    Room room = { 0 };

    room = (Room){ .consts = &roomArchetypes[roomType] };

    return room;
}


void createRoom(Room* rooms, int* numberOfRooms, Vector2* mousePosGridLocked, int* map)
{
    int roomWidthTiles = rooms[*numberOfRooms].consts->atlas.width / TILE_SIZE;
    int roomHeightTiles = rooms[*numberOfRooms].consts->atlas.height / TILE_SIZE;

    rooms[*numberOfRooms].x = floorf(mousePosGridLocked->x - roomWidthTiles / 2);
    rooms[*numberOfRooms].y = floorf(mousePosGridLocked->y - roomHeightTiles / 2);

    // Affecting map array  (index = row * columns + column)
    int topLeftVertex = (int){(mousePosGridLocked->y - ((roomHeightTiles) / 2)) 
        * MAP_COLUMNS 
        + (mousePosGridLocked->x - ((roomWidthTiles) / 2))};

    for (int i = 0; i < (int){roomHeightTiles}; i++) {
        int leftWall = topLeftVertex + (MAP_ROWS * i);

        for (int j = leftWall; j < leftWall + roomWidthTiles; j++) { map[j] = 2; }
    }

    (*numberOfRooms)++;  // this is seriously how you have to increment a pointer... 
}

