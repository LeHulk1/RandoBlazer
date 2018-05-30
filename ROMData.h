#ifndef __ROMDATA_H__
#define __ROMDATA_H__

#include "Item.h"
#include "Lair.h"
#include "Sprite.h"

#include <fstream>
#include <vector>


#define NUMBER_OF_CHESTS  66
#define NUMBER_OF_ITEMS   126
#define NUMBER_OF_LAIRS   400
#define NUMBER_OF_SPRITES 205

#define CHEST_DATA_ADDRESS        0xAADE
#define MONSTER_LAIR_DATA_ADDRESS 0xBA0D


using namespace std;

namespace ROMData {
    void GetOriginalItemData(vector<Item> &ItemList);
    void GetOriginalLairData(vector<Lair> &LairList, fstream &ROMFile);
    void GetOriginalMapSpriteData (vector<Sprite> &SpriteList, fstream &ROMFile);
}


#endif // __ROMDATA_H__
