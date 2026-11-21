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
    int energy = 100, time = 50, score = 0;

    handleInspectCommand(3, 2, energy, time, roomEntities, entityCounts[2][1], obj, score);
    cout << "E" << energy << " T" << time << " S" << score << " clue4=" << obj[OBJ_CLUE_4] << endl;
    handleInspectCommand(3, 2, energy, time, roomEntities, entityCounts[2][1], obj, score);
    cout << "E" << energy << " T" << time << " S" << score << " clue4=" << obj[OBJ_CLUE_4] << endl;
    handleInspectCommand(1, 1, energy, time, roomEntities, entityCounts[0][0], obj, score);
    cout << "E" << energy << " T" << time << " S" << score << endl;
    handleInspectCommand(4, 3, energy, time, roomEntities, entityCounts[3][2], obj, score);
    cout << "E" << energy << " T" << time << " S" << score << " clue2=" << obj[OBJ_CLUE_2] << endl;
    handleInspectCommand(5, 1, energy, time, roomEntities, entityCounts[4][0], obj, score);
    cout << "E" << energy << " T" << time << " S" << score << " clue7=" << obj[OBJ_CLUE_7] << endl;
    bool ok = handleInspectCommand(9, 1, energy, time, roomEntities, 0, obj, score);
    cout << ok << " E" << energy << endl;
    return 0;
}