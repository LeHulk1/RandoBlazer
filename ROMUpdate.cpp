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

    void ROMUpdateTextAndItems(const std::vector<Lair>& RandomizedLairList,
                               const std::vector<Item>& RandomizedItemList,
                               std::fstream &ROMFile,
                               const std::string& Seed) {
        unsigned char Byte;

        /*************************/
        /* Update Chest contents */
        /*************************/

        ROMFile.seekp (CHEST_DATA_ADDRESS, std::ios::beg);
        bool DoubledChestDone = false;
        for (int i=0; i<NUMBER_OF_CHESTS; i++) {
            /* Put the cursor on the contents data for this chest */
            ROMFile.seekp (3, std::ios::cur);

            /* Update the contents */
            ItemID ItemID = RandomizedItemList[i].Contents;
            //ROMFile.write((char*)(&ItemID), 1);
            ROMFile.put((char)ItemID);
            ROMFile.put(ConvertToHex(RandomizedItemList[i].GemsExp % 100));
            ROMFile.put(ConvertToHex(RandomizedItemList[i].GemsExp / 100));

            /* Chest at index 22 is doubled, so we have to double its replacing one */
            if (i==22 && DoubledChestDone==false) {
                i--;
                DoubledChestDone = true;
            }

            /* Skip over FF bytes */
            /* Synchronize read position with write position */
            ROMFile.seekg (ROMFile.tellp(), std::ios::beg);
            do {
                ROMFile.read ((char*)(&Byte), 1);
            }
            while (Byte == 0xFF);
            ROMFile.seekp (-1, std::ios::cur);
        }


        /*************************************/
        /* Full update of text and NPC items */
        /*************************************/

        NPCTextUpdateMain(RandomizedLairList, RandomizedItemList, ROMFile, Seed);

    }


    void ROMUpdateLairs(const std::vector<Lair>& RandomizedLairList, std::fstream &ROMFile) {

        ROMFile.seekp (MONSTER_LAIR_DATA_ADDRESS, std::ios::beg);

        for (int i=0; i<NUMBER_OF_LAIRS; i++) {

            ROMFile.seekg(10, std::ios::cur);

            /* Update the contents of this Monster Lair */
            ROMFile.write((char*)(&RandomizedLairList[i].Act), 1);
            ROMFile.write((char*)(&(RandomizedLairList[i].PositionData[0])), POSITION_DATA_SIZE);
            ROMFile.seekp(2, std::ios::cur);
            unsigned char lairType[2] = {
                static_cast<unsigned char>((static_cast<unsigned int>(RandomizedLairList[i].Type) >> 8) & 0xFF),
                static_cast<unsigned char>(static_cast<unsigned int>(RandomizedLairList[i].Type) & 0xFF)
            };
            ROMFile.write((char*)&lairType, sizeof(lairType));
            ROMFile.seekp(1, std::ios::cur);
            ROMFile.write((char*)(&RandomizedLairList[i].NbEnemies), 1);
            ROMFile.write((char*)(&RandomizedLairList[i].SpawnRate), 1);
            ROMFile.write((char*)(&RandomizedLairList[i].Enemy), 1);
            ROMFile.seekp(1, std::ios::cur);
            ROMFile.write((char*)(&RandomizedLairList[i].Orientation), 1);
            ROMFile.seekp(8, std::ios::cur);
        }
    }


    void ROMUpdateMapSprites(const std::vector<Sprite>& RandomizedSpriteList, std::fstream &ROMFile) {
        int Address;
        for (int SpriteIndex = 0; SpriteIndex < NUMBER_OF_SPRITES; ++SpriteIndex) {
            /* Get the ROM address of this sprite data */
            Address = RandomizedSpriteList[SpriteIndex].Address;

            /* Update the contents of this Sprite */
            ROMFile.seekp (Address, std::ios::beg);
            ROMFile.write((char*)(&RandomizedSpriteList[SpriteIndex].x), 1);
            ROMFile.write((char*)(&RandomizedSpriteList[SpriteIndex].y), 1);
            ROMFile.write((char*)(&RandomizedSpriteList[SpriteIndex].Orientation), 1);
            ROMFile.write((char*)(&RandomizedSpriteList[SpriteIndex].Enemy), 1);
        }
    }
}
