#include "Item.h"
#include "Map.h"
#include "ROMData.h"
#include "TextUpdate.h"

#include <iostream>
#include <fstream>
#include <string.h>

#define NB_NPC_TO_DISABLE_ADDRESSES 28

#define TEXT_ENDTYPE_52FA 0
#define TEXT_ENDTYPE_88B9 1
#define TEXT_ENDTYPE_46EC 2
#define TEXT_ENDTYPE_1EA5 3
#define TEXT_ENDTYPE_A3BF 4
#define TEXT_ENDTYPE_DFF0 5
#define TEXT_ENDTYPE_44AA 6
#define TEXT_ENDTYPE_C5EE 7



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
    case TEXT_ENDTYPE_1EA5:   \
        TEXT_WriteByte(0x1E); \
        TEXT_WriteByte(0xA5); \
    break;                    \
    case TEXT_ENDTYPE_A3BF:   \
        TEXT_WriteByte(0xA3); \
        TEXT_WriteByte(0xBF); \
    break;                    \
    case TEXT_ENDTYPE_DFF0:   \
        TEXT_WriteByte(0xDF); \
        TEXT_WriteByte(0xF0); \
    break;                    \
    case TEXT_ENDTYPE_44AA:   \
        TEXT_WriteByte(0x44); \
        TEXT_WriteByte(0xAA); \
    break;                    \
    case TEXT_ENDTYPE_C5EE:   \
        TEXT_WriteByte(0xC5); \
        TEXT_WriteByte(0xEE); \
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
        0x1CA7F, /* Sleeping bird */
        0x1D532, /* Greenwood's Guardian */
        0x1D869, /* Mole (helping soul) */
        0x22E7F, /* Great Door (helping soul) */
        0x22FF4, /* Leo's cat */
        0x23559, /* Marie */
        0x23FC5, /* Soldier next to basement entrance */
        0x24616, /* Singer */
        0x249D2, /* Queen Magridd */
        0x2521B, /* Soldier (helping soul) */
        0x255E5, /* King Magridd */
        0x25BAF, /* Soldier with Leo */
        0x25FE0, /* Dr. Leo (when the two soldiers are not present) */
        0x26033, /* Dr. Leo (when the two soldiers are present) */
        0xF8109, /* Angelfish (helping soul) */
        0xF87FA, /* Mermaid statue (Blester) */
        0xF8ACF, /* Mermaid statue (Rockbird) */
        0xF8EEA, /* Mermaid statue (Durean) */
        0xF966D, /* Mermaid statue (Ghost Ship) */
        0xF9247, /* Mermaid Queen */
        0xF9BF4  /* Lue */
    };

    static int NPCItemTextAddressList[59] = {
        0x183AE, /* Tool shop owner */
        0x188FD, /* Emblem A tile */
        0x18A2C, /* Goat pen corner */
        0x192DA, /* Tool shop owner's son Teddy */
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
        0x1E0DE, /* Psycho Sword squirrel */ // 46 EC
        0x1E1EF, /* Emblem C squirrel */ // 46 EC
        0x1E49F, /* Water Shrine Strange Bottle */ // 46 EC
        0x1E572, /* Light Arrow crystal */ // 46 EC
        0x1EBC3, /* Lost Marsh crystal */ // 46 EC
        0x1E6C0, /* Water Shrine crystal */ // 46 EC
        0x1E360, /* Fire Shrine crystal */ // 46 EC
        0x209D5, /* Mountain King */ // 1E A5
        0x20DDD, /* Mushroom Shoes boy */ // 1E A5
        0x210CD, /* Nome */ // 1E A5
        0x21A7A, /* Emblem E snail */ // 1E A5
        0x21EB0, /* Emblem F tile */ // 1E A5
        0x2249B, /* Mountain of Souls crystal */ // 1E A5
        0x21100, /* Lune crystal */ // 1E A5
        0x22ADD, /* Emblem G under chest of drawers */ // A3 BF
        0x22A7C, /* Chest of drawers (Mystic Armor) */ // A3 BF
        0x22BE3, /* Herb Plant in Leo's Lab */ // A3 BF
        0x2306D, /* Leo's Cat (door key) */ // A3 BF
        0x231AE, /* Actinidia plant */ // A3 BF
        0x235AD, /* Marie */ // A3 BF
        0x23922, /* Spark Bomb mouse */ // A3 BF
        0x23F34, /* Leo's Lab Basement crystal */ // A3 BF
        0x23BC0, /* Model Town 1 crystal */ // A3 BF
        0x23C00, /* Power Plant crystal */ // A3 BF
        0x24317, /* Elemental Mail soldier */ // DF F0
        0x24AB7, /* Super Bracelet tile */ // DF F0
        0x24A47, /* Queen Magridd (VIP card) */ // DF F0
        0x24C80, /* Platinum Card soldier */ // DF F0
        0x24EBA, /* Maid (Herb) */ // DF F0
        0x253C4, /* Emblem H tile */ // DF F0
        0x2563A, /* Magridd King */ // DF F0
        0x264C4, /* Leo on the Airship deck (Mobile key) */ // DF F0
        0x26A17, /* Harp String tile */ // DF F0
        0xF831D, /* North-eastern Mermaid (Herb) */ // 44 AA
        0xF8BF8, /* Bubble Armor Mermaid */ // 44 AA
        0xF909A, /* Magic Flair Mermaid */ // 44 AA
        0xF9253, /* Mermaid Queen */ // 44 AA
        0xF9874, /* Red-Hot Stick Mermaid */ // 44 AA
        0xF9C13, /* Lue */ // 44 AA
        0xFA9C6, /* Rockbird crystal */ // 44 AA
        0xF9C40, /* Seabed crystal near Blester */ // 44 AA
        0xFA060  /* Seabed crystal near Durean */ // 44 AA
    };

    static int NPCAlreadyHaveItemTextAddressList[59] = {
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
        0, /* Water Shrine Strange Bottle */
        0, /* Light Arrow crystal */
        0, /* Lost Marsh crystal */
        0, /* Water Shrine crystal */
        0, /* Fire Shrine crystal */
        0, /* Mountain King */
        0x20E73, /* Mushroom Shoes boy */
        0, /* Nome */
        0x21AB9, /* Emblem E snail */
        0, /* Emblem F tile */
        0, /* Mountain of Souls crystal */
        0, /* Lune crystal */
        0, /* Emblem G under chest of drawers */
        0x22AB9, /* Chest of drawers (Mystic Armor) */
        0x22C22, /* Herb Plant in Leo's Lab */
        0, /* Leo's Cat (door key) */
        0x23201, /* Actinidia plant */
        0, /* Marie */
        0x23977, /* Spark Bomb mouse */
        0, /* Leo's Lab Basement crystal */
        0, /* Model Town 1 crystal */
        0, /* Power Plant crystal */
        0, /* Elemental Mail soldier */
        0, /* Super Bracelet tile */
        0x24A77, /* Queen Magridd (VIP card) */
        0, /* Platinum Card soldier */
        0x24F0F, /* Maid (Herb) */
        0, /* Emblem H tile */
        0, /* Magridd King */
        0, /* Leo on the Airship deck (Mobile key) */
        0, /* Harp String tile */
        0xF836D, /* North-eastern Mermaid (Herb) */
        0, /* Bubble Armor Mermaid */
        0xF90B6, /* Magic Flair Mermaid */
        0, /* Mermaid Queen */
        0, /* Red-Hot Stick Mermaid */
        0xF9D87, /* Lue */
        0, /* Rockbird crystal */
        0, /* Seabed crystal near Blester */
        0  /* Seabed crystal near Durean */
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
        "Goat`s Food",
        "Harp String",
        "Pass",
        "Dream Rod",
        "Leo`s Brush",
        "Greenwood Leaf",
        "Mole`s Ribbon",
        "Big Pearl",
        "Mermaid`s Tears",
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


    static int PickEndTextCode(int NPCItemIndex) {
        int EndTextCode = TEXT_ENDTYPE_44AA;
        if (NPCItemIndex <= 10) {
            EndTextCode = TEXT_ENDTYPE_88B9;
        }
        else if (NPCItemIndex <= 23) {
            EndTextCode = TEXT_ENDTYPE_46EC;
        }
        else if (NPCItemIndex <= 30) {
            EndTextCode = TEXT_ENDTYPE_1EA5;
        }
        else if (NPCItemIndex <= 40) {
            EndTextCode = TEXT_ENDTYPE_A3BF;
        }
        else if (NPCItemIndex <= 49) {
            EndTextCode = TEXT_ENDTYPE_DFF0;
        }
        return EndTextCode;
    }


    void GeneralTextUpdate(fstream &ROMFile, long Seed) {

        unsigned char Byte;

        /* NPC actions to disable (mostly to remove NPC revival text) */
        for (int i=0; i<NB_NPC_TO_DISABLE_ADDRESSES; ++i) {
            ROMFile.seekp(NPCToDisableAddressList[i], ios::beg);

            /* For leader NPCs, keep the 02 37 code to heal the hero */
            if (i == 5  || /* Village Chief */
                i == 8  || /* Greenwood's Guardian */
                i == 12 || /* Marie */
                i == 17 || /* King Magridd */
                i == 26) { /* Mermaid Queen */
                TEXT_WriteByte(0x37); /* Heal the hero completely */
                if (i == 26) {
                    /* You have to restore this for Mermaid Queen or her revival text glitches out.
                       I have no idea what it does... */
                    TEXT_WriteByte(0x9C);
                    TEXT_WriteByte(0xFB);
                    TEXT_WriteByte(0x03);
                }
                TEXT_WriteByte(0x02);
            }
            TEXT_WriteByte(0x86);
            TEXT_WriteByte(0x6B); /* End code byte */
        }

        /* Master's text when hero dies */
        ROMFile.seekp(0x786B, ios::beg);
        TEXT_WriteString("Tough luck huh?");
        TEXT_EndText(TEXT_ENDTYPE_52FA);

        /* Master's text after Brown Stone */
        ROMFile.seekp(0x78BC, ios::beg);
        TEXT_WriteString("One down,\rfive to go!");
        TEXT_EndText(TEXT_ENDTYPE_52FA);

        /* Master's first text */
        ROMFile.seekp(0x7999, ios::beg);
        TEXT_WriteString("420 Soul blaze it!");
        TEXT_EndText(TEXT_ENDTYPE_52FA);
        ROMFile.seekp(0x7A07, ios::beg);
        TEXT_EndText(TEXT_ENDTYPE_52FA);

        /* Title + file selection screens */
        ROMFile.seekp(0x13B2B, ios::beg);
        TEXT_WriteString("RANDO HYPE");
        ROMFile.seekp(0x13B3C, ios::beg);
        TEXT_WriteString("RandoBlazer v0.4   ");
        ROMFile.seekp(0x143B9, ios::beg);
        TEXT_WriteString("Seed ");
        char SeedChr[11] = {'\0'};
        sprintf(SeedChr, "%10lu", Seed);
        TEXT_WriteString(SeedChr);

        /* Greenwood + Actinidia leaves */
        ROMFile.seekp(0x1514C, ios::beg);
        TEXT_WriteString("G.Leaf");
        ROMFile.seekp(0x151B2, ios::beg);
        TEXT_WriteString("A.Leaf");

        /* Bridge guard */
        ROMFile.seekp(0x18644, ios::beg);
        TEXT_WriteString("Please pass.");
        TEXT_EndText(TEXT_ENDTYPE_88B9);

        /* Water mill keeper */
        ROMFile.seekp(0x1877C, ios::beg);
        TEXT_WriteString("Could you please\rturn this wheel?");
        TEXT_EndText(TEXT_ENDTYPE_88B9);
        ROMFile.seekp(0x188B9, ios::beg);
        TEXT_EndText(TEXT_ENDTYPE_88B9);

        /* Sleeping tulip (move this text to make room for the Pass tile text) */
        ROMFile.seekp(0x1984E, ios::beg);
        TEXT_WriteByte(0x9A); /* Change text pointer */
        ROMFile.seekp(0x1989A, ios::beg);
        TEXT_WriteByte(0x10);
        TEXT_WriteString("Hello...");

        /* Village Chief */
        ROMFile.seekp(0x1A2C5, ios::beg);
        TEXT_WriteString("Good job!");
        TEXT_EndText(TEXT_ENDTYPE_88B9);

        /* Magician text 2 */
        ROMFile.seekp(0x1A914, ios::beg);
        TEXT_WriteString("Good luck and/or\rblame Everhate.");
        TEXT_EndText(TEXT_ENDTYPE_88B9);

        /* Revival of first Underground Castle Crystal Fairy */
        ROMFile.seekp(0x1AC5B, ios::beg);
        TEXT_WriteByte(0x00);

        /* Woodstin Trio */
        ROMFile.seekp(0x1D1E6, ios::beg);
        TEXT_WriteString("Tadaaa!\rTry to guess where\rI go.");
        TEXT_EndText(TEXT_ENDTYPE_46EC);

        /* Turbo's text in sleeping bird's dream */
        ROMFile.seekp(0x1E344, ios::beg);
        TEXT_WriteString("Peace, guys.");
        TEXT_EndText(TEXT_ENDTYPE_46EC);

        /* Lost Marsh raft */
        ROMFile.seekp(0x1E68E, ios::beg);
        TEXT_WriteString("You`re missing the\rGreenwood Leaves!");
        TEXT_EndText(TEXT_ENDTYPE_46EC);
        ROMFile.seekp(0x1E713, ios::beg);
        TEXT_WriteString("Get on!");
        TEXT_EndText(TEXT_ENDTYPE_46EC);

        /* Act 2 crystal fairies: change text pointers */
        ROMFile.seekp(0x1E533, ios::beg); /* Water Shrine crystal */
        TEXT_WriteByte(0xC0);
        TEXT_WriteByte(0xE6);
        ROMFile.seekp(0x1E5B1, ios::beg); /* Fire Shrine crystal */
        TEXT_WriteByte(0x60);
        TEXT_WriteByte(0xE3);

        /* Master's text after World of Evil is opened */
        ROMFile.seekp(0x1ED47, ios::beg);
        TEXT_WriteString("Ready for the finale?");
        TEXT_EndText(TEXT_ENDTYPE_C5EE);

        /* Mountain King */
        ROMFile.seekp(0x20740, ios::beg);
        TEXT_WriteString("Did you find the\rthree Red-Hot items?");
        TEXT_EndText(TEXT_ENDTYPE_1EA5);
        ROMFile.seekp(0x20905, ios::beg);
        TEXT_WriteString("- DANCING GRANDMAS! -");
        TEXT_EndText(TEXT_ENDTYPE_1EA5);
        ROMFile.seekp(0x2092B, ios::beg);
        TEXT_WriteString("Final boss time!");
        TEXT_EndText(TEXT_ENDTYPE_1EA5);
        ROMFile.seekp(0x209F3, ios::beg);
        TEXT_WriteString("Good luck!");
        TEXT_EndText(TEXT_ENDTYPE_1EA5);

        /* Act 4 crystal fairies: change text pointers */
        ROMFile.seekp(0x21F45, ios::beg); /* Lune crystal */
        TEXT_WriteByte(0x00);
        TEXT_WriteByte(0x91);

        /* Leo's Lab entrance door */
        ROMFile.seekp(0x23A7B, ios::beg);
        TEXT_WriteString("Welcome!");
        TEXT_EndText(TEXT_ENDTYPE_A3BF);

        /* Leo's cat's dream */
        ROMFile.seekp(0x23B89, ios::beg);
        TEXT_WriteString("We can`t let him make\ra deal with Deathtoll!");
        TEXT_EndText(TEXT_ENDTYPE_A3BF);
        ROMFile.seekp(0x23C90, ios::beg);
        TEXT_WriteString("Do it for Lisa.");
        TEXT_EndText(TEXT_ENDTYPE_A3BF);
        ROMFile.seekp(0x23D14, ios::beg);
        TEXT_WriteString("Oh Master, please\rforgive me.");
        TEXT_EndText(TEXT_ENDTYPE_A3BF);

        /* Act 5 crystal fairies: change text pointers */
        ROMFile.seekp(0x23E4A, ios::beg); /* Model Town 1 crystal */
        TEXT_WriteByte(0xC0);
        TEXT_WriteByte(0xBB);
        ROMFile.seekp(0x23E7A, ios::beg); /* Power Plant crystal */
        TEXT_WriteByte(0x00);
        TEXT_WriteByte(0xBC);

        /* Singer's text */
        ROMFile.seekp(0x24677, ios::beg);
        TEXT_WriteString("Let`s jam it!");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);

        /* Dr. Leo/Queen Magridd cutscene */
        ROMFile.seekp(0x26145, ios::beg);
        TEXT_WriteString("We have to defeat\rDeathtoll!");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x25C7F, ios::beg);
        TEXT_WriteString("We`re ready to roll!");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x25D4E, ios::beg);
        TEXT_WriteString("Queen Magridd!");
        TEXT_WriteByte(0x11); /* Input prompt */
        TEXT_WriteByte(0x10); /* New textbox */
        TEXT_WriteString("Muahahahaaa!!!\rKill this loser.");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x26241, ios::beg);
        TEXT_WriteByte(0x95); /* "You " */
        TEXT_WriteByte(0x98); /* "are " */
        TEXT_WriteString("worse than\revil!");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x2626E, ios::beg);
        TEXT_WriteString("Wait, there`s more!");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x262BA, ios::beg);
        TEXT_WriteString("Daddy!");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x262EE, ios::beg);
        TEXT_WriteString("Dr. Leo, will you\rcooperate now?");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x26383, ios::beg);
        TEXT_WriteString("All right. But please\rlet her go.");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x26459, ios::beg);
        TEXT_WriteString("Deal.");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x264EC, ios::beg);
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x2650A, ios::beg);
        TEXT_WriteString("This is it.");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x26773, ios::beg);
        TEXT_WriteString("Thanks for everything\ryou are doing for us.\rAre you a real human\rbeing like me?");
        TEXT_WriteByte(0x0C); /* Question prompt */
        ROMFile.seekp(0x26874, ios::beg);
        TEXT_WriteString("I see.........\rPlease leave while my\rback is turned.");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);

        /* Sleeping Soldier's dream */
        ROMFile.seekp(0x26AA7, ios::beg);
        TEXT_WriteString("My inventions should\rmake people happy,\rbut they are being\rused for evil.");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x26C9E, ios::beg);
        TEXT_WriteByte(0x91); /* "The " */
        TEXT_WriteByte(0xF6); /* "world " */
        TEXT_WriteByte(0xF1); /* "will " */
        TEXT_WriteByte(0xA1); /* "be " */
        TEXT_WriteString("\rdestroyed!");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x26CBF, ios::beg);
        TEXT_WriteString("I am sorry but\rI have orders.");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);

        /* Herb Mermaid of St. Elles */
        ROMFile.seekp(0xF8356, ios::beg);
        TEXT_WriteString("Here you go!");
        TEXT_EndText(TEXT_ENDTYPE_44AA);

        /* Mermaid statues */
        ROMFile.seekp(0xF8801, ios::beg);
        TEXT_WriteString("Blester is open!");
        TEXT_EndText(TEXT_ENDTYPE_44AA);
        ROMFile.seekp(0xF8AD6, ios::beg);
        TEXT_WriteString("Rockbird is open!");
        TEXT_EndText(TEXT_ENDTYPE_44AA);
        ROMFile.seekp(0xF8EF1, ios::beg);
        TEXT_WriteString("Durean is open!");
        TEXT_EndText(TEXT_ENDTYPE_44AA);
        ROMFile.seekp(0xF9674, ios::beg);
        TEXT_WriteString("Ghost Ship is open!");
        TEXT_EndText(TEXT_ENDTYPE_44AA);
        ROMFile.seekp(0xFA040, ios::beg);
        TEXT_WriteString("Southerta is open!");
        TEXT_EndText(TEXT_ENDTYPE_44AA);

        /* Act 3 crystal fairies: change text pointers */
        ROMFile.seekp(0xFA4B7, ios::beg); /* Seabed crystal near Blester */
        TEXT_WriteByte(0x40);
        TEXT_WriteByte(0x9C);
        ROMFile.seekp(0xFA4E7, ios::beg); /* Seabed crystal near Durean */
        TEXT_WriteByte(0x60);
        TEXT_WriteByte(0xA0);
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
        if (ItemIndex == ITEM_WATER_SHRINE_TILE ||
            ItemIndex == ITEM_EMBLEM_H) {
            /* Particular cases where we need shorter text */
            TEXT_WriteByte(0x93); /* "There " */
            TEXT_WriteByte(0xBA); /* "is " */
            TEXT_WriteByte(0x97); /* "a " */
            TEXT_WriteByte(0x0D); /* Carriage return */
            TEXT_YellowStyle;
            TEXT_WriteString(ItemName);
            TEXT_EndStyle;
            TEXT_WriteString(".");
            TEXT_EndText(PickEndTextCode(NPCItemIndex));
        }
