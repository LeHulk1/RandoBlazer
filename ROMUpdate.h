#ifndef __ROMUPDATE_H__
#define __ROMUPDATE_H__

#include "Chest.h"
#include "Lair.h"

#include <fstream>
#include <vector>


using namespace std;

namespace ROMUpdate {
    int ConvertToHex(int Dec);
    void ROMUpdateChests(vector<Chest> RandomizedChestList, fstream &ROMFile);
    void ROMUpdateLairs (vector<Lair>  RandomizedLairList,  fstream &ROMFile);
}

#endif // __ROMUPDATE_H__
