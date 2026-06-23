#ifndef HEADER_H
#define HEADER_H

#define TILE_SIZE 16
#define MAP_ROWS 100
#define MAP_COLUMNS 100


// Types and structure definitions
typedef enum GameScreen { TITLE, GAMEPLAY } GameScreen;
typedef enum RoomType { TESTROOM1, TESTROOM2 } RoomType;
typedef enum UnitType { STANDARD } UnitType;


// Global variables
extern GameScreen currentScreen; // simply declaring that this variable exists. we define it in main.c


// Screens 

// title.c
void UpdateTitleScreen(void);

// gameplay.c
void UpdateGameplayScreen(void);


// Mechanics -------------------

// gui.c

Rectangle* initialiseButtons(int numberOfButtons);


// map.c

typedef struct PFNode {
    Vector2 position;
    float gCost; // how far the node is from the STARTING node
    float hCost; // (heuristic) how far the node is from the TARGET node
    float fCost; // gCost + hCost
    struct PFNode* parent; // Parent node for path reconstruction
    int walkable; // 1 if walkable, 0 if not
} PFNode;

typedef struct PFPath {
    PFNode* node;
    int numberOfTiles;
} PFPath;

int* returnMapData(Vector2 mapLimits);
PFPath aStar(Vector2 currentPos, Vector2 targetPos, int* mapData);


// characters.c ---------------------------------------------------------------------------------------------

typedef struct HierarchyOfNeeds {  // this dictates behaviour. if a unit is thirsty, he seeks water. if he is cold
    int thirst;
    int hunger;
    // int warmth; not so interesting
    int rest;
    // int* health; // perhaps a unit could be inflicted with various conditions?
    int socialEngagement;
} HierarchyOfNeeds;

typedef struct UnitArchetype {
    Texture2D atlas;
    char* texturePath;
    char* name;
} UnitArchetype;

// For character creation
typedef struct Unit {
    UnitArchetype* consts;
    Texture2D sprite;
    Vector2 position;  
    PFPath waypoint;

    bool selected;  // selected with the mouse

    HierarchyOfNeeds needs;

    Vector2* memory; // fixed array of vector2 pointers? this way, we can have fixed indices for remembering the location of specific things
                    // for example, the unit remembers that food was at or around {-24, -58} and food is always index 4. meanwhile, mr unit remembers
                    // that his bed is at {-41, -63} which is always at index 2. 
                    // INFORMATION EXCHANGE BETWEEN UNITS. if one unit knows where to find X, another unit can ask him and find out

    // bool* searching; // perhaps this does not actually need to be a stored variable, but rather can be dictated by the behaviour tree every frame
                        // for example, if a unit is hungry, there is no need to set a boolean searchFood value when the behaviour tree conditions will
                        // be the exact same and will trigger the unit to consult its memory for where it last saw food 

    // int *inventory;  // what the unit is currently carrying around with it
    // int *possessions;  // what the unit feels as though he owns. for example, in his room as well as in his inventory. if a possession is missing/taken, bad mood. consequences

} Unit;

UnitArchetype* initialiseUnitArchetypes(void);
Unit initialiseUnit(UnitArchetype* archetypes, int archetype);
Unit* initialiseUnitArray(int numberOfUnits);

// -----rooms.c-----------

typedef struct RoomArchetype
{
    Texture2D atlas;
    char* texturePath;
    char* name;
    //animation frames
    // int number of upgrades
    // int* to a certain value that it will likely increase? for example, library might increment an invisible or invisible research value
} RoomArchetype;

typedef struct Room {
    RoomArchetype* consts;
    int opacity;  // may need to be an unsigned char, since that's what comprises Color values
    float x;
    float y;
    float width;
    float height;
    Rectangle posAndSize;  // this will likely be necessary for animation

} Room;

RoomArchetype* initialiseRoomArchetypes(void);
Room* initialiseRoomData(int numberOfRooms);
Room initialiseSingleRoom(int roomType, RoomArchetype* roomArchetypes);
void createRoom(Room* rooms, int* numberOfRooms, Vector2* mousePosGridLocked, int* map);

// time.c ---------------------------------------------------------------------------------------------

#define SECOND 1.0f // very close to one second

typedef struct {
    float Lifetime;
}Timer;

void StartTimer(Timer* timer, float duration);
void UpdateTimer(Timer* timer);
bool TimerDone(Timer* timer);

#endif