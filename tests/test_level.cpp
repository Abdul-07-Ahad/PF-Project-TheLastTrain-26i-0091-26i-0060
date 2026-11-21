#include "level.h"
#include "util.h"
#include "state.h"
#include "config.h"
#include <iostream>

int main() {
    // ---------- test 1: the map ----------
    char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1];
    char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM];
    int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR] = { 0 };

    if (!loadBuildingMap("data/building.txt", horizontalConn, roomEntities, entityCounts)) {
        std::cout << "Could not load map\n";
        return 1;
    }
    std::cout << "--- doors ---\n";
    for (int i = 0; i < 5; i++) {
        std::cout << "Floor " << i + 1 << ": [" << horizontalConn[i][0] << "]["
                  << horizontalConn[i][1] << "][" << horizontalConn[i][2] << "]\n";
    }
    std::cout << "--- items in F5-R1 ---\n";
    std::cout << "count: " << entityCounts[4][0] << "  items: "
              << roomEntities[4][0][0] << " " << roomEntities[4][0][1] << "\n";

    // ---------- test 2: the real starting values ----------
    int playerFloor = 1, playerRoom = 1, energy = 0, time = 0, score = 0;
    char inventory[MAX_INVENTORY_CAPACITY];
    int inventoryCount = 5, emergencyTripsLeft = 3;
    bool objectives[NUM_OBJECTIVES] = { false };
    std::cout << "before: floor=" << playerFloor << " room=" << playerRoom << "\n";
    loadInitialState("data/initial-state.txt", playerFloor, playerRoom, energy, time, score,
                     inventory, inventoryCount, objectives, emergencyTripsLeft);
    std::cout << "after: floor=" << playerFloor << " room=" << playerRoom << "\n";
    std::cout << "--- initial-state.txt ---\n";
    std::cout << "energy=" << energy << " time=" << time << " score=" << score
              << " passenger=" << objectives[OBJ_PASSENGER_RESCUED]
              << " cat=" << objectives[OBJ_CAT_SECURED] << "\n"
              << " inventoryCount=" << inventoryCount << " emergencyTripsLeft=" << emergencyTripsLeft << "\n";

    // ---------- test 3: a copy of the file with PASSENGER_RESCUED: YES ----------
    bool objectives2[NUM_OBJECTIVES] = { false };
    std::cout << "before: floor=" << playerFloor << " room=" << playerRoom << "\n";
    loadInitialState("data/test-state.txt", playerFloor, playerRoom, energy, time, score,
                     inventory, inventoryCount, objectives2, emergencyTripsLeft);
    std::cout << "after: floor=" << playerFloor << " room=" << playerRoom << "\n";
    std::cout << "--- test-state.txt ---\n";
    std::cout << "passenger=" << objectives2[OBJ_PASSENGER_RESCUED]
              << " cat=" << objectives2[OBJ_CAT_SECURED] << "\n";

    // ---------- test 4: a file that does not exist ----------
    std::cout << "--- missing file ---\n";
    if (!loadInitialState("data/nope.txt", playerFloor, playerRoom, energy, time, score,
                          inventory, inventoryCount, objectives, emergencyTripsLeft)) {
        std::cout << "returned false (good)\n";
    }
    std::cout << clampInt(150, 0, 100) << " " << clampInt(-5, 0, 100) << " " << clampInt(42, 0, 100) << "\n";
    std::cout << getEntityName(ENT_FUSE) << " | " << getEntityName('Z') << "\n";
    std::cout << validateStartingRoom(3, 2 , horizontalConn) << " "
          << validateStartingRoom(0, 1, horizontalConn) << " "
          << validateStartingRoom(6, 1, horizontalConn) << " "
          << validateStartingRoom(5, 5, horizontalConn) << " "
          << validateStartingRoom(5, 4, horizontalConn) << "\n";
    return 0;
}