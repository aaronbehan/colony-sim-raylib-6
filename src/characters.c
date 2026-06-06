#include <stdlib.h>

#include "include/raylib.h"

#include "header.h"


Unit* returnUnitData(void)
{
    int units = 1;

    Unit *unitData = calloc((units), sizeof(int));  

    unitData[0].position = (Vector2){30, 40};

    return unitData;
}

// ------ 

void behaviourTree(Unit* unit) 
{
    if(unit->needs.hunger < 20)
    {
        
        // // update waypoint to nearest food coords
        // if (unit->position.position.x < units[i].waypoint.x) units[i].position.x += units[i].archetype->speed;
        // if (units[i].position.x > units[i].waypoint.x) units[i].position.x -= units[i].archetype->speed;
        // if (units[i].position.z < units[i].waypoint.z) units[i].position.z += units[i].archetype->speed;
        // if (units[i].position.z > units[i].waypoint.z) units[i].position.z -= units[i].archetype->speed;

    }
}