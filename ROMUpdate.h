#ifndef __ROMUPDATE_H__
#define __ROMUPDATE_H__

#include "Item.h"
#include "Lair.h"
#include "Sprite.h"

#include <fstream>
#include <vector>

namespace ROMUpdate {
    bool NPCOriginallyGivesEXP(int NPCIndex);
    void ROMUpdateTextAndItems(const std::vector<Lair>& RandomizedLairList,
                               const std::vector<Item>& RandomizedItemList,
                               std::fstream &ROMFile,
                               long Seed);
    void ROMUpdateLairs(const std::vector<Lair>& RandomizedLairList, std::fstream &ROMFile);
    void ROMUpdateMapSprites(const std::vector<Sprite>& RandomizedSpriteList, std::fstream &ROMFile);
}

#endif // __ROMUPDATE_H__
