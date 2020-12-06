#ifndef __ROMDATA_H__
#define __ROMDATA_H__

#include "Item.h"
#include "Lair.h"
#include "Sprite.h"

#include <fstream>
#include <vector>


#define NUMBER_OF_CHESTS  66
#define NUMBER_OF_ITEMS   126
#define NUMBER_OF_LAIRS   420
#define NUMBER_OF_SPRITES 205

#define CHEST_DATA_ADDRESS        0xAADE
#define MONSTER_LAIR_DATA_ADDRESS 0xBA0D

namespace ROMData {
    void GetOriginalItemData(std::vector<Item> &ItemList);
    void GetOriginalLairData(std::vector<Lair> &LairList, std::fstream &ROMFile);
    void GetOriginalMapSpriteData (std::vector<Sprite> &SpriteList, std::fstream &ROMFile);

    bool NPCOriginallyGivesEXP(int NPCIndex);
}


#endif // __ROMDATA_H__
