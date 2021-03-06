#include "Map.h"
#include "ROMUpdate.h"
#include "ROMData.h"
#include "TextUpdate.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>



namespace ROMUpdate {

    void ROMUpdateTextAndItems(vector<Lair> RandomizedLairList,
                               vector<Item> RandomizedItemList,
                               fstream &ROMFile,
                               long Seed) {

        unsigned char ItemID;
        unsigned char GemsExpValue[2];
        unsigned char Byte;
        int GemsExp_TensAndUnits;


        /*************************/
        /* Update Chest contents */
        /*************************/

        ROMFile.seekp (CHEST_DATA_ADDRESS, ios::beg);
        bool DoubledChestDone = false;
        for (int i=0; i<NUMBER_OF_CHESTS; i++) {

            /* Put the cursor on the contents data for this chest */
            ROMFile.seekp (3, ios::cur);

            /* Update the contents */
            ItemID = RandomizedItemList[i].Contents;
            ROMFile.write((char*)(&ItemID), 1);
            GemsExp_TensAndUnits = RandomizedItemList[i].GemsExp % 100;
            GemsExpValue[0] = ConvertToHex(GemsExp_TensAndUnits);
            GemsExpValue[1] = ConvertToHex((RandomizedItemList[i].GemsExp - GemsExp_TensAndUnits) / 100);
            ROMFile.write((char*)(&GemsExpValue[0]), 2);

            /* Chest at index 22 is doubled, so we have to double its replacing one */
            if (i==22 && DoubledChestDone==false) {
                i--;
                DoubledChestDone = true;
            }

            /* Skip over FF bytes */
            ROMFile.seekg (0, ios::cur);
            do {
                ROMFile.read ((char*)(&Byte), 1);
            }
            while (Byte == 0xFF);
            ROMFile.seekp (-1, ios::cur);
        }


        /*************************************/
        /* Full update of text and NPC items */
        /*************************************/

        NPCTextUpdateMain(RandomizedLairList, RandomizedItemList, ROMFile, Seed);

    }


    void ROMUpdateLairs(vector<Lair> RandomizedLairList, fstream &ROMFile) {

        ROMFile.seekp (MONSTER_LAIR_DATA_ADDRESS, ios::beg);

        for (int i=0; i<NUMBER_OF_LAIRS; i++) {

            ROMFile.seekg(10, ios::cur);

            /* Update the contents of this Monster Lair */
            ROMFile.write((char*)(&RandomizedLairList[i].Act), 1);
            ROMFile.write((char*)(&(RandomizedLairList[i].PositionData[0])), POSITION_DATA_SIZE);
            ROMFile.seekp(2, ios::cur);
            ROMFile.write((char*)(&(RandomizedLairList[i].Type[0])), LAIR_TYPE_SIZE);
            ROMFile.seekp(1, ios::cur);
            ROMFile.write((char*)(&RandomizedLairList[i].NbEnemies), 1);
            ROMFile.write((char*)(&RandomizedLairList[i].SpawnRate), 1);
            ROMFile.write((char*)(&RandomizedLairList[i].Enemy), 1);
            ROMFile.seekp(1, ios::cur);
            ROMFile.write((char*)(&RandomizedLairList[i].Orientation), 1);
            ROMFile.seekp(8, ios::cur);
        }
    }


    void ROMUpdateMapSprites(vector<Sprite> RandomizedSpriteList, fstream &ROMFile) {
        int Address;
        for (int SpriteIndex = 0; SpriteIndex < NUMBER_OF_SPRITES; ++SpriteIndex) {
            /* Get the ROM address of this sprite data */
            Address = RandomizedSpriteList[SpriteIndex].Address;

            /* Update the contents of this Sprite */
            ROMFile.seekp (Address, ios::beg);
            ROMFile.write((char*)(&RandomizedSpriteList[SpriteIndex].x), 1);
            ROMFile.write((char*)(&RandomizedSpriteList[SpriteIndex].y), 1);
            ROMFile.write((char*)(&RandomizedSpriteList[SpriteIndex].Orientation), 1);
            ROMFile.write((char*)(&RandomizedSpriteList[SpriteIndex].Enemy), 1);
        }
    }
}
