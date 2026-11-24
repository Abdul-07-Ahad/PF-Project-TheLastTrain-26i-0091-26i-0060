#include <iostream>
#include "level.h"
using namespace std;

int main() {
    char conn[NUM_FLOORS][ROOMS_PER_FLOOR - 1];
    char rooms[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM];
    int counts[NUM_FLOORS][ROOMS_PER_FLOOR] = {0};
    loadBuildingMap("data/building.txt", conn, rooms, counts);

    for (int f = 1; f <= NUM_FLOORS; f++) {
        for (int r = 1; r <= ROOMS_PER_FLOOR; r++) {
            cout << countReachableRooms(f, r, conn) << " ";
        }
        cout << endl;
    }

    char empty[NUM_FLOORS][ROOMS_PER_FLOOR - 1];
    for (int i = 0; i < NUM_FLOORS; i++) {
        for (int j = 0; j < ROOMS_PER_FLOOR - 1; j++) {
            empty[i][j] = CONN_NONE;
        }
    }
    cout << countReachableRooms(1, 1, empty) << " " << countReachableRooms(1, 2, empty) << " "
         << countReachableRooms(5, 4, empty) << " " << countReachableRooms(0, 1, conn) << endl;
    return 0;
}