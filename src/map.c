#include <stdlib.h>  // for malloc/calloc

#include "raylib.h"

// create a function that either takes in csv of map data or, if that's not present, default to randomly generating it 


// pathfinding

typedef struct PFNode {
    Vector2 position;
    float gCost, hCost, fCost; // Costs
    struct PFNode* parent; // Parent node for path reconstruction
    int walkable; // 1 if walkable, 0 if not
} PFNode;

typedef struct PFPath {
    //PlaneVector* path;  //  can reduce it to planeVector later on
    PFNode* node;
    int numberOfTiles;
} PFPath;


// PFPath path = astar(startPos, targetPos, walls, numberOfWalls);


int* returnMapData(Vector2 mapLimits) 
{
    //numbers up to 10 for example could be foreground related. these would obscure the background. 01, 02, 03, 04 to make the data set easier to read when double digits naturally occur
    //numbers after 10 would make the tile walkable but would have a certain background.

    // mapLimits.y = rows
    // mapLimits.x = columns 

    int *mapData = calloc(((int){mapLimits.x} * (int){mapLimits.y}), sizeof(int));  

    for (int i = (mapLimits.y * mapLimits.x / 2); i < mapLimits.y * mapLimits.x; i++)
    {
        mapData[i] = 1;
    }
    
    return mapData;
}