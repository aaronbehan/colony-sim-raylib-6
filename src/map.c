#include <stdlib.h>  // for malloc/calloc/realloc
#include <math.h>    // for sqrtf

#include "raylib.h"
#include "header.h"

// create a function that either takes in csv of map data or, if that's not present, default to randomly generating it 


// pathfinding

// Function to find the node with the lowest fCost in the open list. A star prioritises node with lowest fCost. Compatible.
PFNode* findLowestFCostNode(PFNode** openList, int openListSize) // whenever an array is passed as an argument, the array decays to a pointer to its first element. hence the pointer to a pointer PFNode**
{
    PFNode* lowest = openList[0];  // asserts that the lowest F cost is openList[0]
    for (int i = 1; i < openListSize; i++) // then checks that assertion against all other F costs in openList, starting at index 1
    {
        if (openList[i]->fCost < lowest->fCost || (openList[i]->fCost == lowest->fCost && openList[i]->hCost < lowest->hCost)) {  // lower H cost is the deciding factor when considering F costs of equivalent values
            lowest = openList[i];  // reassigns lowest to the newly discovered lower F cost
        }
    }
    return lowest;
}

// Reconstruct the path from the end node to the start node. Compatible. 
PFPath reconstructPath(PFNode* endNode) 
{
    PFPath path;
    path.numberOfTiles = 0;

    PFNode* current = endNode;
    while (current != NULL) // The loop is purely to count path.numberOfTiles variable
    {
        current = current->parent; // the parent of the very first node will be null
        path.numberOfTiles++;
    }

    // creating a DEEP copy! that means we can free all other memory used to get to this point
    path.node = (PFNode*)malloc(path.numberOfTiles * sizeof(PFNode));  // path.node is a pointer struct member and we must declare how many nodes go into it
    
    current = endNode;
    for (int i = 0; i < path.numberOfTiles; i++) // Populating array with nodes going from end to start
    {
        path.node[i] = *current;  // using pointers to each successive node from end to start
        current = current->parent; 
    }

    for (int i = 0; i < path.numberOfTiles / 2; i++) // Reversing the variable so that it's easy for a unit to follow
    {
        PFNode temp = path.node[i];
        path.node[i] = path.node[path.numberOfTiles - 1 - i];
        path.node[path.numberOfTiles - 1 - i] = temp;
    }

    return path;
}

// Get neighbors of a node
int getNeighbors(int* mapData, PFNode* currentNode, PFNode** neighbors) {

    int count = 0;
    
    for (int dx = -1; dx <= 1; dx++) {  // starts at the west corner
        for (int dy = -1; dy <= 1; dy++) {  
            if (dx == 0 && dy == 0) continue; // Skip currentNode

            int nx = currentNode->position.x + dx;  // taking currentNode.x, adding dx (which could be a negative number) to it   and then storing it in nx
            int ny = currentNode->position.y + dy;  // taking currentNode.y, adding dy to it   and then storing it in ny

            // Checking whether we have reached the edge of the map data
            if (nx < 0 || nx >= MAP_COLUMNS ||
                ny < 0 || ny >= MAP_ROWS)
            {
                continue;
            }

            // row * cols + col
            if (mapData[(ny * MAP_COLUMNS) + nx] == 0) // here, i am checking the position around currentNode inside mapData for walls (which are represented by 1 inside the array)
            {
                PFNode* neighborNode = (PFNode*)calloc(1, sizeof(PFNode));  // initialise the node. apparently very expensive to calloc every time. probably stupid. chatgpt implies i should abandon the idea
                neighborNode->position.x = nx;
                neighborNode->position.y = ny;

                neighbors[count++] = neighborNode;  // and add it to neighbours array while incrementing count by 1
            }
        }
    }

    return count;
}

// Checks if the node is present in the list
int isNodeInList(PFNode** list, int listSize, PFNode* node) 
{
    for (int i = 0; i < listSize; i++) 
    {
        if (list[i]->position.x == node->position.x && list[i]->position.y == node->position.y) {  // this can be simplified through use of vector struct
            return 1;
        }
    }
    return 0;
}

// Function to calculate the heuristic (Euclidean distance)
float calculateHeuristic(PFNode* a, PFNode* b) {
    return sqrtf((a->position.x - b->position.x) * (a->position.x - b->position.x) + (a->position.y - b->position.y) * (a->position.y - b->position.y));
}

