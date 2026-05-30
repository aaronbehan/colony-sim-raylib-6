#include <stdlib.h>

#include "include/raylib.h"

#include "header.h"


Unit* returnUnitData(void)
{
    int units = 1;

    Unit *unitData = calloc((units), sizeof(int));  

    return unitData;
}

// ------ 

void behaviourTree(Unit* unit) 
{
    if(unit->needs.hunger < 20)
    {
        // update waypoint to nearest food coords
    }


}