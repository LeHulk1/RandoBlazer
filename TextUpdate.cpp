#include "Item.h"
#include "Map.h"
#include "ROMData.h"
#include "TextUpdate.h"

#include <iostream>
#include <fstream>
#include <string.h>

#define NB_NPC_TO_DISABLE_ADDRESSES 22

#define TEXT_ENDTYPE_52FA 0
#define TEXT_ENDTYPE_88B9 1
#define TEXT_ENDTYPE_46EC 2



/***** Macros to write into ROM *****/

#define TEXT_WriteByte(_Byte_)        \
{                                     \
    Byte = _Byte_;                    \
    ROMFile.write((char*)(&Byte), 1); \
}

#define TEXT_WriteString(_String_) {ROMFile.write(_String_, strlen(_String_));}


/* Handle the different types of text ending (yeah, apparently it's a thing) */
#define TEXT_EndText(_Type_)  \
{                             \
    TEXT_WriteByte(0x13);     \
    switch (_Type_) {         \
    case TEXT_ENDTYPE_52FA:   \
        TEXT_WriteByte(0x52); \
        TEXT_WriteByte(0xFA); \
    break;                    \
    case TEXT_ENDTYPE_88B9:   \
        TEXT_WriteByte(0x88); \
        TEXT_WriteByte(0xB9); \
    break;                    \
    case TEXT_ENDTYPE_46EC:   \
        TEXT_WriteByte(0x46); \
        TEXT_WriteByte(0xEC); \
    break;                    \
    }                         \
}

#define TEXT_HeroFound    \
{                         \
    TEXT_WriteByte(0x02); \
    TEXT_WriteByte(0x02); \
    TEXT_WriteByte(0xAF); \
    TEXT_WriteByte(0x0D); \
}                         \

#define TEXT_HeroReceived \
{                         \
    TEXT_WriteByte(0x02); \
    TEXT_WriteByte(0x02); \
    TEXT_WriteByte(0x20); \
    TEXT_WriteByte(0xD4); \
    TEXT_WriteByte(0x0D); \
}                         \

#define TEXT_YellowStyle  \
{                         \
    TEXT_WriteByte(0x03); \
    TEXT_WriteByte(0x24); \
}                         \

#define TEXT_EndStyle     \
{                         \
    /*TEXT_WriteByte(0x20);*/ \
    TEXT_WriteByte(0x03); \
    TEXT_WriteByte(0x20); \
}                         \



namespace TextUpdate {

    static int NPCToDisableAddressList[NB_NPC_TO_DISABLE_ADDRESSES] = {
        0x1815A, /* Old Woman */
        0x184BA, /* Tulip next to Village Chief's house */
        0x18775, /* Water mill keeper */
        0x18BC4, /* Lisa */
        0x19506, /* Old man in Leo's Paintings house */
        0x1A13A, /* Village Chief */
        0x1AA56, /* Tulip next to Underground Castle */
        0x1AC5B, /* Revival of first Underground Castle Crystal Fairy */
        0x1CA7F, /* Sleeping bird */
        0x1D532, /* Greenwood's Guardian */
        0x1D869, /* Mole (helping soul) */
        0x22E7F, /* Great Door (helping soul) */
        0x22FF4, /* Leo's cat */
        0x23FC5, /* Soldier next to basement entrance */
        0x24112, /* Soldier next to Castle entrance */
        0x24616, /* Singer */
        0x2521B, /* Soldier (helping soul) */
        0x25BAF, /* Soldier with Leo */
        0x25FE0, /* Dr. Leo (when the two soldiers are not present) */
        0x26033, /* Dr. Leo (when the two soldiers are present) */
        0xF8109, /* Angelfish (helping soul) */
        0xF8ACF  /* Mermaid statues */
    };

