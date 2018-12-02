#include "ROMData.h"
#include "Map.h"

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

    void GetOriginalItemData(std::vector<Item> &ItemList) {

        /***** CHESTS *****/

        ItemList[0].Contents  = SWORD_OF_LIFE;

        ItemList[1].Contents  = IRON_ARMOR;

        ItemList[2].Contents  = GEMS_EXP;
        ItemList[2].GemsExp   = 50;

        ItemList[3].Contents  = GEMS_EXP;
        ItemList[3].GemsExp   = 12;

        ItemList[4].Contents  = MEDICAL_HERB;

        ItemList[5].Contents  = DREAM_ROD;

        ItemList[6].Contents  = LEOS_BRUSH;

        ItemList[7].Contents  = MEDICAL_HERB;

        ItemList[8].Contents  = TORNADO;

        ItemList[9].Contents  = ICE_ARMOR;

        ItemList[10].Contents = MOLES_RIBBON;

        ItemList[11].Contents = GEMS_EXP;
        ItemList[11].GemsExp  = 50;

        ItemList[12].Contents = GEMS_EXP;
        ItemList[12].GemsExp  = 150;

        ItemList[13].Contents = MEDICAL_HERB;

        ItemList[14].Contents = DELICIOUS_SEEDS;

        ItemList[15].Contents = GEMS_EXP;
        ItemList[15].GemsExp  = 50;

        ItemList[16].Contents = MEDICAL_HERB;

        ItemList[17].Contents = NOTHING;

        ItemList[18].Contents = STRANGE_BOTTLE;

        ItemList[19].Contents = GEMS_EXP;
        ItemList[19].GemsExp  = 100;

        ItemList[20].Contents = GEMS_EXP;
        ItemList[20].GemsExp  = 60;

        ItemList[21].Contents = FLAME_PILLAR;

        ItemList[22].Contents = MERMAIDS_TEARS;

        ItemList[23].Contents = BIG_PEARL;

        ItemList[24].Contents = EMBLEM_D;

        ItemList[25].Contents = GEMS_EXP;
        ItemList[25].GemsExp  = 80;

        ItemList[26].Contents = MEDICAL_HERB;

        ItemList[27].Contents = NOTHING;

        ItemList[28].Contents = MEDICAL_HERB;

        ItemList[29].Contents = MEDICAL_HERB;

        ItemList[30].Contents = GEMS_EXP;
        ItemList[30].GemsExp  = 60;

        ItemList[31].Contents = CRITICAL_SWORD;

        ItemList[32].Contents = STRANGE_BOTTLE;

        ItemList[33].Contents = GEMS_EXP;
        ItemList[33].GemsExp  = 1;

        ItemList[34].Contents = POWER_BRACELET;

        ItemList[35].Contents = GEMS_EXP;
        ItemList[35].GemsExp  = 40;

        ItemList[36].Contents = MAGIC_ARMOR;

        ItemList[37].Contents = NOTHING;

        ItemList[38].Contents = MEDICAL_HERB;

        ItemList[39].Contents = GEMS_EXP;
        ItemList[39].GemsExp  = 50;

        ItemList[40].Contents = LUCKY_BLADE;

        ItemList[41].Contents = STRANGE_BOTTLE;

        ItemList[42].Contents = ROTATOR;

        ItemList[43].Contents = ZANTETSU_SWORD;

        ItemList[44].Contents = LIGHT_ARMOR;

        ItemList[45].Contents = GEMS_EXP;
        ItemList[45].GemsExp  = 50;

        ItemList[46].Contents = MEDICAL_HERB;

        ItemList[47].Contents = GEMS_EXP;
        ItemList[47].GemsExp  = 80;

        ItemList[48].Contents = STRANGE_BOTTLE;

        ItemList[49].Contents = MEDICAL_HERB;

        ItemList[50].Contents = GEMS_EXP;
        ItemList[50].GemsExp  = 80;

        ItemList[51].Contents = SPIRIT_SWORD;

        ItemList[52].Contents = STRANGE_BOTTLE;

        ItemList[53].Contents = MEDICAL_HERB;

        ItemList[54].Contents = GEMS_EXP;
        ItemList[54].GemsExp  = 100;

        ItemList[55].Contents = EMBLEM_B;

        ItemList[56].Contents = GEMS_EXP;
        ItemList[56].GemsExp  = 80;

        ItemList[57].Contents = GEMS_EXP;
        ItemList[57].GemsExp  = 80;

        ItemList[58].Contents = GEMS_EXP;
        ItemList[58].GemsExp  = 100;

        ItemList[59].Contents = MEDICAL_HERB;

        ItemList[60].Contents = MEDICAL_HERB;

        ItemList[61].Contents = GEMS_EXP;
        ItemList[61].GemsExp  = 200;

        ItemList[62].Contents = RED_HOT_BALL;

        ItemList[63].Contents = SOUL_ARMOR;

        ItemList[64].Contents = SOUL_BLADE;

        ItemList[65].Contents = GEMS_EXP;
        ItemList[65].GemsExp  = 100;

        /***** NPCs *****/

        ItemList[ITEM_TOOL_SHOP_OWNER].Type     = TYPE_NPC;
        ItemList[ITEM_TOOL_SHOP_OWNER].Contents = MEDICAL_HERB;

        ItemList[ITEM_EMBLEM_A].Type     = TYPE_NPC;
        ItemList[ITEM_EMBLEM_A].Contents = EMBLEM_A;

        ItemList[ITEM_GOAT_PEN].Type     = TYPE_NPC;
        ItemList[ITEM_GOAT_PEN].Contents = MEDICAL_HERB;

        ItemList[ITEM_TEDDY].Type     = TYPE_NPC;
        ItemList[ITEM_TEDDY].Contents = GOATS_FOOD;

        ItemList[ITEM_PASS].Type     = TYPE_NPC;
        ItemList[ITEM_PASS].Contents = PASS;

        ItemList[ITEM_SECRET_CAVE_TILE].Type     = TYPE_NPC;
        ItemList[ITEM_SECRET_CAVE_TILE].Contents = STRANGE_BOTTLE;

        ItemList[ITEM_VILLAGE_CHIEF].Type     = TYPE_NPC;
        ItemList[ITEM_VILLAGE_CHIEF].Contents = BROWN_STONE;

        ItemList[ITEM_MAGICIAN].Type     = TYPE_NPC;
        ItemList[ITEM_MAGICIAN].Contents = FLAME_BALL;

        ItemList[ITEM_CRYSTAL_RECOVERY_SWORD].Type     = TYPE_NPC;
        ItemList[ITEM_CRYSTAL_RECOVERY_SWORD].Contents = RECOVERY_SWORD;

        ItemList[ITEM_BIRD_RED_HOT_MIRROR].Type     = TYPE_NPC;
        ItemList[ITEM_BIRD_RED_HOT_MIRROR].Contents = RED_HOT_MIRROR;

        ItemList[ITEM_CRYSTAL_MAGIC_BELL].Type     = TYPE_NPC;
        ItemList[ITEM_CRYSTAL_MAGIC_BELL].Contents = MAGIC_BELL;

        ItemList[ITEM_WOODSTIN_TRIO].Type     = TYPE_NPC;
        ItemList[ITEM_WOODSTIN_TRIO].Contents = MEDICAL_HERB;

        ItemList[ITEM_GREENWOODS_GUARDIAN].Type     = TYPE_NPC;
        ItemList[ITEM_GREENWOODS_GUARDIAN].Contents = GREEN_STONE;

        ItemList[ITEM_GREENWOOD_LEAVES].Type     = TYPE_NPC;
        ItemList[ITEM_GREENWOOD_LEAVES].Contents = GREENWOOD_LEAF;

        ItemList[ITEM_MOLE_SHIELD_BRACELET].Type     = TYPE_NPC;
        ItemList[ITEM_MOLE_SHIELD_BRACELET].Contents = SHIELD_BRACELET;

        ItemList[ITEM_SQUIRREL_PSYCHO_SWORD].Type     = TYPE_NPC;
        ItemList[ITEM_SQUIRREL_PSYCHO_SWORD].Contents = PSYCHO_SWORD;

        ItemList[ITEM_SQUIRREL_EMBLEM_C].Type     = TYPE_NPC;
        ItemList[ITEM_SQUIRREL_EMBLEM_C].Contents = EMBLEM_C;

        ItemList[ITEM_WATER_SHRINE_TILE].Type     = TYPE_NPC;
        ItemList[ITEM_WATER_SHRINE_TILE].Contents = STRANGE_BOTTLE;

        ItemList[ITEM_CRYSTAL_LIGHT_ARROW].Type     = TYPE_NPC;
        ItemList[ITEM_CRYSTAL_LIGHT_ARROW].Contents = LIGHT_ARROW;

        ItemList[ITEM_MOUNTAIN_KING].Type     = TYPE_NPC;
        ItemList[ITEM_MOUNTAIN_KING].Contents = PHOENIX;

        ItemList[ITEM_BOY_MUSHROOM_SHOES].Type     = TYPE_NPC;
        ItemList[ITEM_BOY_MUSHROOM_SHOES].Contents = MUSHROOM_SHOES;

        ItemList[ITEM_NOME].Type     = TYPE_NPC;
        ItemList[ITEM_NOME].Contents = SILVER_STONE;

        ItemList[ITEM_SNAIL_EMBLEM_E].Type     = TYPE_NPC;
        ItemList[ITEM_SNAIL_EMBLEM_E].Contents = EMBLEM_E;

        ItemList[ITEM_EMBLEM_F].Type     = TYPE_NPC;
        ItemList[ITEM_EMBLEM_F].Contents = EMBLEM_F;

        ItemList[ITEM_EMBLEM_G].Type     = TYPE_NPC;
        ItemList[ITEM_EMBLEM_G].Contents = EMBLEM_G;

        ItemList[ITEM_CHEST_OF_DRAWERS_MYSTIC_ARMOR].Type     = TYPE_NPC;
        ItemList[ITEM_CHEST_OF_DRAWERS_MYSTIC_ARMOR].Contents = MYSTIC_ARMOR;

        ItemList[ITEM_PLANT_HERB].Type     = TYPE_NPC;
        ItemList[ITEM_PLANT_HERB].Contents = MEDICAL_HERB;

        ItemList[ITEM_CAT_DOOR_KEY].Type     = TYPE_NPC;
        ItemList[ITEM_CAT_DOOR_KEY].Contents = DOOR_KEY;

        ItemList[ITEM_PLANT_ACTINIDIA_LEAVES].Type     = TYPE_NPC;
        ItemList[ITEM_PLANT_ACTINIDIA_LEAVES].Contents = ACTINIDIA_LEAF;

        ItemList[ITEM_CHEST_OF_DRAWERS_HERB].Type     = TYPE_NPC;
        ItemList[ITEM_CHEST_OF_DRAWERS_HERB].Contents = MEDICAL_HERB;

        ItemList[ITEM_MARIE].Type     = TYPE_NPC;
        ItemList[ITEM_MARIE].Contents = PURPLE_STONE;

        ItemList[ITEM_MOUSE_SPARK_BOMB].Type     = TYPE_NPC;
        ItemList[ITEM_MOUSE_SPARK_BOMB].Contents = SPARK_BOMB;

        ItemList[ITEM_SOLDIER_ELEMENTAL_MAIL].Type     = TYPE_NPC;
        ItemList[ITEM_SOLDIER_ELEMENTAL_MAIL].Contents = ELEMENTAL_MAIL;

        ItemList[ITEM_SUPER_BRACELET].Type     = TYPE_NPC;
        ItemList[ITEM_SUPER_BRACELET].Contents = SUPER_BRACELET;

        ItemList[ITEM_QUEEN_MAGRIDD].Type     = TYPE_NPC;
        ItemList[ITEM_QUEEN_MAGRIDD].Contents = VIP_CARD;

        ItemList[ITEM_SOLDIER_PLATINUM_CARD].Type     = TYPE_NPC;
        ItemList[ITEM_SOLDIER_PLATINUM_CARD].Contents = PLATINUM_CARD;

        ItemList[ITEM_MAID_HERB].Type     = TYPE_NPC;
        ItemList[ITEM_MAID_HERB].Contents = MEDICAL_HERB;

        ItemList[ITEM_EMBLEM_H].Type     = TYPE_NPC;
        ItemList[ITEM_EMBLEM_H].Contents = EMBLEM_H;

        ItemList[ITEM_KING_MAGRIDD].Type     = TYPE_NPC;
        ItemList[ITEM_KING_MAGRIDD].Contents = BLACK_STONE;

        ItemList[ITEM_DR_LEO].Type     = TYPE_NPC;
        ItemList[ITEM_DR_LEO].Contents = MOBILE_KEY;

        ItemList[ITEM_HARP_STRING].Type     = TYPE_NPC;
        ItemList[ITEM_HARP_STRING].Contents = HARP_STRING;

        ItemList[ITEM_MERMAID_HERB].Type     = TYPE_NPC;
        ItemList[ITEM_MERMAID_HERB].Contents = MEDICAL_HERB;

        ItemList[ITEM_MERMAID_BUBBLE_ARMOR].Type     = TYPE_NPC;
        ItemList[ITEM_MERMAID_BUBBLE_ARMOR].Contents = BUBBLE_ARMOR;

        ItemList[ITEM_MERMAID_MAGIC_FLARE].Type     = TYPE_NPC;
        ItemList[ITEM_MERMAID_MAGIC_FLARE].Contents = MAGIC_FLARE;

        ItemList[ITEM_MERMAID_QUEEN].Type     = TYPE_NPC;
        ItemList[ITEM_MERMAID_QUEEN].Contents = BLUE_STONE;

        ItemList[ITEM_MERMAID_RED_HOT_STICK].Type     = TYPE_NPC;
        ItemList[ITEM_MERMAID_RED_HOT_STICK].Contents = RED_HOT_STICK;

        ItemList[ITEM_LUE].Type     = TYPE_NPC;
        ItemList[ITEM_LUE].Contents = THUNDER_RING;

        ItemList[ITEM_CRYSTAL_GRASS_VALLEY].Type     = TYPE_NPC;
        ItemList[ITEM_CRYSTAL_GRASS_VALLEY].Contents = GEMS_EXP;
        ItemList[ITEM_CRYSTAL_GRASS_VALLEY].GemsExp  = 80;

        ItemList[ITEM_CRYSTAL_UNDERGROUND_CASTLE].Type     = TYPE_NPC;
        ItemList[ITEM_CRYSTAL_UNDERGROUND_CASTLE].Contents = GEMS_EXP;
        ItemList[ITEM_CRYSTAL_UNDERGROUND_CASTLE].GemsExp  = 30;

        ItemList[ITEM_CRYSTAL_LOST_MARSH].Type     = TYPE_NPC;
        ItemList[ITEM_CRYSTAL_LOST_MARSH].Contents = GEMS_EXP;
        ItemList[ITEM_CRYSTAL_LOST_MARSH].GemsExp  = 150;

        ItemList[ITEM_CRYSTAL_WATER_SHRINE].Type     = TYPE_NPC;
        ItemList[ITEM_CRYSTAL_WATER_SHRINE].Contents = GEMS_EXP;
        ItemList[ITEM_CRYSTAL_WATER_SHRINE].GemsExp  = 180;

        ItemList[ITEM_CRYSTAL_FIRE_SHRINE].Type     = TYPE_NPC;
        ItemList[ITEM_CRYSTAL_FIRE_SHRINE].Contents = GEMS_EXP;
        ItemList[ITEM_CRYSTAL_FIRE_SHRINE].GemsExp  = 1;

        ItemList[ITEM_CRYSTAL_MOUNTAIN_OF_SOULS].Type     = TYPE_NPC;
        ItemList[ITEM_CRYSTAL_MOUNTAIN_OF_SOULS].Contents = GEMS_EXP;
        ItemList[ITEM_CRYSTAL_MOUNTAIN_OF_SOULS].GemsExp  = 300;

        ItemList[ITEM_CRYSTAL_LUNE].Type     = TYPE_NPC;
        ItemList[ITEM_CRYSTAL_LUNE].Contents = GEMS_EXP;
        ItemList[ITEM_CRYSTAL_LUNE].GemsExp  = 400;

        ItemList[ITEM_CRYSTAL_LEOS_LAB_BASEMENT].Type     = TYPE_NPC;
        ItemList[ITEM_CRYSTAL_LEOS_LAB_BASEMENT].Contents = GEMS_EXP;
        ItemList[ITEM_CRYSTAL_LEOS_LAB_BASEMENT].GemsExp  = 300;

        ItemList[ITEM_CRYSTAL_MODEL_TOWN].Type     = TYPE_NPC;
        ItemList[ITEM_CRYSTAL_MODEL_TOWN].Contents = GEMS_EXP;
        ItemList[ITEM_CRYSTAL_MODEL_TOWN].GemsExp  = 300;

        ItemList[ITEM_CRYSTAL_POWER_PLANT].Type     = TYPE_NPC;
        ItemList[ITEM_CRYSTAL_POWER_PLANT].Contents = GEMS_EXP;
        ItemList[ITEM_CRYSTAL_POWER_PLANT].GemsExp  = 300;

        ItemList[ITEM_CRYSTAL_ROCKBIRD].Type     = TYPE_NPC;
        ItemList[ITEM_CRYSTAL_ROCKBIRD].Contents = GEMS_EXP;
        ItemList[ITEM_CRYSTAL_ROCKBIRD].GemsExp  = 200;

        ItemList[ITEM_CRYSTAL_SEABED_NEAR_BLESTER].Type     = TYPE_NPC;
        ItemList[ITEM_CRYSTAL_SEABED_NEAR_BLESTER].Contents = GEMS_EXP;
        ItemList[ITEM_CRYSTAL_SEABED_NEAR_BLESTER].GemsExp  = 300;

        ItemList[ITEM_CRYSTAL_SEABED_NEAR_DUREAN].Type     = TYPE_NPC;
        ItemList[ITEM_CRYSTAL_SEABED_NEAR_DUREAN].Contents = GEMS_EXP;
        ItemList[ITEM_CRYSTAL_SEABED_NEAR_DUREAN].GemsExp  = 250;
    }


    void GetOriginalLairData (std::vector<Lair> &LairList, std::fstream &ROMFile) {

        ROMFile.seekg (MONSTER_LAIR_DATA_ADDRESS, std::ios::beg);

        for (int i=0; i<NUMBER_OF_LAIRS; i++) {

            ROMFile.seekg(10, std::ios::cur);

            /* Read the contents of this Monster Lair */
            ROMFile.read((char*)(&(LairList[i].Act)), 1);
            ROMFile.read((char*)(&(LairList[i].PositionData[0])), POSITION_DATA_SIZE);
            ROMFile.seekg(2, std::ios::cur);
            ROMFile.read((char*)(&(LairList[i].Type[0])), LAIR_TYPE_SIZE);
            ROMFile.seekg(1, std::ios::cur);
            ROMFile.read((char*)(&(LairList[i].NbEnemies)), 1);
            ROMFile.read((char*)(&(LairList[i].SpawnRate)), 1);
            ROMFile.read((char*)(&(LairList[i].Enemy)), 1);
            ROMFile.seekg(1, std::ios::cur);
            ROMFile.read((char*)(&(LairList[i].Orientation)), 1);
            ROMFile.seekg(8, std::ios::cur);
        }
    }


    static int SpriteDataAddressList[7][63] = {
        {0x9CCA, 0x9CCE, 0x9CD2, 0x9CD6, 0x9CDA, 0x9CDE, 0x9CE2, 0x9CE6, 0x9CEA, 0x9CEE,
         0x9CF2, 0x9D1B, 0x9D1F, 0x9D23, 0x9D27, 0x9D2B, 0x9D2F, 0x9D33, 0x9D37, 0x9D3B,
         0x9D3F, 0x9D43, 0x9D47, 0x9D4B, 0x9D4F, 0x9D53, 0x9D57},
        {0x9F01, 0x9F05, 0x9F09, 0x9F0D, 0x9F11, 0x9F15, 0x9F19, 0x9F1D, 0x9F21, 0x9F25,
         0x9F29, 0x9F2D, 0x9F31, 0x9F35, 0x9F6A, 0x9F6E, 0x9F72, 0x9F76, 0x9F93, 0x9F97,
         0x9F9B, 0x9F9F, 0x9FA3, 0x9FA7, 0x9FAB},
        {0xA11C, 0xA120, 0xA124, 0xA128, 0xA12C, 0xA130, 0xA194, 0xA198, 0xA19C, 0xA1A0,
         0xA1A4, 0xA1A8, 0xA1AC, 0xA1B0, 0xA1B4, 0xA1B8, 0xA1D1, 0xA1D5, 0xA1D9, 0xA1DD,
         0xA1E1, 0xA1E5, 0xA1E9, 0xA1ED, 0xA202, 0xA206, 0xA20A, 0xA20E, 0xA212, 0xA243,
         0xA247, 0xA24B, 0xA24F, 0xA253, 0xA28C, 0xA290, 0xA294, 0xA298},
        {0xA3B9, 0xA3BD, 0xA3C1, 0xA3C5, 0xA3C9, 0xA407, 0xA40B, 0xA40F, 0xA413, 0xA42C,
         0xA430, 0xA434, 0xA438, 0xA47B, 0xA47F, 0xA498, 0xA49C, 0xA4A0, 0xA4A4, 0xA4C5,
         0xA4C9, 0xA4CD, 0xA4D1, 0xA4D5, 0xA4D9, 0xA4DD, 0xA4E1},
        {0xA608, 0xA60C, 0xA610, 0xA614, 0xA618, 0xA61C, 0xA63D, 0xA641, 0xA645, 0xA649,
         0xA64D, 0xA651},
        {0xA6CB, 0xA6CF, 0xA6D3, 0xA6D7, 0xA6DB, 0xA6DF, 0xA6E3, 0xA6E7, 0xA6EB, 0xA6EF,
         0xA6F3, 0xA708, 0xA70C, 0xA710, 0xA714, 0xA718, 0xA71C, 0xA751, 0xA755, 0xA759,
         0xA75D, 0xA761, 0xA765, 0xA77F, 0xA783, 0xA787, 0xA78B, 0xA78F, 0xA793, 0xA797,
         0xA79B, 0xA79F, 0xA7A3, 0xA7A7, 0xA7AB, 0xA7BC, 0xA7C0, 0xA7C4, 0xA7C8, 0xA7CC,
         0xA7D0, 0xA7D4, 0xA7D8, 0xA7F6, 0xA7FA, 0xA7FE, 0xA802, 0xA806, 0xA80A, 0xA81B,
         0xA81F, 0xA823, 0xA827, 0xA834, 0xA838, 0xA83C, 0xA840, 0xA844, 0xA848, 0xA84C,
         0xA850, 0xA861, 0xA865},                                /* leave these 3 bricks for grinding */
        {0xA946, 0xA94A, 0xA94E, 0xA952, 0xA956, 0xA95A, 0xA95E, /*0xA987, 0xA98B, 0xA98F,*/
         0xA993, 0xA997, 0xA99B, 0xA99F, 0xA9A3, 0xA9A7}
    };

    void GetOriginalMapSpriteData (std::vector<Sprite> &SpriteList, std::fstream &ROMFile) {

        int SpriteIndex = 0;
        for (int Act = ACT_1; Act < 7; ++Act) {
            for (int AddressIndex = 0; AddressIndex < 63; ++AddressIndex) {
                if (SpriteDataAddressList[Act][AddressIndex] == 0) {
                    break;
                }
                SpriteList[SpriteIndex].Address = SpriteDataAddressList[Act][AddressIndex];
                SpriteList[SpriteIndex].Act = Act;

                ROMFile.seekg(SpriteDataAddressList[Act][AddressIndex], std::ios::beg);
                ROMFile.read((char*)(&(SpriteList[SpriteIndex].x)), 1);
                ROMFile.read((char*)(&(SpriteList[SpriteIndex].y)), 1);
                ROMFile.read((char*)(&(SpriteList[SpriteIndex].Orientation)), 1);
                ROMFile.read((char*)(&(SpriteList[SpriteIndex].Enemy)), 1);
                SpriteIndex++;
            }
        }
    }

    bool NPCOriginallyGivesEXP(int NPCIndex) {
        switch (NPCIndex) {
        case ITEM_CRYSTAL_GRASS_VALLEY:
        case ITEM_CRYSTAL_UNDERGROUND_CASTLE:
        case ITEM_CRYSTAL_LOST_MARSH:
        case ITEM_CRYSTAL_WATER_SHRINE:
        case ITEM_CRYSTAL_FIRE_SHRINE:
        case ITEM_CRYSTAL_MOUNTAIN_OF_SOULS:
        case ITEM_CRYSTAL_LUNE:
        case ITEM_CRYSTAL_LEOS_LAB_BASEMENT:
        case ITEM_CRYSTAL_MODEL_TOWN:
        case ITEM_CRYSTAL_POWER_PLANT:
        case ITEM_CRYSTAL_ROCKBIRD:
        case ITEM_CRYSTAL_SEABED_NEAR_BLESTER:
        case ITEM_CRYSTAL_SEABED_NEAR_DUREAN:
            return true;
            break;
        default:
            return false;
        }
    }
}