PFPath aStar(Vector2 currentPos, Vector2 targetPos, int* mapData)
{
    // Initialising start and end nodes
    PFNode* startNode = (PFNode*)malloc(sizeof(PFNode));  // don't forget to free this and the endNode!
    startNode->position.x = currentPos.x;
    startNode->position.y = currentPos.y;
    startNode->gCost = 0.0f;
    startNode->hCost = 0.0f;
    startNode->fCost = 0.0f;
    startNode->parent = NULL;
    
    PFNode* endNode = (PFNode*)malloc(sizeof(PFNode));
    endNode->position.x = targetPos.x;
    endNode->position.y = targetPos.y;
    endNode->gCost = 0.0f;
    endNode->hCost = 0.0f;
    endNode->fCost = 0.0f;
    endNode->parent = NULL;

    PFPath path = { 0 };  // the path i can expect to return upon finding endNode

    int openListAllowance = 20;  // DELIBERATELY QUITE LOW SO THAT YOU REMEMBER TO FIX THIS! the idea is that if this allowance is exceeded, then we manually allocate more, but this has not been done yet
    int closedListAllowance = 20;

    PFNode** openList = (PFNode**)malloc(sizeof(PFNode*) * openListAllowance);
    int openListSize = 0;

    PFNode** closedList = (PFNode**)malloc(sizeof(PFNode*) * closedListAllowance);  
    int closedListSize = 0;

    openList[openListSize++] = startNode;  // putting the first node into the start list and then incrementing by 1

    while (openListSize > 0) // loop will not stop until it has exhausted all nodes
    {
        PFNode* currentNode = findLowestFCostNode(openList, openListSize);  // searches through openList and assigns node with lowest F cost to current node

        // Remove currentNode from openList
        for (int i = 0; i < openListSize; i++) // iterates through openList
        {
            if (openList[i] == currentNode) // it finds the currentNode inside openList
            {
                openList[i] = openList[--openListSize];  // it takes the very last element in openList and puts it at the address currentNode was found at
                break;
            } // i get that you're concerned about the ACTUAL CONTENTS of the list, but, because openlistsize is decremented (momentarily) to 0, it is basically the same as being empty because the next value to be added to the list will be at index 0 because it has been reduced to this value on the first iteration of the root for loop
        }
        
        // Before adding current node to closedList, check if memory has been exceeded and add more if necessary
        if ((closedListSize + 1) >= closedListAllowance)  
        {
            closedListAllowance = 2 * closedListSize;
            PFNode** temp = realloc(closedList, sizeof(PFNode*) * (closedListAllowance));  // not sure whether PFNode needs to be a pointer or a pointer to a pointer
            if (temp) closedList = temp;
        }
        closedList[closedListSize++] = currentNode;  // adds currentNode to closedList and increases closedListSize by 1
            
        if ((currentNode->position.x == endNode->position.x) && (currentNode->position.y == endNode->position.y))  // exit function if currentNode has reached endNode coordinates
        {
            path = reconstructPath(currentNode);  // make sure that this is a deep copy of path data

            // free all memory
            for (int i = 0; i < openListSize; i++) free(openList[i]);
            free(openList);

            for (int i = 0; i < closedListSize; i++) free(closedList[i]);   // where the error occurs!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            free(closedList);

            return path;
        }

        PFNode* neighbors[8];  // creates a pointer to an array of 8 neighbours
        int neighborCount = getNeighbors(mapData, currentNode, neighbors);  // stores 8 nodes (that neighbour currentNode) in neighbours array and returns number of neighbours created. only neighbours that are out of bounds or inside a wall do not get created

        for (int i = 0; i < neighborCount; i++) // iterates through the newly made neighbours array
        {
            PFNode* neighbor = neighbors[i];

            if (isNodeInList(closedList, closedListSize, neighbor)) {
                free(neighbor);
                continue;  // skip everything below it and move onto the next iteration
            }

            float tentativeGCost = currentNode->gCost + calculateHeuristic(currentNode, neighbor);  // this does work for getting the g cost. you take the g cost from parent node and add 1 or 1.4. i'm guessing these values are returned from the calculateH func

            if (!isNodeInList(openList, openListSize, neighbor)) // if node is not in the openList: add it to the open list. !!!!!!! significant step!
            {  
                if ((openListSize + 1) >= openListAllowance) { // checking if more memory is needed
                    openListAllowance = 2 * openListSize;
                    PFNode** temp = realloc(openList, sizeof(PFNode*) * (openListAllowance));  // is this honestly right?
                    if (temp) openList = temp;
                }
                openList[openListSize++] = neighbor;
            } 
            // This needs looking into
            else if (tentativeGCost >= neighbor->gCost) {  // if node is in the list and the newly calculated gcost is greater than its previously calculated gcost, skip to next iteration
                free(neighbor);
                continue;
            }

            // initialise its members 
            neighbor->gCost = tentativeGCost;
            neighbor->hCost = calculateHeuristic(neighbor, endNode);
            neighbor->fCost = neighbor->gCost + neighbor->hCost;
            neighbor->parent = currentNode;
        }
    }

    return path;  // this just mitigates an annoying warning
}




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