    static int NPCItemTextAddressList[58] = {
        0x183AE, /* Tool shop owner */
        0x188FD, /* Emblem A tile */
        0x18979, /* Goat pen corner */
        0x19256, /* Tool shop owner's son Teddy */
        0x19870, /* A Pass */
        0x199DD, /* Tile at end of child's secret cave */
        0x1A192, /* Village Chief */
        0x1A7FA, /* Magician (start of the game) */
        0x1AB84, /* Recovery Sword crystal */
        0x1ABBA, /* Grass Valley secret room crystal */
        0x1ACBB, /* Underground Castle 1st part crystal */ // D3 AB
        0x1BE57, /* Red-Hot Mirror bird */ // 46 EC
        0x1C1D9, /* Magic Bell crystal */ // 46 EC
        0x1D292, /* Woodstin Trio */  // 46 EC
        0x1D53B, /* Greenwood's Guardian */ // 46 EC
        0x1D827, /* Greenwood Leaves */ // 46 EC
        0x1DC1A, /* Shield Bracelet mole */ // 46 EC
        0x1E046, /* Psycho Sword squirrel */ // 46 EC
        0x1E1EF, /* Emblem C squirrel */ // 46 EC
        0x1E49F, /* Water Shrine Strange Bottle */ // 46 EC
        0x1E572, /* Light Arrow crystal */
        0x1EBC3, /* Lost Marsh crystal */
        0x1EBC3, /* Water Shrine crystal */
        0x207D3, /* Mountain King */
        0x20D74, /* Mushroom Shoes boy */
        0x210CD, /* Nome */
        0x21A7A, /* Emblem E snail */
        0x21EB0, /* Emblem F tile */
        0x2249B, /* Mountain of Souls crystal */
        0x2249B, /* Lune crystal */
        0x22ADD, /* Emblem G under chest of drawers */
        0x22A7C, /* Chest of drawers (Mystic Armor) */
        0x22BC9, /* Herb Plant in Leo's Lab */
        0x2306D, /* Leo's Cat (door key) */
        0x231AE, /* Actinidia plant */
        0x23562, /* Marie */
        0x238F9, /* Spark Bomb mouse */
        0x23F34, /* Leo's Lab Basement crystal */
        0x23F34, /* Model Town 1 crystal */
        0x23F34, /* Power Plant crystal */
        0x242EC, /* Elemental Mail soldier */
        0x24AB7, /* Super Bracelet tile */
        0x249D9, /* Queen Magridd (VIP card) */
        0x24C49, /* Platinum Card soldier */
        0x24E9E, /* Maid (Herb) */
        0x253C4, /* Emblem H tile */
        0x255EE, /* Magridd King */
        0x264C4, /* Leo on the Airship deck (Mobile key) */
        0x26A17, /* Harp String tile */
        0xF831D, /* North-eastern Mermaid (Herb) */
        0xF8BBD, /* Bubble Armor Mermaid */
        0xF909A, /* Magic Flair Mermaid */
        0xF9253, /* Mermaid Queen */
        0xF9874, /* Red-Hot Stick Mermaid */
        0xF9BFB, /* Lue */
        0xFA9C6, /* Rockbird crystal */
        0xFA9C6, /* Seabed crystal near Blester */
        0xFA9C6  /* Seabed crystal near Durean */
    };

