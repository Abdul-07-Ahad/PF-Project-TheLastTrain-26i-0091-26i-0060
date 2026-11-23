#include "logger.h"
#include "util.h"
#include "state.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;
// File persistence for savegame.txt and game_summary.txt using primitive variables
void writeGameSummaryReport(const char* filename,
                            int score, int remainingEnergy, int remainingTime,
                            const bool objectives[NUM_OBJECTIVES], bool victory) {
    ofstream file(filename);
    if (!file) {
        cout << "Error: could not write " << filename << endl;
        return;
    }
    file << "=== THE LAST TRAIN - GAME SUMMARY ===" << endl;
    if (victory) {
        file << "Result: VICTORY" << endl;
    } 
    else if (remainingEnergy <= 0 || remainingTime <= 0) {
        file << "Result: DEFEAT" << endl;
    } 
    else {
        file << "Result: INCOMPLETE (player quit)" << endl;
    }
    file << "Final score: " << score << endl;
    file << "Remaining Energy: " << remainingEnergy << endl;
    file << "Remaining Time: " << remainingTime << endl;

    file << endl << "--- Objectives ---" << endl;
    for (int i = 0; i < OBJ_MISSION_WON; i++) {   
        if (objectives[i]) {
            file << "[x] ";
        } else {
            file << "[ ] ";
        }
        file << getObjectiveName(i) << endl;
    }
}
bool saveGameStateToFile(const char* filename,
                         int playerFloor, int playerRoom,
                         int energy, int time, int score,
                         const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                         const bool objectives[NUM_OBJECTIVES], int emergencyTripsLeft,
                         const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                         const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]) {
    ofstream file(filename);
    if (!file) {
        return false;
    }

    file << "PLAYER_FLOOR: " << playerFloor << endl;
    file << "PLAYER_ROOM: " << playerRoom << endl;
    file << "ENERGY: " << energy << endl;
    file << "TIME: " << time<< endl;
    file << "SCORE: " << score << endl;
    file << "TRIPS: " << emergencyTripsLeft << endl;
    file << "INVENTORY_COUNT: " << inventoryCount << endl;

    file << "INVENTORY: ";
    for (int i = 0; i < inventoryCount; i++){
        file << inventory[i];
    }
    file << endl;

    file << "OBJECTIVES: ";
    for (int i = 0; i < NUM_OBJECTIVES; i++){
        if (objectives[i]){
            file << '1';
        } 
        else {
            file << '0';
        }
    }
    file << endl;
    for (int f = 1; f <= 5; f++){
        for (int r = 1; r <= 4; r++){
            file << "ROOM " << f << " " << r << ": ";
            for (int i = 0; i < entityCounts[f - 1][r - 1]; i++) {
                file << roomEntities[f-1][r-1][i]<< " "; 
            }
            file << endl;
        }
    }
    return true;
}
bool loadGameStateFromFile(const char* filename,
                           int& playerFloor, int& playerRoom,
                           int& energy, int& time, int& score,
                           char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                           bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                           char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                           int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]) {
    ifstream file(filename);
    if (!file) {
        return false;
    }
    inventoryCount = 0;
    char line[MAX_LINE_LEN];
    while (file.getline(line, MAX_LINE_LEN)){
        trimWhitespace(line);
        const char* value = getValue(line, "PLAYER_FLOOR:");
        if (value != nullptr) {
            playerFloor = atoi(value);
        }
        value = getValue(line, "PLAYER_ROOM:");
        if (value != nullptr) {
            playerRoom = atoi(value);
        }
        value = getValue(line, "ENERGY:");
        if (value != nullptr) {
            energy = atoi(value);
        }
        value = getValue(line, "TIME:");
        if (value != nullptr) {
            time = atoi(value);
        }
        value = getValue(line, "SCORE:");
        if (value != nullptr) {
            score = atoi(value);
        }
        value = getValue(line, "TRIPS:");
        if (value != nullptr) {
            emergencyTripsLeft = atoi(value);
        }
        value = getValue(line, "INVENTORY:");
        if (value != nullptr) {
            inventoryCount = 0;
            for (int j = 0; value[j] != '\0' && inventoryCount < MAX_INVENTORY_CAPACITY; j++){
                inventory[j] = value[j];
                inventoryCount++;
            }
        }
        value = getValue(line, "OBJECTIVES:");
        if (value != nullptr) {
            for (int i = 0; i < NUM_OBJECTIVES && value[i] != '\0'; i++){
                objectives[i] = (value[i] == '1');
            }
        }
        if (strncmp(line, "ROOM ", 5) == 0 && strlen(line) >= 9){
            int f = line[5] - '0';
            int r = line[7] - '0';
            if (f >= 1 && f <= NUM_FLOORS && r >= 1 && r <= ROOMS_PER_FLOOR){
                entityCounts[f - 1][r - 1] = 0;
                for (int k = 0; k < MAX_ENTITIES_PER_ROOM; k++){
                    roomEntities[f - 1][r - 1][k] = ENT_NONE;
                }
                const char* p = line + 9;
                while (*p != '\0') {
                    if (*p != ' ' && entityCounts[f - 1][r - 1] < MAX_ENTITIES_PER_ROOM){
                        roomEntities[f - 1][r - 1][entityCounts[f - 1][r - 1]] = *p;
                        entityCounts[f - 1][r - 1]++;
                    }
                    p++;
                }
            }
        }
    }
    return true;
}
