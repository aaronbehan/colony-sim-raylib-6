#include <stdlib.h>  // for malloc/calloc

// create a function that either takes in csv of map data or, if that's not present, default to randomly generating it 


// pathfinding

// quad tree 

int* returnMapData(int rows, int columns) 
{
    int *mapData = calloc((rows * columns), sizeof(int));  

    for (int i = (rows * columns / 2); i < rows * columns; i++)
    {
        mapData[i] = 1;
    }
    
    return mapData;
}