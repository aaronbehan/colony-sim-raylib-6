#include <stdlib.h>  // for malloc/calloc

// create a function that either takes in csv of map data or, if that's not present, default to randomly generating it 


// pathfinding

// quad tree 

int* returnMapData(int rows, int columns) 
{
    //numbers up to 10 for example could be foreground related. these would obscure the background. 01, 02, 03, 04 to make the data set easier to read when double digits naturally occur
    //numbers after 10 would make the tile walkable but would have a certain background.

    int *mapData = calloc((rows * columns), sizeof(int));  

    for (int i = (rows * columns / 2); i < rows * columns; i++)
    {
        mapData[i] = 1;
    }
    
    return mapData;
}