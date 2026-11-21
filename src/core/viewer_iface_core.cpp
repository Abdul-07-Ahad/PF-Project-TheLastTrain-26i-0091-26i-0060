#include <iostream>
#include "viewer_iface.h"
#include "util.h"
#include "level.h"
using namespace std;
// Terminal UI and dashboard rendering using primitive parameters
const char* ROOM_NAMES[NUM_FLOORS][ROOMS_PER_FLOOR] = {
    {"Entrance Lobby", "Reception", "Waiting Area", "Utility Room"},     
    {"Lounge", "Archive", "Lab", "Meeting Room"},
    {"Stairwell Annex", "Security Vestibule", "Animal Room", "Control Room"},
    {"First Aid Station", "Medical Room", "Maintenance Corridor", "Office"},
    {"Security Room", "Train Boarding Platform", "Storage Room", "Observation Deck"}
};
const char* OBJ_NAMES[] = {
    "Master Key collected",        
    "Clue Digit 4 found",          
    "Clue Digit 2 found",          
    "Clue Digit 7 found",          
    "Cabinet unlocked",            
    "Train Key acquired",          
    "Fuse collected",              
    "Fuse installed", 
    "Fift 2 pass",            
    "Lift 2 active",               
    "Passenger rescued",           
    "Cat Carrier collected",       
    "Cat secured",                 
    "Train prepared"               
};
void renderDashboard(int playerFloor, int playerRoom, int energy, int time, int score,
                     const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                     int emergencyTripsLeft) {
    printHeader("THE LAST TRAIN");
    cout << "Location : F" << playerFloor << "-R" <<playerRoom << endl;
    cout << "Energy   : " << energy << "     Time : " << time << "     Score : " << score << endl;
    cout << "Emergency trips left : " << emergencyTripsLeft << endl;
    cout << "Inventory (" << inventoryCount << "/" << MAX_INVENTORY_CAPACITY<< "): ";
    if (inventoryCount == 0) {
        cout << "empty";
    } 
    else {
        for (int i = 0; i < inventoryCount; i++) {
            cout << getEntityName(inventory[i])<< " (" << inventory[i] << ")";       
            if (i < inventoryCount - 1) {
                cout << ", ";                 
            }
        }
    }
    cout << endl;
}
void renderRoomDetails(int floor, int room,
                       const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                       int entityCount) {
    cout << "--- F" << floor << "-R" << room << ": " << ROOM_NAMES[floor-1][room-1] << endl;
    cout << "You see: ";
    if (entityCount == 0) {
        cout << "nothing";
    } else {
        for (int i = 0; i < entityCount; i++) {
            cout << getEntityName(roomEntities[floor-1][room-1][i])<< " (" << roomEntities[floor - 1][room - 1][i] << ")";
            if (i < entityCount - 1) {
                cout << ", ";
            }
        }
    }
    cout << endl;
}
void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES]) {
    cout << "--Objectives--" << endl;
    for (int i = 0; i < NUM_OBJECTIVES; i++){
        if (objectives[i]){
            cout << "[done] ";
        } 
        else {
            cout << "[ ] ";
        }
        cout << objectives[i] << endl;
    }
}
void renderAsciiBuilding(int playerFloor, int playerRoom) {
    cout << "-- Building --" << endl;
    for (int f = 5; f >= 1; f--) {
        cout << "F" << f << " ";
        for (int r = 1; r <= 4; r++) {
            if (f == playerFloor && r == playerRoom) {
                cout << "[P] ";
            } 
            else {
                cout << "[ ] ";
            }
        }
        cout << endl;
    }
    cout << "    R1  R2  R3  R4" << endl;
}


