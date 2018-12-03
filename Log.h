#ifndef __LOG_H__
#define __LOG_H__

#include "Item.h"
#include "Lair.h"

#include <string>
#include <vector>


using namespace std;

namespace Log {
    int GetAreaNumber(Lair &Lair);
    string GetKeyNPCName(int NPC_ID);
    string GetAreaName(Lair &Lair);

    void CreateSpoilerLog(vector<Lair> &RandomizedLairList,
                          vector<Item> &RandomizedChestList);
}

#endif // __LOG_H__