//        else if (ItemIndex == ITEM_CRYSTAL_LOST_MARSH ||
//                 ItemIndex == ITEM_CRYSTAL_WATER_SHRINE ||
//                 ItemIndex == ITEM_CRYSTAL_FIRE_SHRINE ||
//                 ItemIndex == ITEM_CRYSTAL_MOUNTAIN_OF_SOULS ||
//                 ItemIndex == ITEM_CRYSTAL_LUNE ||
//                 ItemIndex == ITEM_CRYSTAL_LEOS_LAB_BASEMENT ||
//                 ItemIndex == ITEM_CRYSTAL_MODEL_TOWN ||
//                 ItemIndex == ITEM_CRYSTAL_POWER_PLANT ||
//                 ItemIndex == ITEM_CRYSTAL_ROCKBIRD ||
//                 ItemIndex == ITEM_CRYSTAL_SEABED_NEAR_BLESTER ||
//                 ItemIndex == ITEM_CRYSTAL_SEABED_NEAR_DUREAN) {
//            /* Crystal fairies with shared text */
//            TEXT_HeroReceived;
//            TEXT_WriteString("something...");
//            TEXT_WriteByte(0x11);
//            TEXT_WriteByte(0x0C); /* Question prompt */
//        }
        else {
            /* Normal case */
            if (ItemIndex == ITEM_CRYSTAL_WATER_SHRINE ||
                ItemIndex == ITEM_CRYSTAL_FIRE_SHRINE ||
                ItemIndex == ITEM_CRYSTAL_LUNE ||
                ItemIndex == ITEM_CRYSTAL_MODEL_TOWN ||
                ItemIndex == ITEM_CRYSTAL_POWER_PLANT ||
                ItemIndex == ITEM_CRYSTAL_SEABED_NEAR_BLESTER ||
                ItemIndex == ITEM_CRYSTAL_SEABED_NEAR_DUREAN ||
                ItemIndex == ITEM_SOLDIER_PLATINUM_CARD) {
                /* These texts have been moved from their original location */
                TEXT_WriteByte(0x10);
            }
            TEXT_HeroReceived;
            TEXT_YellowStyle;
            TEXT_WriteString(ItemName);
            TEXT_EndStyle;
            TEXT_WriteString(".");
            TEXT_EndText(PickEndTextCode(NPCItemIndex));
        }

        /* Update text when NPC doesn't give its item because the hero already has it */
        if (NPCAlreadyHaveItemTextAddressList[NPCItemIndex] != 0) {

            ROMFile.seekp(NPCAlreadyHaveItemTextAddressList[NPCItemIndex], ios::beg);
            if (ItemIndex == ITEM_BIRD_RED_HOT_MIRROR ||
                ItemIndex == ITEM_SQUIRREL_PSYCHO_SWORD ||
                ItemIndex == ITEM_SQUIRREL_EMBLEM_C ||
                ItemIndex == ITEM_SNAIL_EMBLEM_E ||
                ItemIndex == ITEM_PLANT_ACTINIDIA_LEAVES ||
                ItemIndex == ITEM_MAID_HERB) {
                /* Particular cases where we need shorter text */
                TEXT_WriteByte(0x95); /* "You " */
                TEXT_WriteByte(0xB5); /* "have " */
                TEXT_WriteByte(0xC2); /* "my " */
                TEXT_WriteString("item.");
            }
            else {
                TEXT_WriteByte(0x95); /* "You " */
                TEXT_WriteString("already ");
                TEXT_WriteByte(0xB5); /* "have " */
                TEXT_WriteByte(0x97); /* "a " */
                TEXT_WriteByte(0x0D); /* Carriage return */
                TEXT_WriteString(ItemName);
                TEXT_WriteString(".");
            }
            TEXT_EndText(PickEndTextCode(NPCItemIndex));
        }

        /* Tool shop owner's son Teddy */
        if (ItemIndex == ITEM_TEDDY) {
            ROMFile.seekp(0x19256, ios::beg);
            TEXT_WriteString("Fancy ");
            TEXT_WriteByte(0x97); /* "a " */
            TEXT_WriteByte(0x0D); /* Carriage return */
            TEXT_YellowStyle;
            TEXT_WriteString(ItemName);
            TEXT_EndStyle;
            TEXT_WriteString("\rfor a billion dollars?");
            TEXT_WriteByte(0x0C); /* Question prompt */
        }

        /* Magic Bell crystal fairy's text */
        if (ItemIndex == ITEM_CRYSTAL_MAGIC_BELL) {
            ROMFile.seekp(0x1C11E, ios::beg);
            TEXT_WriteString("If you bring me all 8\rMaster`s Emblems,\rI will give you a\r");
            TEXT_YellowStyle;
            TEXT_WriteString(ItemName);
            TEXT_EndStyle;
            TEXT_WriteString(".");
            TEXT_EndText(TEXT_ENDTYPE_46EC);
        }

        /* Woodstin Trio's text */
        if (ItemIndex == ITEM_WOODSTIN_TRIO) {
            ROMFile.seekp(0x1D135, ios::beg);
            TEXT_WriteString("Let`s start the show!\rThe prize is a\r");
            TEXT_YellowStyle;
            TEXT_WriteString(ItemName);
            TEXT_EndStyle;
            TEXT_WriteString(".");
            TEXT_EndText(TEXT_ENDTYPE_46EC);
        }

        /* Psycho Sword squirrel's revival text */
        if (ItemIndex == ITEM_SQUIRREL_PSYCHO_SWORD) {
            ROMFile.seekp(0x1E14D, ios::beg);
            TEXT_WriteString("I will exchange this\r");
            TEXT_YellowStyle;
            TEXT_WriteString(ItemName);
            TEXT_EndStyle;
            TEXT_WriteString("\rfor food...");
            TEXT_EndText(TEXT_ENDTYPE_88B9);
        }

        /* Mushroom Shoes boy's revival text */
        if (ItemIndex == ITEM_BOY_MUSHROOM_SHOES) {
            ROMFile.seekp(0x20D74, ios::beg);
            TEXT_WriteString("I have a nice\r");
            TEXT_YellowStyle;
            TEXT_WriteString(ItemName);
            TEXT_EndStyle;
            TEXT_WriteString("\rfor you!");
            TEXT_EndText(TEXT_ENDTYPE_1EA5);
        }

        /* Leo's Lab herb plant's first question */
        if (ItemIndex == ITEM_PLANT_HERB) {
            ROMFile.seekp(0x22BC9, ios::beg);
            TEXT_WriteString("Fancy ");
            TEXT_WriteByte(0x97); /* "a " */
            TEXT_WriteByte(0x0D); /* Carriage return */
            TEXT_WriteString(ItemName);
            TEXT_WriteString("? ");
            TEXT_WriteByte(0x0C); /* Question prompt */
        }

        /* Soldier text: tells what item the sleeping soldier has */
        if (ItemIndex == ITEM_SOLDIER_ELEMENTAL_MAIL) {
            ROMFile.seekp(0x24119, ios::beg);
            TEXT_WriteString("I know a sleeping\rsoldier who has a\r");
            TEXT_YellowStyle;
            TEXT_WriteString(ItemName);
            TEXT_EndStyle;
            TEXT_WriteString("...");
            TEXT_EndText(TEXT_ENDTYPE_DFF0);
        }

        /* Platinum Card soldier text */
        if (ItemIndex == ITEM_SOLDIER_PLATINUM_CARD) {

            /* Move the item text location */
            ROMFile.seekp(0x24C3E, ios::beg);
            TEXT_WriteByte(0x80);
            TEXT_WriteByte(0xCC);

            /* Change Soldier's text */
            ROMFile.seekp(0x24C49, ios::beg);
            TEXT_WriteByte(0x93); /* "There " */
            TEXT_WriteByte(0xBA); /* "is " */
            TEXT_WriteByte(0x97); /* "a " */
            TEXT_WriteByte(0x0D); /* Carriage return */
            TEXT_YellowStyle;
            TEXT_WriteString(ItemName);
            TEXT_EndStyle;
            TEXT_WriteString(" under\rthe guard`s feet.");
            TEXT_EndText(TEXT_ENDTYPE_DFF0);
        }

        /* Magridd Castle herb maid's first question */
        if (ItemIndex == ITEM_MAID_HERB) {
            ROMFile.seekp(0x24E9E, ios::beg);
            TEXT_WriteString("Would ");
            TEXT_WriteByte(0xFE); /* "you " */
            TEXT_WriteByte(0xBE); /* "like " */
            TEXT_WriteByte(0x97); /* "a " */
            TEXT_WriteByte(0x0D); /* Carriage return */
            TEXT_WriteString(ItemName);
            TEXT_WriteString("? ");
            TEXT_WriteByte(0x0C); /* Question prompt */
        }

        /* St. Elles herb mermaid's first question */
        if (ItemIndex == ITEM_MERMAID_HERB) {
            ROMFile.seekp(0xF831D, ios::beg);
            TEXT_WriteString("Would ");
            TEXT_WriteByte(0xFE); /* "you " */
            TEXT_WriteByte(0xBE); /* "like " */
            TEXT_WriteByte(0x97); /* "a " */
            TEXT_WriteByte(0x0D); /* Carriage return */
            TEXT_WriteString(ItemName);
            TEXT_WriteString("? ");
            TEXT_WriteByte(0x0C); /* Question prompt */
        }

        /* Bubble Armor mermaid's revival text */
        if (ItemIndex == ITEM_MERMAID_BUBBLE_ARMOR) {
            ROMFile.seekp(0xF8BBD, ios::beg);
            TEXT_WriteString("Does anyone want my\r");
            TEXT_YellowStyle;
            TEXT_WriteString(ItemName);
            TEXT_EndStyle;
            TEXT_WriteString("?");
            TEXT_EndText(TEXT_ENDTYPE_44AA);
        }
    }

}
