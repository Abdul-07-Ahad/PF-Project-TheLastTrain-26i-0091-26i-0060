#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "level.h"
#include "util.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Modular simulation loop using primitive variables
bool handleMoveCommand(int targetFloor, int targetRoom,
                       int& currentFloor, int& currentRoom,
                       int& energy, int& time,
                       const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                       const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                       int& emergencyTripsLeft, bool lift2Operational) {
    char connType;
    if (!isDirectlyConnected(currentFloor, currentRoom, targetFloor, targetRoom, horizontalConn, connType)) {
        cout << "You can't go there from here." << endl;
        return false;
    }
    if (!canTraverse(targetFloor, targetRoom, connType, inventory, inventoryCount,
              emergencyTripsLeft, lift2Operational)) {
        cout << "That way is blocked right now." << endl;
        return false;
    }

    int costEnergy = COST_MOVE_ORDINARY_ENERGY;      // ordinary move by default
    int costTime = COST_MOVE_ORDINARY_TIME;
    if (connType == CONN_WINDOW) {
        costEnergy = COST_MOVE_WINDOW_ENERGY;
        costTime = COST_MOVE_WINDOW_TIME;
    } 
    else if (connType == CONN_LIFT1 || connType == CONN_LIFT2) {
        costEnergy = COST_MOVE_LIFT_ENERGY;
        costTime = COST_MOVE_LIFT_TIME;
    }

    if (connType == CONN_LIFT2 && !lift2Operational) {
        emergencyTripsLeft--;     // use up one emergency trip
    }

    applyResourceDelta(energy, time, costEnergy, costTime);
    currentFloor = targetFloor;
    currentRoom = targetRoom;
    return true;
}
bool handleInspectCommand(int currentFloor, int currentRoom,
                          int& energy, int& time,
                          const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int entityCount,
                          bool objectives[NUM_OBJECTIVES], int& score) {
    if (currentFloor < 1 || currentFloor > 5 || currentRoom < 1 || currentRoom > 4) {
        return false;
    }

    applyResourceDelta(energy, time, COST_INSPECT_ENERGY, COST_INSPECT_TIME);
    renderRoomDetails(currentFloor, currentRoom, roomEntities, entityCount);

    for (int i = 0; i < entityCount; i++) {
        char item = roomEntities[currentFloor-1][currentRoom-1][i];     // floor-1, room-1
        if (item == ENT_CLUE_4 && !objectives[OBJ_CLUE_4]) {
            objectives[OBJ_CLUE_4] = true;
            applyScoreDelta(score, SCORE_CLUE_FOUND);
            cout << "You found clue digit 4!" << endl;
        }
        if (item == ENT_CLUE_2 && !objectives[OBJ_CLUE_2]) {
            objectives[OBJ_CLUE_2] = true;
            applyScoreDelta(score, SCORE_CLUE_FOUND);
            cout << "You found clue digit 2!" << endl;
        }
        if (item == ENT_CLUE_7 && !objectives[OBJ_CLUE_7]) {
            objectives[OBJ_CLUE_7] = true;
            applyScoreDelta(score, SCORE_CLUE_FOUND);
            cout << "You found clue digit 7!" << endl;
        }
    }
    return true;
}
int findEntityInRoom(const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                     int floor, int room, int entityCount, char symbol) {
    for (int i = 0; i < entityCount; i++) {
        if (roomEntities[floor-1][room-1][i] == symbol) {
            return i;
        }
    }
    return -1;
}

