#include "ROMUpdate.h"
#include "ROMData.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>



namespace ROMUpdate {

    int ConvertToHex(int Dec) {
        /* Converts a decimal integer into its hex "equivalent"
           This is useful where the ROM stores the data as decimal values (like gems in chests). */
        int Tens = Dec / 10;
        return (Tens*16) + (Dec - (Tens*10));
    }

    void ROMUpdateItems(vector<Item> RandomizedItemList, fstream &ROMFile) {

        ROMFile.seekp (CHEST_DATA_ADDRESS, ios::beg);

        unsigned char ChestItem;
        unsigned char ChestGems[2];
        unsigned char Byte;
        int GemTensAndUnits;
        bool DoubledChestDone = false;
        for (int i=0; i<NUMBER_OF_CHESTS; i++) {

            /* Put the cursor on the contents data for this chest */
            ROMFile.seekp (3, ios::cur);

            /* Update the contents */
            ChestItem = RandomizedItemList[i].Contents;
            ROMFile.write((char*)(&ChestItem), 1);
            GemTensAndUnits = RandomizedItemList[i].Gems % 100;
            ChestGems[0] = ConvertToHex(GemTensAndUnits);
            ChestGems[1] = ConvertToHex((RandomizedItemList[i].Gems - GemTensAndUnits) / 100);
            ROMFile.write((char*)(&ChestGems[0]), 2);

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

    }


    void ROMUpdateLairs(vector<Lair> RandomizedLairList, fstream &ROMFile) {

        ROMFile.seekp (MONSTER_LAIR_DATA_ADDRESS, ios::beg);

        unsigned char Byte;
        for (int i=0; i<NUMBER_OF_LAIRS; i++) {

            /* Ignore this data if it is an empty line                              ==> monster type = 0xFF
               or if it is one of the game's 5 empty lairs (like in Lisa's dream)   ==> monster type = 0x00 */
            do {
                ROMFile.seekg(21, ios::cur);
                ROMFile.read ((char*)(&Byte), 1);
                ROMFile.seekg(10, ios::cur);
            } while (Byte == 0x00 || Byte == 0xFF);
            ROMFile.seekg(-22, ios::cur);

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
            ROMFile.write((char*)(&RandomizedLairList[i].UpsideDownFlag), 1);
            ROMFile.seekp(8, ios::cur);
        }
    }
}
