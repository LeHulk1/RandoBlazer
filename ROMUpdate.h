#ifndef __ROMUPDATE_H__
#define __ROMUPDATE_H__

#include "ItemHolder.h"
#include "Lair.h"

#include <fstream>
#include <vector>


using namespace std;

namespace ROMUpdate {
    int ConvertToHex(int Dec);
    void ROMUpdateItems(vector<ItemHolder> RandomizedItemList, fstream &ROMFile);
    void ROMUpdateLairs(vector<Lair>  RandomizedLairList,  fstream &ROMFile);
}

#endif // __ROMUPDATE_H__
