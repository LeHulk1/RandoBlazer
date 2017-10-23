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

    void GetOriginalChestData(vector<Chest> &ChestList) {

        ChestList[0].Contents = SWORD_OF_LIFE;

        ChestList[1].Contents = IRON_ARMOR;

        ChestList[2].Contents = GEMS;
        ChestList[2].Gems     = 50;

        ChestList[3].Contents = GEMS;
        ChestList[3].Gems     = 12;

        ChestList[4].Contents = MEDICAL_HERB;

        ChestList[5].Contents = DREAM_ROD;

        ChestList[6].Contents = LEOS_BRUSH;

        ChestList[7].Contents = MEDICAL_HERB;

        ChestList[8].Contents = TORNADO;

        ChestList[9].Contents = ICE_ARMOR;

        ChestList[10].Contents = MOLES_RIBBON;

        ChestList[11].Contents = GEMS;
        ChestList[11].Gems     = 50;

        ChestList[12].Contents = GEMS;
        ChestList[12].Gems     = 150;

        ChestList[13].Contents = MEDICAL_HERB;

        ChestList[14].Contents = DELICIOUS_SEEDS;

        ChestList[15].Contents = GEMS;
        ChestList[15].Gems     = 50;

        ChestList[16].Contents = MEDICAL_HERB;

        ChestList[17].Contents = NOTHING;

        ChestList[18].Contents = STRANGE_BOTTLE;

        ChestList[19].Contents = GEMS;
        ChestList[19].Gems     = 100;

        ChestList[20].Contents = GEMS;
        ChestList[20].Gems     = 60;

        ChestList[21].Contents = FLAME_PILLAR;

        ChestList[22].Contents = MERMAIDS_TEARS;

        ChestList[23].Contents = BIG_PEARL;

        ChestList[24].Contents = EMBLEM_D;

        ChestList[25].Contents = GEMS;
        ChestList[25].Gems     = 80;

        ChestList[26].Contents = MEDICAL_HERB;

        ChestList[27].Contents = NOTHING;

        ChestList[28].Contents = MEDICAL_HERB;

        ChestList[29].Contents = MEDICAL_HERB;

        ChestList[30].Contents = GEMS;
        ChestList[30].Gems     = 60;

        ChestList[31].Contents = CRITICAL_SWORD;

        ChestList[32].Contents = STRANGE_BOTTLE;

        ChestList[33].Contents = GEMS;
        ChestList[33].Gems     = 1;

        ChestList[34].Contents = POWER_BRACELET;

        ChestList[35].Contents = GEMS;
        ChestList[35].Gems     = 40;

        ChestList[36].Contents = MAGIC_ARMOR;

        ChestList[37].Contents = NOTHING;

        ChestList[38].Contents = MEDICAL_HERB;

        ChestList[39].Contents = GEMS;
        ChestList[39].Gems     = 50;

        ChestList[40].Contents = LUCKY_BLADE;

        ChestList[41].Contents = STRANGE_BOTTLE;

        ChestList[42].Contents = ROTATOR;

        ChestList[43].Contents = ZANTETSU_SWORD;

        ChestList[44].Contents = LIGHT_ARMOR;

        ChestList[45].Contents = GEMS;
        ChestList[45].Gems     = 50;

        ChestList[46].Contents = MEDICAL_HERB;

        ChestList[47].Contents = GEMS;
        ChestList[47].Gems     = 80;

        ChestList[48].Contents = STRANGE_BOTTLE;

        ChestList[49].Contents = MEDICAL_HERB;

        ChestList[50].Contents = GEMS;
        ChestList[50].Gems     = 80;

        ChestList[51].Contents = SPIRIT_SWORD;

        ChestList[52].Contents = STRANGE_BOTTLE;

        ChestList[53].Contents = MEDICAL_HERB;

        ChestList[54].Contents = GEMS;
        ChestList[54].Gems     = 100;

        ChestList[55].Contents = EMBLEM_B;

        ChestList[56].Contents = GEMS;
        ChestList[56].Gems     = 80;

        ChestList[57].Contents = GEMS;
        ChestList[57].Gems     = 80;

        ChestList[58].Contents = GEMS;
        ChestList[58].Gems     = 100;

        ChestList[59].Contents = MEDICAL_HERB;

        ChestList[60].Contents = MEDICAL_HERB;

        ChestList[61].Contents = GEMS;
        ChestList[61].Gems     = 200;

        ChestList[62].Contents = RED_HOT_BALL;

        ChestList[63].Contents = SOUL_ARMOR;

        ChestList[64].Contents = SOUL_BLADE;

        ChestList[65].Contents = GEMS;
        ChestList[65].Gems     = 100;
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
