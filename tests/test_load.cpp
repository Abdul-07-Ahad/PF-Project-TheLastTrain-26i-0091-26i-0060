#include <iostream>
#include "logger.h"
using namespace std;

int main() {
    int floor = 0, room = 0, energy = 0, time = 0, score = 0, trips = 0, invCount = 0;
    char inv[MAX_INVENTORY_CAPACITY] = {};
    bool obj[NUM_OBJECTIVES] = {};
    char rooms[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM] = {};
    int counts[NUM_FLOORS][ROOMS_PER_FLOOR] = {0};

    bool ok = loadGameStateFromFile("out/test-save.txt", floor, room, energy, time, score,
                                    inv, invCount, obj, trips, rooms, counts);
    cout << ok << " F" << floor << " R" << room << " E" << energy << " T" << time
         << " S" << score << " trips" << trips << " inv=" << invCount << " ";
    for (int i = 0; i < invCount; i++) cout << inv[i];
    cout << " obj=";
    for (int i = 0; i < NUM_OBJECTIVES; i++) cout << obj[i];
    cout << endl;

    ok = loadGameStateFromFile("out/does-not-exist.txt", floor, room, energy, time, score,
                               inv, invCount, obj, trips, rooms, counts);
    cout << ok << endl;
        counts[0][0] = 3;     // to prove the loader resets rooms, set junk first
    ok = loadGameStateFromFile("out/test-save.txt", floor, room, energy, time, score,
                               inv, invCount, obj, trips, rooms, counts);
    cout << "r12=" << counts[0][1] << rooms[0][1][0]
         << " r32=" << counts[2][1] << rooms[2][1][0] << rooms[2][1][1]
         << " r11=" << counts[0][0] << " r54=" << counts[4][3] << endl;
    return 0;
}