void removeEntityFromRoom(char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int floor, int room, int& entityCount, int index) {
    if (index < 0 || index >= entityCount) {
        return;
    }
    for (int i = index; i < entityCount - 1; i++) {
        roomEntities[floor - 1][room - 1][i] = roomEntities[floor-1][room-1][i + 1];
    }
    roomEntities[floor - 1][room - 1][entityCount - 1] = ENT_NONE;   // clear the vacated slot
    entityCount--;
}
bool handleCollectCommand(char itemSymbol, int currentFloor, int currentRoom,
                          int& energy, int& time, int& score,
                          char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int& entityCount,
                          char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                          bool objectives[NUM_OBJECTIVES]) {
    int index = findEntityInRoom(roomEntities, currentFloor, currentRoom, entityCount, itemSymbol);
    if (index == -1) {
        cout << "That item is not here." << endl;
        return false;
    }

    if (itemSymbol == ENT_CLUE_4 || itemSymbol == ENT_CLUE_2 || itemSymbol == ENT_CLUE_7 ||
        itemSymbol == ENT_CONTROL_PANEL || itemSymbol == ENT_CAT ||
        itemSymbol == ENT_PASSENGER || itemSymbol == ENT_CABINET_TRAIN_KEY) {
        cout << "You can't pick that up." << endl;
        return false;
    }

    if (itemSymbol == ENT_ENERGY_PACK || itemSymbol == ENT_EMERGENCY_TOOLKIT ||
        itemSymbol == ENT_OBSERVATION_TOKEN) {
        applyResourceDelta(energy, time, COST_ITEM_PICKUP_ENERGY, COST_ITEM_PICKUP_TIME);       // pickup cost
        if (itemSymbol == ENT_ENERGY_PACK) {
            applyResourceDelta(energy, time, BONUS_ENERGY_PACK, 0);
        } 
        else if (itemSymbol == ENT_EMERGENCY_TOOLKIT) {
            applyResourceDelta(energy, time, BONUS_EMERGENCY_TOOLKIT, 0);
        } 
        else {
            applyResourceDelta(energy, time, 0, BONUS_OBSERVATION_TOKEN);      // the token gives time
        }
        removeEntityFromRoom(roomEntities, currentFloor, currentRoom, entityCount, index);
        cout << "You used up " << getEntityName(itemSymbol) << "." << endl;
        return true;
    }

    if (!addItem(inventory, inventoryCount, itemSymbol)) {     // try to add it to the bag
        cout << "Your inventory is full." << endl;
        return false;
    }

    applyResourceDelta(energy, time, COST_ITEM_PICKUP_ENERGY, COST_ITEM_PICKUP_TIME);            // pickup cost
    removeEntityFromRoom(roomEntities, currentFloor, currentRoom, entityCount, index);

    if (itemSymbol == ENT_MASTER_KEY) {
        objectives[OBJ_MASTER_KEY] = true;
        applyScoreDelta(score, SCORE_KEY_COLLECT);
    } 
    else if (itemSymbol == ENT_FUSE) {
        objectives[OBJ_FUSE_COLLECTED] = true;
    } 
    else if (itemSymbol == ENT_LIFT_PASS) {
        objectives[OBJ_LIFT_PASS] = true;
        applyScoreDelta(score, SCORE_LIFT_PASS);
    } 
    else if (itemSymbol == ENT_CAT_CARRIER) {
        objectives[OBJ_CAT_CARRIER] = true;
    }

    cout << "You picked up " << getEntityName(itemSymbol) << "." << endl;
    return true;
}
bool handleUseCommand(char itemSymbol, int currentFloor, int currentRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES]) {
    if (!hasItem(inventory, inventoryCount, itemSymbol)) {
        cout << "You don't have that." << endl;
        return false;
    }

    if (itemSymbol == ENT_FUSE) {
        if (currentFloor != 3 || currentRoom != 4) {
            cout << "There is nothing to use the fuse on here." << endl;
            return false;
        }
        if (!installFuseControlPanel(inventory, inventoryCount, objectives, score)) {   // install the fuse
            return false;
        }
    } 
    else if (itemSymbol == ENT_MEDICAL_KIT) {
        if (currentFloor != 4 || currentRoom != 1 || objectives[OBJ_PASSENGER_RESCUED]) {
            cout << "You can't use the medical kit here." << endl;
            return false;
        }
        removeItem(inventory, inventoryCount, ENT_MEDICAL_KIT);
        objectives[OBJ_PASSENGER_RESCUED] = true;
        applyScoreDelta(score, SCORE_RESCUE_PASSENGER );
    } 
    else if (itemSymbol == ENT_CAT_CARRIER) {
        if (currentFloor != 3 || currentRoom != 3 || objectives[OBJ_CAT_SECURED]) {
            cout << "You can't secure cat here." << endl;
            return false;
        }
        removeItem(inventory, inventoryCount, ENT_CAT_CARRIER);
        objectives[OBJ_CAT_SECURED] = true;
        applyScoreDelta(score, SCORE_SECURE_CAT );
    } 
    else {
        cout << "You can't use that here." << endl;
        return false;
    }
    applyResourceDelta(energy, time,COST_ITEM_USE_ENERGY, COST_ITEM_USE_TIME);    // use cost, charged once
        return true;
}
bool handleUnlockCabinetCommand(const char* code, int currentFloor, int currentRoom,
                                bool objectives[NUM_OBJECTIVES], int& score,
                                char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                                int& entityCount) {
    if (currentFloor != 5|| currentRoom != 1) {
        cout << "There is no cabinet here." << endl;
        return false;
    }
    if (objectives[OBJ_CABINET_UNLOCKED]) {
        cout << "The cabinet is already open." << endl;
        return false;
    }
    if (!verifyCabinetCode(code, objectives)) {          // your switches function
        cout << "Wrong code, or you haven't found all the clues." << endl;
        return false;
    }
    objectives[OBJ_CABINET_UNLOCKED] = true;
    applyScoreDelta(score, SCORE_CABINET_UNLOCK);
    objectives[OBJ_TRAIN_KEY] = true;
    applyScoreDelta(score, SCORE_TRAIN_KEY);

    int index = findEntityInRoom(roomEntities, currentFloor, currentRoom, entityCount, ENT_CABINET_TRAIN_KEY);
    if (index != -1) {
        removeEntityFromRoom(roomEntities, currentFloor, currentRoom, entityCount, index);
    }
    cout << "The cabinet opens. You take the Train Key!" << endl;
    return true;
}
bool checkEndConditions(int energy, int time, const bool objectives[NUM_OBJECTIVES]) {
    if (objectives[OBJ_MISSION_FAILED]) {
        return true;
    }
    if (objectives[OBJ_MISSION_WON]) {
        return true;
    }
    if (energy <= 0 || time <= 0) {
        return true;
    }
    return false;
}
void printHelp() {
    cout << "--- Commands ---" << endl;
    cout << "move <floor> <room>   go to a room, e.g.  move 1 2" << endl;
    cout << "inspect               search the room you are in" << endl;
    cout << "collect <item>        pick up an item, e.g.  collect <" << endl;
    cout << "use <item>            use an item you carry, e.g.  use *" << endl;
    cout << "unlock <code>         enter a cabinet code, e.g.  unlock 123" << endl;
    cout << "objectives            show your checklist" << endl;
    cout << "help                  show this list" << endl;
    cout << "quit                  leave the game" << endl;
    cout << "board                 board the train (at F5-R2, when ready)" << endl;
}
bool handleBoardTrainCommand(int currentFloor, int currentRoom,
                             int& energy, int& time, int& score,
                             bool objectives[NUM_OBJECTIVES]) {
    if (currentFloor != 5 || currentRoom != 2) {
        cout << "The train is on the boarding platform (F5-R2)." << endl;
        return false;
    }
    if (objectives[OBJ_MISSION_WON]) {
        cout << "You have already boarded." << endl;
        return false;
    }

    bool ready = true;
    if (!objectives[OBJ_MASTER_KEY]) {
        cout << "You still need the Train Key." << endl;
        ready = false;
    }
    if (!objectives[OBJ_PASSENGER_RESCUED]) {
        cout << "The injured passenger has not been rescued." << endl;
        ready = false;
    }
    if (!objectives[OBJ_CAT_SECURED]) {
        cout << "The cat has not been secured." << endl;
        ready = false;
    }
    if (!ready) {
        return false;
    }

    applyResourceDelta(energy, time, COST_ITEM_USE_ENERGY, COST_ITEM_USE_TIME);
    objectives[OBJ_TRAIN_PREPARED] = true;
    applyScoreDelta(score, SCORE_PREPARE_TRAIN);
    objectives[OBJ_MISSION_WON] = true;
    applyScoreDelta(score, SCORE_MISSION_VICTORY);
    cout << "The train pulls away with everyone aboard. You made it!" << endl;
    return true;
}
void runSimulation(int& playerFloor, int& playerRoom,
                   int& energy, int& time, int& score,
                   char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                   bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                   char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                   char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                   int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]) {
    char command[32];
    bool running = true;
    printHelp();

    while (running) {
        bool lift2Operational = objectives[OBJ_FUSE_INSTALLED] && objectives[OBJ_LIFT_PASS];
        renderDashboard(playerFloor, playerRoom, energy, time, score,
                        inventory, inventoryCount, emergencyTripsLeft);
        renderAsciiBuilding(playerFloor, playerRoom);
        renderRoomDetails(playerFloor, playerRoom, roomEntities,
                          entityCounts[playerFloor - 1][playerRoom - 1]);
        cout << "(type 'help' for commands)" << endl;
        cout << "enter command to play : ";

        if (!(cin >> setw(32) >> command)) {
            break;                             
        }

        if (stringEquals(command, "quit")) {
            running = false;
        }
        else if (stringEquals(command, "help")) {
            printHelp();
        }
        else if (stringEquals(command, "inspect")) {
            handleInspectCommand(playerFloor, playerRoom, energy, time, roomEntities,
                                 entityCounts[playerFloor - 1][playerRoom - 1],
                                 objectives, score);
        }
        else if (stringEquals(command, "move")) {
            int targetFloor, targetRoom;
            if (!(cin >> targetFloor >> targetRoom)) {
                if (cin.eof()) {
                    break;
                }
                cin.clear();                         
                cout << "Invalid input. Usage: move <floor> <room>, e.g. move 1 2" << endl;
            } else {
                handleMoveCommand(targetFloor, targetRoom, playerFloor, playerRoom,
                                  energy, time, horizontalConn, inventory, inventoryCount,
                                  emergencyTripsLeft, lift2Operational);
            }
        }
        else if (stringEquals(command, "collect")) {
            char item;
            if (!(cin >> item)) {
                break;                           
            }
            bool picked = handleCollectCommand(item, playerFloor, playerRoom, energy, time, score,
                                               roomEntities,
                                               entityCounts[playerFloor - 1][playerRoom - 1],
                                               inventory, inventoryCount, objectives);
            if (picked && item == ENT_EMERGENCY_CARD) {
                emergencyTripsLeft = EMERGENCY_TRIPS_GRANTED;
            }
        }
        else if (stringEquals(command, "use")) {
            char item;
            if (!(cin >> item)) {
                break;
            }
            handleUseCommand(item, playerFloor, playerRoom, energy, time, score,
                             inventory, inventoryCount, objectives);
        }
        else if (stringEquals(command, "unlock")) {
            char code[16];
            if (!(cin >> setw(16) >> code)) {
                break;
            }
            bool opened = handleUnlockCabinetCommand(code, playerFloor, playerRoom, objectives, score,
                                                     roomEntities,
                                                     entityCounts[playerFloor - 1][playerRoom - 1]);
            if (opened) {
                applyResourceDelta(energy, time, COST_ITEM_USE_ENERGY, COST_ITEM_USE_TIME);
            }
        }
        else if (stringEquals(command, "objectives")) {
            renderObjectivesChecklist(objectives);
        }
        else if (stringEquals(command, "board")) {
            handleBoardTrainCommand(playerFloor, playerRoom, energy, time, score, objectives);
        }
        else {
            cout << "Invalid input. Type 'help' to see the commands." << endl;
        }

        cin.ignore(1000, '\n');            

        if (checkEndConditions(energy, time, objectives)) {
            running = false;
        }
    }

    if (objectives[OBJ_MISSION_WON]) {
        cout << "=== MISSION COMPLETE ===" << endl;
    } else if (energy <= 0) {
        objectives[OBJ_MISSION_FAILED] = true;
        cout << "=== MISSION FAILED: you ran out of Energy ===" << endl;
    } else if (time <= 0) {
        objectives[OBJ_MISSION_FAILED] = true;
        cout << "=== MISSION FAILED: you ran out of Time ===" << endl;
    } else {
        cout << "Game ended." << endl;
    }
    cout << "Final score: " << score << endl;
}