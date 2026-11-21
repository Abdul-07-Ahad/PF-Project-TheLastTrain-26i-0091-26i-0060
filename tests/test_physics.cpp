#include "physics.h"
#include <iostream>

int main() {
    int e = 100, t = 50, s = 0;
    applyResourceDelta(e, t, COST_MOVE_ORDINARY_ENERGY, COST_MOVE_ORDINARY_TIME);
    std::cout << "after a move:        " << e << " " << t << "\n";
    e = 90; t = 40;
    applyResourceDelta(e, t, BONUS_ENERGY_PACK, 0);
    std::cout << "pack at 90 energy:   " << e << " " << t << "\n";
    e = 1; t = 1;
    applyResourceDelta(e, t, COST_MOVE_LIFT_ENERGY, COST_MOVE_LIFT_TIME);
    std::cout << "lift at 1/1:         " << e << " " << t << "\n";
    applyScoreDelta(s, SCORE_KEY_COLLECT);
    applyScoreDelta(s, SCORE_CLUE_FOUND);
    std::cout << "score after key+clue: " << s << "\n";
    char inv[MAX_INVENTORY_CAPACITY];
    inv[0] = ENT_MASTER_KEY;
    inv[1] = ENT_FUSE;
    inv[2] = ENT_CAT_CARRIER;      // sits beyond the count on purpose
    int count = 2;
    std::cout << hasItem(inv, count, ENT_FUSE) << " "
          << hasItem(inv, count, ENT_MEDICAL_KIT) << " "
          << hasItem(inv, count, ENT_CAT_CARRIER) << "\n";
    char inv2[MAX_INVENTORY_CAPACITY];
    int count2 = 0;
    bool ok1 = addItem(inv2, count2, ENT_FUSE);
    std::cout << ok1 << " count=" << count2 << " has fuse=" << hasItem(inv2, count2, ENT_FUSE) << "\n";
    for (int i = 0; i < MAX_INVENTORY_CAPACITY; i++) addItem(inv2, count2, ENT_MASTER_KEY);
    std::cout << "full: count=" << count2 << " next add=" << addItem(inv2, count2, ENT_CAT_CARRIER)
          << " count=" << count2 << "\n";
    char inv3[MAX_INVENTORY_CAPACITY];
int count3 = 0;
addItem(inv3, count3, ENT_MASTER_KEY);
addItem(inv3, count3, ENT_FUSE);
addItem(inv3, count3, ENT_CAT_CARRIER);
bool r1 = removeItem(inv3, count3, ENT_FUSE);
std::cout << r1 << " count=" << count3 << " items: " << inv3[0] << inv3[1]
          << " fuse? " << hasItem(inv3, count3, ENT_FUSE) << "\n";
bool r2 = removeItem(inv3, count3, ENT_FUSE);
std::cout << r2 << " count=" << count3 << "\n";
    return 0;
}