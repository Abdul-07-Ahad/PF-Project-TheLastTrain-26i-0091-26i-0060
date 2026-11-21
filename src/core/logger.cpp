#include "logger.h"
#include "util.h"
#include "state.h"
#include <fstream>
#include <iostream>
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
    } else {
        file << "Result: DEFEAT" << endl;
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
        file << getObjectiveName(i) << endl;                  // the function that gives the objective's name
    }
}