#ifndef __LOG_H__
#define __LOG_H__

#include "Item.h"
#include "Lair.h"

#include <string>
#include <vector>

namespace Log {
    int GetAreaNumber(const Lair &Lair);
    std::string GetKeyNPCName(int NPC_ID);
    std::string GetAreaName(const Lair &Lair);

    void CreateSpoilerLog(const std::vector<Lair> &RandomizedLairList,
                          const std::vector<Item> &RandomizedChestList);
}

#endif // __LOG_H__
