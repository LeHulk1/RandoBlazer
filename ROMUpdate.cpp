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

    static int NPCItemAddressList[59] =
    {
        0x183AB, /* Tool shop owner */
        0x1875E, /* Emblem A tile */
        0x18960, /* Goat pen corner */
        0x19253, /* Tool shop owner's son Teddy */
        0x1983B, /* A Pass */
        0x19914, /* Tile in child's secret cave */
        0x1A137, /* Village Chief */
        0x1A7E4, /* Magician (start of the game) */
        0x1AB78, /* Recovery Sword crystal */
        0x1AC26, /* Grass Valley secret room crystal */
        0x1AD15, /* Underground Castle 1st part crystal */
        0x1BE54, /* Red-Hot Mirror bird */
        0x1C116, /* Magic Bell crystal */
        0x1D120, /* Woodstin Trio */
        0x1D525, /* Greenwood's Guardian */
        0x1D81E, /* Greenwood Leaves */
        0x1DC17, /* Shield Bracelet mole */
        0x1E039, /* Psycho Sword squirrel */
        0x1E1E3, /* Emblem C squirrel */
        0x1E496, /* Water Shrine Strange Bottle */
        0x1E569, /* Light Arrow crystal */
        0x1E4E3, /* Lost Marsh crystal */
        0x1E537, /* Water Shrine crystal */
        0x1E5B5, /* Fire Shrine crystal */
        0x205A5, /* Mountain King */
        0x20D63, /* Mushroom Shoes boy */
        0x210C1, /* Nome */
        0x21A6E, /* Emblem E snail */
        0x21EA7, /* Emblem F tile */
        0x21EF5, /* Mountain of Souls crystal */
        0x21F49, /* Lune crystal */
        0x22A40, /* Emblem G under chest of drawers */
        0x22A5B, /* Chest of drawers (Mystic Armor) */
        0x22BC1, /* Herb Plant in Leo's Lab */
        0x22FB3, /* Leo's Cat (door key) */
        0x231AB, /* Actinidia plant */
        0x2354D, /* Marie */
        0x238F6, /* Spark Bomb mouse */
        0x23DFA, /* Leo's Lab Basement crystal */
        0x23E4E, /* Model Town 1 crystal */
        0x23E7E, /* Power Plant crystal */
        0x242A3, /* Elemental Mail soldier */
        0x2499B, /* Super Bracelet tile */
        0x249C2, /* Queen Magridd (VIP card) */
        0x24C44, /* Platinum Card soldier */
        0x24E94, /* Maid (Herb) */
        0x25345, /* Emblem H tile */
        0x255D9, /* Magridd King */
        0x25F51, /* Leo on the Airship deck (Mobile key) */
        0x26A0E, /* Harp String tile */
        0xF8315, /* North-eastern Mermaid (Herb) */
        0xF8B9F, /* Bubble Armor Mermaid */
        0xF9097, /* Magic Flair Mermaid */
        0xF9223, /* Mermaid Queen */
        0xF9871, /* Red-Hot Stick Mermaid */
        0xF9BEB, /* Lue */
        0xFA467, /* Rockbird crystal */
        0xFA4BB, /* Seabed crystal near Blester */
        0xFA4EB, /* Seabed crystal near Durean */
    };

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

    int ConvertToHex(int Dec) {
        /* Converts a decimal integer into its hex "equivalent"
           This is useful where the ROM stores the data as decimal values (like gems in chests). */
        int Tens = Dec / 10;
        return (Tens*16) + (Dec - (Tens*10));
    }

    void ROMUpdateItems(vector<Item> RandomizedItemList, fstream &ROMFile) {

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


        /********************/
        /* Update NPC Items */
        /********************/

        int ItemAddress;
        for (int i=NUMBER_OF_CHESTS; i<NUMBER_OF_ITEMS; i++) {
            ItemAddress = NPCItemAddressList[i-NUMBER_OF_CHESTS];
            ItemID = RandomizedItemList[i].Contents;

            if (ItemID == GEMS_EXP || ItemID == NOTHING) {

                if (NPCOriginallyGivesEXP(i)) {
                    ROMFile.seekp (ItemAddress, ios::beg);
                    GemsExp_TensAndUnits = RandomizedItemList[i].GemsExp % 100;
                    GemsExpValue[0] = ConvertToHex(GemsExp_TensAndUnits);
                    GemsExpValue[1] = ConvertToHex((RandomizedItemList[i].GemsExp - GemsExp_TensAndUnits) / 100);
                    ROMFile.write((char*)(&GemsExpValue[0]), 2);
                }
                else {
                    /* If the NPC doesn't normally give EXP, let us turn the
                       prize into a Medical Herb for now. */
                    ROMFile.seekp (ItemAddress, ios::beg);
                    ItemID = MEDICAL_HERB;
                    ROMFile.write((char*)(&ItemID), 1);
                }
            }
            else {

                if (NPCOriginallyGivesEXP(i)) {
                    /* If the NPC is a crystal fairy which normally gives EXP,
                       we need to do some tweaking to make it give an item. */
                    ROMFile.seekp (ItemAddress-1, ios::beg);
                    Byte = 0x0A;
                    ROMFile.write((char*)(&Byte), 1);
                    ROMFile.write((char*)(&ItemID), 1);

                    if (i == ITEM_CRYSTAL_FIRE_SHRINE) {
                        /* This one is really weird, the textbox sometimes glitches out */
                        Byte = 0x02;
                        ROMFile.write((char*)(&Byte), 1);
                    }
                    else {
                        Byte = 0x00;
                        ROMFile.write((char*)(&Byte), 1);
                    }
                }
                else {
                    ROMFile.seekp (ItemAddress, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                }
            }

            if (ItemID != GEMS_EXP && ItemID != NOTHING) {
                /* For some NPCs we need to update their text conditions, otherwise it might
                   not give the wished item if the player already has its original item. */
                switch (i) {
                case ITEM_TOOL_SHOP_OWNER:
                    ROMFile.seekp (0x1839B, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_TEDDY:
                    ROMFile.seekp (0x1922E, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_BIRD_RED_HOT_MIRROR:
                    ROMFile.seekp (0x1BE3D, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    ROMFile.seekp (0x1BE44, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_CRYSTAL_MAGIC_BELL:
                    ROMFile.seekp (0x1C0C6, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_MOLE_SHIELD_BRACELET:
                    ROMFile.seekp (0x1DC07, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_SQUIRREL_PSYCHO_SWORD:
                    ROMFile.seekp (0x1E01C, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_MOUNTAIN_KING:
                    ROMFile.seekp (0x203C1, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    ROMFile.seekp (0x204DF, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    ROMFile.seekp (0x2058A, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    ROMFile.seekp (0x20635, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_BOY_MUSHROOM_SHOES:
                    ROMFile.seekp (0x20D53, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_CHEST_OF_DRAWERS_MYSTIC_ARMOR:
                    ROMFile.seekp (0x22A6B, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_PLANT_HERB:
                    ROMFile.seekp (0x22BB1, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_PLANT_ACTINIDIA_LEAVES:
                    ROMFile.seekp (0x2319B, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_MOUSE_SPARK_BOMB:
                    ROMFile.seekp (0x238E6, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_SUPER_BRACELET:
                    ROMFile.seekp (0x24983, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_QUEEN_MAGRIDD:
                    ROMFile.seekp (0x249A1, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_SOLDIER_PLATINUM_CARD:
                    ROMFile.seekp (0x24BEF, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_MAID_HERB:
                    ROMFile.seekp (0x24E83, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_MERMAID_HERB:
                    ROMFile.seekp (0xF8305, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_MERMAID_MAGIC_FLARE:
                    ROMFile.seekp (0xF9087, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_MERMAID_RED_HOT_STICK:
                    ROMFile.seekp (0xF985A, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    ROMFile.seekp (0xF9861, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                case ITEM_LUE:
                    ROMFile.seekp (0xF9BB7, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    /* Modify text conditions for Mermmaid guarding Lue's prison entrance */
                    ROMFile.seekp (0xF98FE, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    ROMFile.seekp (0xF99D3, ios::beg);
                    ROMFile.write((char*)(&ItemID), 1);
                    break;
                default:
                    /* Do nothing */
                    break;
                }
            }

            /* Update the NPC's text accordingly */
            TextUpdate::NPCItemTextUpdate(i, ItemID, ROMFile);
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
