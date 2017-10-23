#ifndef __ROMDATA_H__
#define __ROMDATA_H__

#include "Chest.h"
#include "Lair.h"

#include <fstream>
#include <vector>


#define NUMBER_OF_CHESTS 66
#define NUMBER_OF_LAIRS  400

#define CHEST_DATA_ADDRESS        0xAADE
#define MONSTER_LAIR_DATA_ADDRESS 0xBA0D


using namespace std;

namespace ROMData {
    void GetOriginalChestData(vector<Chest> &ChestList);
    void GetOriginalLairData (vector<Lair>  &LairList, fstream &ROMFile);
}


#endif // __ROMDATA_H__
