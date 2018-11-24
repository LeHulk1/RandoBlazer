#ifndef __ROMUPDATE_H__
#define __ROMUPDATE_H__

#include "Item.h"
#include "Lair.h"
#include "Sprite.h"

#include <fstream>
#include <vector>


using namespace std;

namespace ROMUpdate {
    bool NPCOriginallyGivesEXP(int NPCIndex);
    void ROMUpdateTextAndItems(vector<Lair> RandomizedLairList,
                               vector<Item> RandomizedItemList,
                               fstream &ROMFile,
                               long Seed);
    void ROMUpdateLairs(vector<Lair> RandomizedLairList, fstream &ROMFile);
    void ROMUpdateMapSprites(vector<Sprite> RandomizedSpriteList, fstream &ROMFile);
}

#endif // __ROMUPDATE_H__
