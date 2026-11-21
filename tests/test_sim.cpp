#include <iostream>
#include "simulator.h"
#include "level.h"
using namespace std;

int main() {
    char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1];
    char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM];
    int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR] = {0};
    loadBuildingMap("data/building.txt", horizontalConn, roomEntities, entityCounts);

    char inv[MAX_INVENTORY_CAPACITY] = {};
    int floor = 1, room = 1, energy = 100, time = 50, trips = 0;

    bool r;
    r = handleMoveCommand(1, 2, floor, room, energy, time, horizontalConn, inv, 0, trips, false);
    cout << r << " E" << energy << " T" << time << " F" << floor << "R" << room << " trips" << trips << endl;
    r = handleMoveCommand(1, 3, floor, room, energy, time, horizontalConn, inv, 0, trips, false);
    cout << r << " E" << energy << " T" << time << " F" << floor << "R" << room << " trips" << trips << endl;
    r = handleMoveCommand(2, 3, floor, room, energy, time, horizontalConn, inv, 0, trips, false);
    cout << r << " E" << energy << " T" << time << " F" << floor << "R" << room << " trips" << trips << endl;
    r = handleMoveCommand(2, 4, floor, room, energy, time, horizontalConn, inv, 0, trips, false);
    cout << r << " E" << energy << " T" << time << " F" << floor << "R" << room << " trips" << trips << endl;
    r = handleMoveCommand(4, 4, floor, room, energy, time, horizontalConn, inv, 0, trips, false);
    cout << r << " E" << energy << " T" << time << " F" << floor << "R" << room << " trips" << trips << endl;
    trips = 2;
    r = handleMoveCommand(4, 4, floor, room, energy, time, horizontalConn, inv, 0, trips, false);
    cout << r << " E" << energy << " T" << time << " F" << floor << "R" << room << " trips" << trips << endl;
    r = handleMoveCommand(4, 3, floor, room, energy, time, horizontalConn, inv, 0, trips, false);
    cout << r << " E" << energy << " T" << time << " F" << floor << "R" << room << " trips" << trips << endl;
    r = handleMoveCommand(4, 2, floor, room, energy, time, horizontalConn, inv, 0, trips, false);
    cout << r << " E" << energy << " T" << time << " F" << floor << "R" << room << " trips" << trips << endl;
    return 0;
}