#include <iostream>
#include "simulator.h"
#include "level.h"
using namespace std;

void testPartA() {
    cout << "===== PART A =====" << endl;
    char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1];
    char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM];
    int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR] = {0};
    loadBuildingMap("data/building.txt", horizontalConn, roomEntities, entityCounts);

    char inv[MAX_INVENTORY_CAPACITY] = {};
    int invCount = 0;
    bool obj[NUM_OBJECTIVES] = {};
    int energy = 50, time = 20, score = 0;
    bool r;

    r = handleCollectCommand(ENT_ENERGY_PACK, 1, 4, energy, time, score, roomEntities, entityCounts[0][3], inv, invCount, obj);
    cout << r << " E" << energy << " T" << time << " left=" << entityCounts[0][3] << endl;
    r = handleCollectCommand(ENT_ENERGY_PACK, 1, 4, energy, time, score, roomEntities, entityCounts[0][3], inv, invCount, obj);
    cout << r << " E" << energy << " T" << time << endl;
    r = handleCollectCommand(ENT_EMERGENCY_TOOLKIT, 2, 3, energy, time, score, roomEntities, entityCounts[1][2], inv, invCount, obj);
    cout << r << " E" << energy << " T" << time << endl;
    r = handleCollectCommand(ENT_OBSERVATION_TOKEN, 5, 4, energy, time, score, roomEntities, entityCounts[4][3], inv, invCount, obj);
    cout << r << " E" << energy << " T" << time << endl;
    r = handleCollectCommand(ENT_CLUE_4, 3, 2, energy, time, score, roomEntities, entityCounts[2][1], inv, invCount, obj);
    cout << r << " E" << energy << " T" << time << endl;
}

void testPartB() {
    cout << "===== PART B =====" << endl;
    char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1];
    char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM];
    int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR] = {0};
    loadBuildingMap("data/building.txt", horizontalConn, roomEntities, entityCounts);

    char inv[MAX_INVENTORY_CAPACITY] = {};
    int invCount = 0;
    bool obj[NUM_OBJECTIVES] = {};
    int energy = 50, time = 20, score = 0;
    bool r;

    r = handleCollectCommand(ENT_MASTER_KEY, 1, 2, energy, time, score, roomEntities, entityCounts[0][1], inv, invCount, obj);
    cout << r << " E" << energy << " T" << time << " S" << score << " inv=" << invCount << " key=" << obj[OBJ_MASTER_KEY] << " left=" << entityCounts[0][1] << endl;

    r = handleCollectCommand(ENT_FUSE, 3, 2, energy, time, score, roomEntities, entityCounts[2][1], inv, invCount, obj);
    cout << r << " E" << energy << " T" << time << " S" << score << " inv=" << invCount << " fuse=" << obj[OBJ_FUSE_COLLECTED] << endl;

    r = handleCollectCommand(ENT_LIFT_PASS, 3, 4, energy, time, score, roomEntities, entityCounts[2][3], inv, invCount, obj);
    cout << r << " E" << energy << " T" << time << " S" << score << " inv=" << invCount << " pass=" << obj[OBJ_LIFT_PASS] << endl;

    r = handleCollectCommand(ENT_CAT_CARRIER, 5, 3, energy, time, score, roomEntities, entityCounts[4][2], inv, invCount, obj);
    cout << r << " E" << energy << " T" << time << " S" << score << " inv=" << invCount << " carrier=" << obj[OBJ_CAT_CARRIER] << endl;

    char fullInv[MAX_INVENTORY_CAPACITY];
    for (int i = 0; i < MAX_INVENTORY_CAPACITY; i++) fullInv[i] = ENT_ENERGY_PACK;
    int fullCount = MAX_INVENTORY_CAPACITY;
    r = handleCollectCommand(ENT_MEDICAL_KIT, 4, 2, energy, time, score, roomEntities, entityCounts[3][1], fullInv, fullCount, obj);
    cout << r << " E" << energy << " T" << time << " inv=" << fullCount << " left=" << entityCounts[3][1] << endl;
}

int main() {
    testPartA();
    cout << endl;
    testPartB();
    return 0;
}