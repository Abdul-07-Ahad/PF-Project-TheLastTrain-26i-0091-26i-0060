#include "switches.h"
#include "physics.h"
#include <util.h>
#include <iostream>
using namespace std;
int main() {
    // char inv[MAX_INVENTORY_CAPACITY] = {};
    // int n = 0;
    // // std::cout << unlockFloor3Door(inv, n) << " ";
    // addItem(inv, n, ENT_MASTER_KEY);
    // // std::cout << unlockFloor3Door(inv, n) << "\n";

    // std::cout << canUseLift1(1,2) << canUseLift1(2,1) << canUseLift1(2,3) << canUseLift1(1,3) << "\n";

    // std::cout << canUseLift2(2,4,false,0) << canUseLift2(2,4,false,2) << canUseLift2(2,4,true,0)
    //           << canUseLift2(2,3,true,3) << canUseLift2(4,4,true,3) << canUseLift2(1,2,true,3) << "\n";
    // char inv[10] = {ENT_MASTER_KEY};
    // char none[10] = {};
    // std::cout << canTraverse(1,1,'L', inv,  1, 0, false)   // expect 1  (has key)
    //  << canTraverse(1,1,'L', none, 0, 0, false)   // expect 0  (no key)
    //  << canTraverse(1,1,'2', none, 0, 2, false)   // expect 1  (emergency trips)
    //  << canTraverse(1,1,'2', none, 0, 0, false)   // expect 0  (broken, no trips)
    //  << canTraverse(1,1,'#', none, 0, 0, true)    // expect 0  (wall)
    //  << std::endl;
    // char inv[MAX_INVENTORY_CAPACITY] = {ENT_FUSE, ENT_MASTER_KEY};
    // int count = 2;
    // bool obj[NUM_OBJECTIVES] = {};
    // int score = 0;

    // bool r1 = installFuseControlPanel(inv, count, obj, score);
    // cout << r1 << " " << count << " " << obj[OBJ_FUSE_INSTALLED] << " " << score << endl;
    // bool r2 = installFuseControlPanel(inv, count, obj, score);   // second try
    // cout << r2 << " " << count << " " << score << endl;
    // return 0;
    bool obj2[NUM_OBJECTIVES] = {};
cout << verifyCabinetCode("427", obj2);          // clues not found
obj2[OBJ_CLUE_4] = true;
obj2[OBJ_CLUE_2] = true;
cout << verifyCabinetCode("427", obj2);          // one clue missing
obj2[OBJ_CLUE_7] = true;
cout << verifyCabinetCode("427", obj2);          // all found, right code
cout << verifyCabinetCode("472", obj2);          // wrong order
cout << verifyCabinetCode("42", obj2);           // too short
cout << verifyCabinetCode("4270", obj2) << endl; 
printHeader("THE LAST TRAIN");
clearScreen();
printHeader("Floor 3");

}
