#include <iostream>
#include "viewer_iface.h"
using namespace std;

int main() {
    char inv[MAX_INVENTORY_CAPACITY] = {ENT_MASTER_KEY, ENT_FUSE};
    renderDashboard(3, 2, 85, 40, 30, inv, 2, 3);
    cout << endl;
    renderDashboard(1, 1, 100, 50, 0, inv, 0, 0);  
char rooms[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM] = {};
rooms[2][1][0] = ENT_CLUE_4;   // F3-R2
rooms[2][1][1] = ENT_FUSE;
renderRoomDetails(3, 2, rooms, 2);
renderRoomDetails(1, 1, rooms, 0);
bool obj[NUM_OBJECTIVES] = {};
obj[OBJ_MASTER_KEY] = true;
obj[OBJ_CLUE_2] = true;
obj[OBJ_FUSE_INSTALLED] = true;
renderObjectivesChecklist(obj);
renderAsciiBuilding(3, 2);
renderAsciiBuilding(1, 1);
    return 0;
}