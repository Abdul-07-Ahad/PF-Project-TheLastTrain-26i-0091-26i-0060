#include "simulator.h"
#include "level.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"
#include <iostream>
#include <sstream>
using namespace std;
int main() {
    int playerFloor = 0;
    int playerRoom = 0;
    int energy = INITIAL_ENERGY;
    int time = INITIAL_TIME;
    int score = INITIAL_SCORE;
    char inventory[MAX_INVENTORY_CAPACITY];
    int inventoryCount = 0;
    int emergencyTripsLeft = 0;
    bool objectives[NUM_OBJECTIVES] = { false };
    char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1];
    char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM];
    int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR] = { 0 };
    const char* mapFile = "data/building.txt";
    const char* initStateFile = "data/initial-state.txt";
    if (!loadBuildingMap(mapFile, horizontalConn, roomEntities, entityCounts)) {
        cerr << "Error: Could not load " << mapFile << "\n";
        return 1;
    }
    int reachable = countReachableRooms(1, 1, horizontalConn);
    if (reachable < TOTAL_ROOMS) {
        cerr << "Warning: the map is not fully connected (" << reachable
             << " of " << TOTAL_ROOMS << " rooms reachable)." << endl;
    }
    if (!loadInitialState(initStateFile, playerFloor, playerRoom, energy, time, score,
                          inventory, inventoryCount, objectives, emergencyTripsLeft)) {
        cerr << "Error: Could not load " << initStateFile << "\n";
        return 1;
    }

    char answer[100];
    bool chosen = false;
    while (!chosen) {
        cout << "Type n for a new game, or r to resume a saved game (or quit): ";
        if (!cin.getline(answer, 100)) {
            if (cin.eof()) {
                return 1;
            }
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please type n, r or quit." << endl;
            continue;
        }
        if (stringEquals(answer, "quit")) {
            return 0;
        } 
        else if (stringEquals(answer, "n") || stringEquals(answer, "N")) {
            chosen = true;
        } 
        else if (stringEquals(answer, "r") || stringEquals(answer, "R")) {
            chosen = true;
            if (loadGameStateFromFile("out/savegame.txt", playerFloor, playerRoom, energy, time, score,
                     inventory, inventoryCount, objectives, emergencyTripsLeft,
                     roomEntities, entityCounts)) {
                cout << "Game loaded." << endl;
            } else {
                cout << "No saved game could be loaded. Starting a new game." << endl;
            }
        } 
        else {
            cout << "Invalid input. Please type n, r or quit." << endl;
        }
    }

    while (!validateStartingRoom(playerFloor, playerRoom, horizontalConn)) {
        cout << "Choose your starting room (floor room), e.g. 5 2 (or type quit): ";
        char line[100];
        if (!cin.getline(line, 100)) {
            if (cin.eof()) {
                return 1;
            }
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        if (stringEquals(line, "quit")) {
            return 0; 
        }
        istringstream input(line);
        if (!(input >> playerFloor >> playerRoom)) {
            playerFloor = 0;
            playerRoom = 0;
            cout << "Please type two numbers: floor, then room, e.g. 5 2" << endl;
        } 
        else if (!validateStartingRoom(playerFloor, playerRoom, horizontalConn)) {
            cout << "There is no such room. Floors are 1-5 and rooms are 1-4." << endl;
        }
    }
    cout << "===========================================\n";
    cout << " THE LAST TRAIN - SIMULATION INITIALIZED   \n";
    cout << "===========================================\n";
    runSimulation(playerFloor, playerRoom, energy, time, score,
                  inventory, inventoryCount, objectives, emergencyTripsLeft,
                  horizontalConn, roomEntities, entityCounts);
    return 0;
}