#include <iostream>
#include "level.h"
#include "state.h"
#include "util.h"
#include "config.h"
#include <fstream>
#include <cstring>
#include <cstdlib>
using namespace std;
bool loadBuildingMap(const char* filename,
                    char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                    char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                    int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){
    ifstream file(filename);
    if (!file) {
        return false;
    }
    char lines[60][MAX_LINE_LEN];
    int count = 0;
    while ( count<60 && file.getline(lines[count], MAX_LINE_LEN)){
        count++;
    }
    for (int floor = 0; floor < 5; floor++){
        int sourceRow = 14 - (3 * floor); 
        for (int room = 0; room < 3; room++){
            int sourceCol = 20 + (12 * room); 
            char c = lines[sourceRow][sourceCol];
            if (c != CONN_DOOR && c != CONN_WINDOW && c != CONN_LOCKED_DOOR && c != CONN_SOLID_WALL) {                          
                horizontalConn[floor][room] = CONN_NONE;                          
            } 
            else {
                horizontalConn[floor][room] = c;
            }
        }
    }
    for (int floor = 1; floor <= 5; floor++){
        for( int room = 1; room<=4 ; room++ ){
            entityCounts[floor-1][room-1] = 0;
            int firstcol = 9+12*(room-1);
            for(int col = firstcol ; col<=(firstcol+10) ; col++) {
                int row = 3+3*(5-floor);
                char c = lines[row][col];
                if ( c!= ' ' ){           
                    roomEntities[floor-1][room-1][entityCounts[floor-1][room-1]] = c;    
                    entityCounts[floor-1][room-1]++;
                }
            }
        }
    }
    return true;
}
bool loadInitialState(const char* filename,
                      int& playerFloor, int& playerRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft){
    ifstream file(filename);
    if (!file) {
        return false ;
    }
    emergencyTripsLeft = 0;
    inventoryCount = 0;
    for(int i=0; i<MAX_INVENTORY_CAPACITY; i++){
        inventory[i] = ENT_NONE;
    }
    char line[MAX_LINE_LEN];
    while (file.getline(line, MAX_LINE_LEN)){
        trimWhitespace(line);
        const char* value = getValue(line, "INITIAL_ENERGY:");
        if (value != nullptr){
            energy = clampInt(atoi(value), 0,MAX_ENERGY);
        }
        value = getValue(line, "INITIAL_TIME:");
        if (value != nullptr){
            time = clampInt(atoi(value), 0,MAX_TIME);
        }
        value = getValue(line, "INITIAL_SCORE:");
        if (value != nullptr){
            score = atoi(value);
        }
        value = getValue(line, "PASSENGER_RESCUED:");
        if (value != nullptr){
            objectives[OBJ_PASSENGER_RESCUED] = stringEquals(value, "YES");
        }
        value = getValue(line, "CAT_RESCUED:");
        if (value != nullptr){
            objectives[OBJ_CAT_SECURED] = stringEquals(value, "YES");
        }
        value = getValue(line, "CABINET_UNLOCKED:");
        if (value != nullptr){
            objectives[OBJ_CABINET_UNLOCKED] = stringEquals(value, "YES");
        }
        value = getValue(line, "TRAIN_KEY_COLLECTED:");
        if (value != nullptr){
            objectives[OBJ_TRAIN_KEY] = stringEquals(value, "YES");
        }
        value = getValue(line, "FUSE_INSTALLED:");
        if (value != nullptr){
            objectives[OBJ_FUSE_INSTALLED] = stringEquals(value, "YES");
        }
        value = getValue(line, "LIFT_PASS_COLLECTED:");
        if (value != nullptr){
            objectives[OBJ_LIFT_PASS] = stringEquals(value, "YES");
        }
        value = getValue(line, "TRAIN_PREPARED:");
        if (value != nullptr){
            objectives[OBJ_TRAIN_PREPARED] = stringEquals(value, "YES");
        }
        value = getValue(line, "START_ROOM:");
        if (value != nullptr && strlen(value) == 5 && value[0] == 'F' && value[2] == '-' && value[3] == 'R') {
        playerFloor = value[1] - '0' ;
        playerRoom  = value[4] - '0' ;
        }
    }
    return true;
}
const char* getEntityName(char entitySymbol) {
    switch (entitySymbol){
        case ENT_MASTER_KEY: return "Master Key";
        case ENT_ENERGY_PACK: return "Energy Pack";
        case ENT_EMERGENCY_TOOLKIT: return "Emergency Toolkit";
        case ENT_CLUE_4: return "Clue Digit 4";
        case ENT_FUSE: return "Fuse";
        case ENT_CAT: return "Stranded Cat";
        case ENT_CONTROL_PANEL: return "Control Panel Terminal";
        case ENT_LIFT_PASS: return "Lift Pass";
        case ENT_PASSENGER: return "Stranded Passenger";
        case ENT_MEDICAL_KIT: return "Medical Kit";
        case ENT_EMERGENCY_CARD: return "Emergency Access Card";
        case ENT_CLUE_2: return "Clue Digit 2";
        case ENT_CLUE_7: return "Clue Digit 7";
        case ENT_CABINET_TRAIN_KEY: return "Cabinet / Train Key";
        case ENT_CAT_CARRIER: return "Cat Carrier";
        case ENT_OBSERVATION_TOKEN: return "Observation Token";
        default: return "Unknown item";
    }
}
bool validateStartingRoom(int startFloor, int startRoom , const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1]){
    (void)horizontalConn; 
    if (startFloor < 1 || startFloor > NUM_FLOORS || startRoom < 1 || startRoom > ROOMS_PER_FLOOR ){
        return false;
    }
    return true;
}