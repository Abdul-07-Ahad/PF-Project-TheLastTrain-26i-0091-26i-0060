#include "physics.h"
#include "util.h"
#include "switches.h"
// Movement validation and resource adjustments without structures
void applyResourceDelta(int& energy, int& time, int deltaEnergy, int deltaTime) {
    energy = clampInt( energy + deltaEnergy, 0, MAX_ENERGY);
    time   = clampInt( time + deltaTime, 0, MAX_TIME);
}
void applyScoreDelta(int& score, int deltaScore) {
    score = score + deltaScore;
}
bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, char itemSymbol) {
    for (int i = 0; i < inventoryCount ; i++) {
        if ( inventory[i] == itemSymbol ) {
            return true;
        }
    }
    return false;
}
bool addItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol) {
    if ( inventoryCount >= MAX_INVENTORY_CAPACITY ) {                      // the inventory is already full
        return false;
    }
    inventory[inventoryCount] = itemSymbol;    // the next free slot
    inventoryCount++;                             // one more item
    return true;
}
bool removeItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol) {
    for (int i = 0; i < inventoryCount; i++) {
        if ( inventory[i] == itemSymbol ) {
            // move every later item one slot to the left
            for (int j = i; j < inventoryCount - 1; j++) {
                inventory[j] = inventory[j + 1];
            }
            inventoryCount--;
            return true;
        }
    }
    return false;
}
bool isDirectlyConnected(int fromF, int fromR, int toF, int toR,
                         const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                         char& outConnType) {
    outConnType = CONN_NONE;

    // 1. any number outside the building: no link
    if ( fromF < 1 || fromF > NUM_FLOORS || fromR < 1 || fromR > ROOMS_PER_FLOOR ||
        toF < 1 || toF > NUM_FLOORS || toR < 1 || toR > ROOMS_PER_FLOOR ) {
        return false;
    }

    // 2. rooms next to each other on the same floor
    if (fromF == toF && (toR - fromR == 1 || fromR - toR == 1)) {
        int gap = (fromR < toR) ? fromR - 1 : toR - 1;
        outConnType = horizontalConn[fromF - 1][gap];
    }

    // 3. stairs: only between room 1 of the two floors            <-- FIX 1
    if (fromR == 1 && toR == 1) {
        if ( (fromF == 1 && toF == 2) || (fromF == 2 && toF == 1) ||
             (fromF == 2 && toF == 3) || (fromF == 3 && toF == 2) ||
             (fromF == 4 && toF == 5) || (fromF == 5 && toF == 4) ) {
            outConnType = CONN_STAIRS;
        }
    }

    // 4. lift 1: floors 1 and 2, rooms 2 or 3 on both floors
    if ( (fromF == 1 && toF == 2 && fromR >= 2 && fromR <= 3 && toR >= 2 && toR <= 3) ||
         (fromF == 2 && toF == 1 && fromR >= 2 && fromR <= 3 && toR >= 2 && toR <= 3) ) {
        outConnType = CONN_LIFT1;
    }

    // 5. lift 2: room 4, between floors 2, 4 and 5                <-- FIX 3
    if ( (fromF == 2 && toF == 4 && fromR == 4 && toR == 4) ||
         (fromF == 4 && toF == 2 && fromR == 4 && toR == 4) ||
         (fromF == 4 && toF == 5 && fromR == 4 && toR == 4) ||
         (fromF == 5 && toF == 4 && fromR == 4 && toR == 4) ||
         (fromF == 2 && toF == 5 && fromR == 4 && toR == 4) ||
         (fromF == 5 && toF == 2 && fromR == 4 && toR == 4) ) {
        outConnType = CONN_LIFT2;
    }

    return outConnType != CONN_NONE && outConnType != CONN_SOLID_WALL;   // <-- FIX 2
}
bool canTraverse(int targetFloor, int targetRoom, char connType,
                 const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                 int emergencyTripsLeft, bool lift2Operational){
    (void)targetFloor;
    (void)targetRoom;

    if (connType == CONN_DOOR ||connType == CONN_WINDOW ||connType == CONN_STAIRS ||connType == CONN_LIFT1){
        return true;
    }
    if(connType == CONN_LOCKED_DOOR){
        return unlockFloor3Door(inventory, inventoryCount);
    }
    if (connType == CONN_LIFT2){
        return lift2Operational || emergencyTripsLeft>0;
    }
    return false;
}