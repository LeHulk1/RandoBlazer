#include "ROMData.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


/*
    List of chest contents:

     - Medical Herb x13         - Mole's Ribbon
     - Strange Bottle x5        - Big Pearl
     - Sword of Life            - Mermaid's Tears
     - Critical Sword           - Delicious Seeds
     - Lucky Blade              - Emblem B
     - Zantetsu Sword           - Emblem D
     - Spirit Sword             - Red-Hot Ball
     - Soul Blade               - Power Bracelet
     - Iron Armor               - 1 Gem
     - Ice Armor                - 12 Gems
     - Magic Armor              - 40 Gems
     - Light Armor              - 50 Gems x5
     - Soul Armor               - 60 Gems x2
     - Rotator                  - 80 Gems x5
     - Flame Pillar             - 100 Gems x4
     - Tornado                  - 150 Gems
     - Dream Rod                - 200 Gems
     - Leo's Brush              - Nothing x3

    Total: 66 chests
*/

namespace ROMData {

    void GetOriginalItemData(vector<ItemHolder> &ItemList) {

        ItemList[0].Contents  = SWORD_OF_LIFE;

        ItemList[1].Contents  = IRON_ARMOR;

        ItemList[2].Contents  = GEMS;
        ItemList[2].Gems      = 50;

        ItemList[3].Contents  = GEMS;
        ItemList[3].Gems      = 12;

        ItemList[4].Contents  = MEDICAL_HERB;

        ItemList[5].Contents  = DREAM_ROD;

        ItemList[6].Contents  = LEOS_BRUSH;

        ItemList[7].Contents  = MEDICAL_HERB;

        ItemList[8].Contents  = TORNADO;

        ItemList[9].Contents  = ICE_ARMOR;

        ItemList[10].Contents = MOLES_RIBBON;

        ItemList[11].Contents = GEMS;
        ItemList[11].Gems     = 50;

        ItemList[12].Contents = GEMS;
        ItemList[12].Gems     = 150;

        ItemList[13].Contents = MEDICAL_HERB;

        ItemList[14].Contents = DELICIOUS_SEEDS;

        ItemList[15].Contents = GEMS;
        ItemList[15].Gems     = 50;

        ItemList[16].Contents = MEDICAL_HERB;

        ItemList[17].Contents = NOTHING;

        ItemList[18].Contents = STRANGE_BOTTLE;

        ItemList[19].Contents = GEMS;
        ItemList[19].Gems     = 100;

        ItemList[20].Contents = GEMS;
        ItemList[20].Gems     = 60;

        ItemList[21].Contents = FLAME_PILLAR;

        ItemList[22].Contents = MERMAIDS_TEARS;

        ItemList[23].Contents = BIG_PEARL;

        ItemList[24].Contents = EMBLEM_D;

        ItemList[25].Contents = GEMS;
        ItemList[25].Gems     = 80;

        ItemList[26].Contents = MEDICAL_HERB;

        ItemList[27].Contents = NOTHING;

        ItemList[28].Contents = MEDICAL_HERB;

        ItemList[29].Contents = MEDICAL_HERB;

        ItemList[30].Contents = GEMS;
        ItemList[30].Gems     = 60;

        ItemList[31].Contents = CRITICAL_SWORD;

        ItemList[32].Contents = STRANGE_BOTTLE;

        ItemList[33].Contents = GEMS;
        ItemList[33].Gems     = 1;

        ItemList[34].Contents = POWER_BRACELET;

        ItemList[35].Contents = GEMS;
        ItemList[35].Gems     = 40;

        ItemList[36].Contents = MAGIC_ARMOR;

        ItemList[37].Contents = NOTHING;

        ItemList[38].Contents = MEDICAL_HERB;

        ItemList[39].Contents = GEMS;
        ItemList[39].Gems     = 50;

        ItemList[40].Contents = LUCKY_BLADE;

        ItemList[41].Contents = STRANGE_BOTTLE;

        ItemList[42].Contents = ROTATOR;

        ItemList[43].Contents = ZANTETSU_SWORD;

        ItemList[44].Contents = LIGHT_ARMOR;

        ItemList[45].Contents = GEMS;
        ItemList[45].Gems     = 50;

        ItemList[46].Contents = MEDICAL_HERB;

        ItemList[47].Contents = GEMS;
        ItemList[47].Gems     = 80;

        ItemList[48].Contents = STRANGE_BOTTLE;

        ItemList[49].Contents = MEDICAL_HERB;

        ItemList[50].Contents = GEMS;
        ItemList[50].Gems     = 80;

        ItemList[51].Contents = SPIRIT_SWORD;

        ItemList[52].Contents = STRANGE_BOTTLE;

        ItemList[53].Contents = MEDICAL_HERB;

        ItemList[54].Contents = GEMS;
        ItemList[54].Gems     = 100;

        ItemList[55].Contents = EMBLEM_B;

        ItemList[56].Contents = GEMS;
        ItemList[56].Gems     = 80;

        ItemList[57].Contents = GEMS;
        ItemList[57].Gems     = 80;

        ItemList[58].Contents = GEMS;
        ItemList[58].Gems     = 100;

        ItemList[59].Contents = MEDICAL_HERB;

        ItemList[60].Contents = MEDICAL_HERB;

        ItemList[61].Contents = GEMS;
        ItemList[61].Gems     = 200;

        ItemList[62].Contents = RED_HOT_BALL;

        ItemList[63].Contents = SOUL_ARMOR;

        ItemList[64].Contents = SOUL_BLADE;

        ItemList[65].Contents = GEMS;
        ItemList[65].Gems     = 100;


    }


    void GetOriginalLairData (vector<Lair> &LairList, fstream &ROMFile) {

        ROMFile.seekg (MONSTER_LAIR_DATA_ADDRESS, ios::beg);

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

            /* Read the contents of this Monster Lair */
            ROMFile.read((char*)(&(LairList[i].Act)), 1);
            ROMFile.read((char*)(&(LairList[i].PositionData[0])), POSITION_DATA_SIZE);
            ROMFile.seekg(2, ios::cur);
            ROMFile.read((char*)(&(LairList[i].Type[0])), LAIR_TYPE_SIZE);
            ROMFile.seekg(1, ios::cur);
            ROMFile.read((char*)(&(LairList[i].NbEnemies)), 1);
            ROMFile.read((char*)(&(LairList[i].SpawnRate)), 1);
            ROMFile.read((char*)(&(LairList[i].Enemy)), 1);
            ROMFile.seekg(1, ios::cur);
            ROMFile.read((char*)(&(LairList[i].UpsideDownFlag)), 1);
            ROMFile.seekg(8, ios::cur);
        }

    }
}
