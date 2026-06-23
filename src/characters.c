#include <stdlib.h>
#include <string.h>

#include "include/raylib.h"

#include "header.h"


UnitArchetype* initialiseUnitArchetypes(void)
{
    UnitArchetype dataStack[] = {

        {
            .texturePath = "assets/mouseTiny.png",
            .name = "",
        }
    };

    int sumOfArchetypes = sizeof(dataStack) / sizeof(dataStack[0]);  // Total number of bytes comprising dataStack divided by number of bytes comprising one item in the array

    UnitArchetype* dataHeap = (UnitArchetype*)malloc(sizeof(UnitArchetype) * (sumOfArchetypes));
    memcpy(dataHeap, dataStack, sizeof(dataStack));

    for (int i = 0; i < sumOfArchetypes; i++) 
    {
        dataHeap[i].atlas = LoadTexture(dataHeap[i].texturePath);
    }

    return dataHeap;
}


Unit initialiseUnit(UnitArchetype* archetypes, int archetype)
{
    Unit unit = { 0 };

    unit = (Unit){ .consts = &archetypes[archetype] };

    return unit;
}


Unit* initialiseUnitArray(int numberOfUnits)
{
    Unit *unitData = calloc(numberOfUnits, sizeof(Unit));  

    return unitData;
}


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