    static int NPCAlreadyHaveItemTextAddressList[58] = {
        0x18429, /* Tool shop owner */
        0, /* Emblem A tile */
        0, /* Goat pen corner */
        0x19320, /* Tool shop owner's son Teddy */
        0, /* A Pass */
        0, /* Tile at end of child's secret cave */
        0, /* Village Chief */
        0, /* Magician (start of the game) */
        0, /* Recovery Sword crystal */
        0, /* Grass Valley secret room crystal */
        0, /* Underground Castle 1st part crystal */
        0x1BED3, /* Red-Hot Mirror bird */
        0x1C225, /* Magic Bell crystal */
        0, /* Woodstin Trio */
        0, /* Greenwood's Guardian */
        0, /* Greenwood Leaves */
        0x1DD73, /* Shield Bracelet mole */
        0x1E11C, /* Psycho Sword squirrel */
        0x1E22C, /* Emblem C squirrel */
        0x1E49F, /* Water Shrine Strange Bottle */
        0x1E572, /* Light Arrow crystal */
        0x1EBC3, /* Lost Marsh crystal */
        0x1EBC3, /* Water Shrine crystal */
        0x207D3, /* Mountain King */
        0x20D74, /* Mushroom Shoes boy */
        0x210CD, /* Nome */
        0x21A7A, /* Emblem E snail */
        0x21EB0, /* Emblem F tile */
        0x2249B, /* Mountain of Souls crystal */
        0x2249B, /* Lune crystal */
        0x22ADD, /* Emblem G under chest of drawers */
        0x22A7C, /* Chest of drawers (Mystic Armor) */
        0x22BC9, /* Herb Plant in Leo's Lab */
        0x2306D, /* Leo's Cat (door key) */
        0x231AE, /* Actinidia plant */
        0x23562, /* Marie */
        0x238F9, /* Spark Bomb mouse */
        0x23F34, /* Leo's Lab Basement crystal */
        0x23F34, /* Model Town 1 crystal */
        0x23F34, /* Power Plant crystal */
        0x242EC, /* Elemental Mail soldier */
        0x24AB7, /* Super Bracelet tile */
        0x249D9, /* Queen Magridd (VIP card) */
        0x24C49, /* Platinum Card soldier */
        0x24E9E, /* Maid (Herb) */
        0x253C4, /* Emblem H tile */
        0x255EE, /* Magridd King */
        0x264C4, /* Leo on the Airship deck (Mobile key) */
        0x26A17, /* Harp String tile */
        0xF831D, /* North-eastern Mermaid (Herb) */
        0xF8BBD, /* Bubble Armor Mermaid */
        0xF909A, /* Magic Flair Mermaid */
        0xF9253, /* Mermaid Queen */
        0xF9874, /* Red-Hot Stick Mermaid */
        0xF9BFB, /* Lue */
        0xFA9C6, /* Rockbird crystal */
        0xFA9C6, /* Seabed crystal near Blester */
        0xFA9C6  /* Seabed crystal near Durean */
    };


    static string ItemNameList[65] = {
        "Nothing",
        "Sword of Life",
        "Psycho Sword",
        "Critical Sword",
        "Lucky Blade",
        "Zantetsu Sword",
        "Spirit Sword",
        "Recovery Sword",
        "Soul Blade",
        "Iron Armor",
        "Ice Armor",
        "Bubble Armor",
        "Magic Armor",
        "Mystic Armor",
        "Light Armor",
        "Elemental Mail",
        "Soul Armor",
        "Flame Ball",
        "Light Arrow",
        "Magic Flare",
        "Rotator",
        "Spark Bomb",
        "Flame Pillar",
        "Tornado",
        "Phoenix",
        "Goat's Food",
        "Harp String",
        "Pass",
        "Dream Rod",
        "Leo's Brush",
        "Greenwood Leaf",
        "Mole's Ribbon",
        "Big Pearl",
        "Mermaid's Tears",
        "Mushroom Shoes",
        "Mobile Key",
        "Thunder Ring",
        "Delicious Seeds",
        "Actinidia Leaf",
        "Door Key",
        "Platinum Card",
        "VIP Card",
        "Emblem A",
        "Emblem B",
        "Emblem C",
        "Emblem D",
        "Emblem E",
        "Emblem F",
        "Emblem G",
        "Emblem H",
        "Red-Hot Mirror",
        "Red-Hot Ball",
        "Red-Hot Stick",
        "Power Bracelet",
        "Shield Bracelet",
        "Super Bracelet",
        "Medical Herb",
        "Strange Bottle",
        "Brown Stone",
        "Green Stone",
        "Blue Stone",
        "Silver Stone",
        "Purple Stone",
        "Black Stone",
        "Magic Bell"
    };


