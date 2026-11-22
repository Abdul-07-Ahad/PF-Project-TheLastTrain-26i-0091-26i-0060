#include <iostream>
#include "logger.h"
using namespace std;

int main() {
    char inv[MAX_INVENTORY_CAPACITY] = {ENT_MASTER_KEY, ENT_FUSE};
    bool obj[NUM_OBJECTIVES] = {};
    obj[OBJ_MASTER_KEY] = true;
    obj[OBJ_FUSE_COLLECTED] = true;
    char rooms[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM] = {};
    int counts[NUM_FLOORS][ROOMS_PER_FLOOR] = {0};
    rooms[0][1][0] = ENT_MASTER_KEY;     // F1-R2
    counts[0][1] = 1;
    rooms[2][1][0] = ENT_CLUE_4;         // F3-R2
    rooms[2][1][1] = ENT_FUSE;
    counts[2][1] = 2;  
    bool ok = saveGameStateToFile("out/test-save.txt", 3, 2, 85, 40, 30,
                                  inv, 2, obj, 1, rooms, counts);
    cout << ok << endl;
    return 0;
}