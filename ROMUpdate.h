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
    int ConvertToHex(int Dec);
    void ROMUpdateItems(vector<Item> RandomizedItemList, fstream &ROMFile);
    void ROMUpdateLairs(vector<Lair> RandomizedLairList, fstream &ROMFile);
    void ROMUpdateMapSprites(vector<Sprite> RandomizedSpriteList, fstream &ROMFile);
}

#endif // __ROMUPDATE_H__