    void GeneralTextUpdate(fstream &ROMFile) {

        unsigned char Byte;

        /* NPC actions to disable (mostly to remove NPC revival text) */
        for (int i=0; i<NB_NPC_TO_DISABLE_ADDRESSES; ++i) {
            ROMFile.seekp(NPCToDisableAddressList[i], ios::beg);
            TEXT_WriteByte(0x00);
        }

        /* Master's text when hero dies */
        ROMFile.seekp(0x786B, ios::beg);
        //ROMFile.write("Tough luck huh?", 15);
        TEXT_WriteString("Tough luck huh?");
        TEXT_EndText(TEXT_ENDTYPE_52FA);

        /* Master's text after Brown Stone */
        ROMFile.seekp(0x78BC, ios::beg);
        //ROMFile.write("One down,\rfive to go!", 21);
        TEXT_WriteString("One down,\rfive to go!");
        TEXT_EndText(TEXT_ENDTYPE_52FA);

        /* Master's first text */
        ROMFile.seekp(0x7999, ios::beg);
        //ROMFile.write("420 Soul blaze it!", 18);
        TEXT_WriteString("420 Soul blaze it!");
        TEXT_EndText(TEXT_ENDTYPE_52FA);
        ROMFile.seekp(0x7A07, ios::beg);
        TEXT_EndText(TEXT_ENDTYPE_52FA);

        /* PUSH START */
        ROMFile.seekp(0x13B2B, ios::beg);
        //ROMFile.write("RANDO HYPE", 10);
        TEXT_WriteString("RANDO HYPE");

        /* Bridge guard */
        ROMFile.seekp(0x18644, ios::beg);
        //ROMFile.write("Please pass.", 12);
        TEXT_WriteString("Please pass.");
        TEXT_EndText(TEXT_ENDTYPE_88B9);

        /* Water mill keeper */
        ROMFile.seekp(0x1877C, ios::beg);
        //ROMFile.write("Could you please\rturn this wheel?", 33);
        TEXT_WriteString("Could you please\rturn this wheel?");
        TEXT_EndText(TEXT_ENDTYPE_88B9);
        ROMFile.seekp(0x188B9, ios::beg);
        TEXT_EndText(TEXT_ENDTYPE_88B9);

        /* Village Chief */
        ROMFile.seekp(0x1A2C5, ios::beg);
        //ROMFile.write("Good job!", 9);
        TEXT_WriteString("Good job!");
        TEXT_EndText(TEXT_ENDTYPE_88B9);

        /* Magician text 2 */
        ROMFile.seekp(0x1A914, ios::beg);
        //ROMFile.write("Good luck and/or\rblame Everhate.", 32);
        TEXT_WriteString("Good luck and/or\rblame Everhate.");
        TEXT_EndText(TEXT_ENDTYPE_88B9);

        /* Woodstin Trio */
        ROMFile.seekp(0x1D1E6, ios::beg);
        TEXT_WriteString("Tadaaa!\rTry to guess where\rI go.");
        TEXT_EndText(TEXT_ENDTYPE_46EC);

        /* Leo's Lab entrance door */
        ROMFile.seekp(0x23A7B, ios::beg);
        //ROMFile.write("Welcome!", 8);
        TEXT_WriteString("Welcome!");
        TEXT_EndText(TEXT_ENDTYPE_88B9);/* A3BF */

        /* Mermaid statue (Southerta) */
        ROMFile.seekp(0xFA03C, ios::beg);
        //ROMFile.write("Southerta is open!", 18);
        TEXT_WriteString("Southerta is open!");
        TEXT_EndText(TEXT_ENDTYPE_88B9);/* 44AA */
    }


