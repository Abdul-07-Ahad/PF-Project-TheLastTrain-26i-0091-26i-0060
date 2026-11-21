#include <iostream>
#include "simulator.h"
#include "level.h"
using namespace std;

int main() {
    char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1];
    char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM];
    int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR] = {0};
    loadBuildingMap("data/building.txt", horizontalConn, roomEntities, entityCounts);

    bool obj[NUM_OBJECTIVES] = {};
    int score = 0;
    bool r;

    r = handleUnlockCabinetCommand("427", 1, 1, obj, score, roomEntities, entityCounts[0][0]);
    cout << r << " S" << score << " cab=" << obj[OBJ_CABINET_UNLOCKED] << " key=" << obj[OBJ_TRAIN_KEY] << " left=" << entityCounts[4][0] << endl;

    r = handleUnlockCabinetCommand("427", 5, 1, obj, score, roomEntities, entityCounts[4][0]);   // no clues yet
    cout << r << " S" << score << " cab=" << obj[OBJ_CABINET_UNLOCKED] << " key=" << obj[OBJ_TRAIN_KEY] << " left=" << entityCounts[4][0] << endl;

    obj[OBJ_CLUE_4] = obj[OBJ_CLUE_2] = obj[OBJ_CLUE_7] = true;

    r = handleUnlockCabinetCommand("123", 5, 1, obj, score, roomEntities, entityCounts[4][0]);   // wrong code
    cout << r << " S" << score << " cab=" << obj[OBJ_CABINET_UNLOCKED] << " key=" << obj[OBJ_TRAIN_KEY] << " left=" << entityCounts[4][0] << endl;

    r = handleUnlockCabinetCommand("427", 5, 1, obj, score, roomEntities, entityCounts[4][0]);   // success
    cout << r << " S" << score << " cab=" << obj[OBJ_CABINET_UNLOCKED] << " key=" << obj[OBJ_TRAIN_KEY] << " left=" << entityCounts[4][0] << endl;

    r = handleUnlockCabinetCommand("427", 5, 1, obj, score, roomEntities, entityCounts[4][0]);   // again
    cout << r << " S" << score << " cab=" << obj[OBJ_CABINET_UNLOCKED] << " key=" << obj[OBJ_TRAIN_KEY] << " left=" << entityCounts[4][0] << endl;
    return 0;
}