    void NPCItemTextUpdate(int ItemIndex, int ItemID, fstream &ROMFile) {

        unsigned int Byte;
        const char* ItemName;
        int NPCItemIndex = ItemIndex - NUMBER_OF_CHESTS;

        /* Get the item name */
        if (ItemID != GEMS_EXP) {
            ItemName = ItemNameList[ItemID].c_str();
        }
        else {
            ItemName = "EXP";
        }

        /* Update text when NPC gives the item */
        ROMFile.seekp(NPCItemTextAddressList[NPCItemIndex], ios::beg);
        if (ItemIndex == ITEM_WATER_SHRINE_TILE) {
            /* Particular cases where we need shorter text */
            TEXT_WriteByte(0x93); /* "There " */
            TEXT_WriteByte(0xBA); /* "is " */
            TEXT_WriteByte(0x97); /* "a " */
            TEXT_YellowStyle;
            TEXT_WriteString(ItemName);
            TEXT_EndStyle;
            TEXT_WriteString(".");
            TEXT_EndText(TEXT_ENDTYPE_46EC);
        }
        else {
            TEXT_HeroReceived;
            TEXT_YellowStyle;
            TEXT_WriteString(ItemName);
            TEXT_EndStyle;
            TEXT_WriteString(".");
            TEXT_EndText(TEXT_ENDTYPE_88B9);
        }

        /* Update text when NPC doesn't give its item because the hero already has it */
        if (NPCAlreadyHaveItemTextAddressList[NPCItemIndex] != 0) {

            ROMFile.seekp(NPCAlreadyHaveItemTextAddressList[NPCItemIndex], ios::beg);
            if (ItemIndex == ITEM_BIRD_RED_HOT_MIRROR ||
                ItemIndex == ITEM_SQUIRREL_PSYCHO_SWORD ||
                ItemIndex == ITEM_SQUIRREL_EMBLEM_C) {
                /* Particular cases where we need shorter text */
                TEXT_WriteByte(0x95); /* "You " */
                TEXT_WriteByte(0xB5); /* "have " */
                TEXT_WriteByte(0xC2); /* "my " */
                TEXT_WriteString("item.");
                //ROMFile.write("item.", 5);
            }
            else {
                TEXT_WriteByte(0x95); /* "You " */
                //ROMFile.write("already ", 8);
                TEXT_WriteString("already.");
                TEXT_WriteByte(0xB5); /* "have " */
                TEXT_WriteByte(0x97); /* "a " */
                TEXT_WriteByte(0x0D); /* Carriage return */
                TEXT_WriteString(ItemName);
                TEXT_WriteString(".");
            }
            TEXT_EndText(TEXT_ENDTYPE_88B9);
        }

        /* Magic Bell crystal fairy's text */
        if (ItemIndex == ITEM_CRYSTAL_MAGIC_BELL) {
            ROMFile.seekp(0x1C11E, ios::beg);
            TEXT_WriteString("If you bring me all 8\rMaster's Emblems,\rI will give you a");
            TEXT_YellowStyle;
            TEXT_WriteString(ItemName);
            TEXT_EndStyle;
            TEXT_WriteString(".");
            TEXT_EndText(TEXT_ENDTYPE_46EC);
        }

        /* Woodstin Trio's text */
        if (ItemIndex == ITEM_WOODSTIN_TRIO) {
            ROMFile.seekp(0x1D135, ios::beg);
            TEXT_WriteString("Let's start the show!\rThe prize is a\r");
            TEXT_YellowStyle;
            TEXT_WriteString(ItemName);
            TEXT_EndStyle;
            TEXT_WriteString(".");
            TEXT_EndText(TEXT_ENDTYPE_46EC);
        }

        /* Psycho Sword squirrel's revival text */
        if (ItemIndex == ITEM_SQUIRREL_PSYCHO_SWORD) {
            ROMFile.seekp(0x1E14D, ios::beg);
            //ROMFile.write("I will exchange this\r", 20);
            TEXT_WriteString("I will exchange this\r");
            TEXT_YellowStyle;
            //ROMFile.write(ItemName, strlen(ItemName));
            TEXT_WriteString(ItemName);
            TEXT_EndStyle;
            //ROMFile.write("\rfor food...", 12);
            TEXT_WriteString("\rfor food...");
            TEXT_EndText(TEXT_ENDTYPE_88B9);
        }


    }

}
