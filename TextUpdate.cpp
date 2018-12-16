#include "Item.h"
#include "Map.h"
#include "Random.h"
#include "ROMData.h"
#include "TextUpdate.h"

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>

#define NB_NPC_TO_DISABLE_ADDRESSES 27
#define NB_MASTER_INTRO_TEXTS 26
#define NB_MASTER_DEATH_TEXTS 13
#define NB_DEATHTOLL_1_TEXTS  10
#define NB_DEATHTOLL_2_TEXTS  10
#define NB_VICTORY_TEXTS      18

#define TEXT_ENDTYPE_52FA 0
#define TEXT_ENDTYPE_88B9 1
#define TEXT_ENDTYPE_46EC 2
#define TEXT_ENDTYPE_1EA5 3
#define TEXT_ENDTYPE_A3BF 4
#define TEXT_ENDTYPE_DFF0 5
#define TEXT_ENDTYPE_44AA 6
#define TEXT_ENDTYPE_C5EE 7
#define TEXT_ENDTYPE_12   8



/***** Macros to write into ROM *****/

#define TEXT_WriteByte(_Byte_)        \
{                                     \
    Byte = _Byte_;                    \
    ROMFile.write((char*)(&Byte), 1); \
}

#define TEXT_WriteString(_String_) {ROMFile.write(_String_, strlen(_String_));}

#define TEXT_WriteItemString(_ItemID_) {                                              \
    if (RandomizedItemList[_ItemID_].Contents == ItemID::GEMS_EXP) {TEXT_WriteString("EXP");} \
    else {TEXT_WriteString(ItemNameList[(size_t)RandomizedItemList[_ItemID_].Contents]);}     \
}

#define TEXT_WriteItemByte(_ItemID_) {                                        \
    ItemID Byte = RandomizedItemList[_ItemID_].Contents;                             \
    if (Byte != ItemID::GEMS_EXP && Byte != ItemID::NOTHING) {ROMFile.write((char*)(&Byte), 1);} \
}


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

#define TEXT_EndText12    \
{                         \
    TEXT_WriteByte(0x12); \
    TEXT_WriteByte(0x08); \
    TEXT_WriteByte(0x08); \
    TEXT_WriteByte(0x04); \
    TEXT_WriteByte(0x0C); \
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
    TEXT_WriteByte(0x03); \
    TEXT_WriteByte(0x20); \
}                         \



namespace ROMUpdate {

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
        0x23FCA, /* Soldier next to basement entrance */
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
        0xF9BF4  /* Lue */
    };

    static int NPCItemTextAddressList[60] = {
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
        0x1E360, /* Lost Marsh crystal */ // 46 EC
        0x1E6C0, /* Water Shrine crystal */ // 46 EC
        0x1EBC3, /* Fire Shrine crystal */ // 46 EC
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
        0x23404, /* Chest of drawers (Herb) */ // A3 BF
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
        0,       /* North-eastern Mermaid (Herb) */ // 44 AA
        0xF8BF8, /* Bubble Armor Mermaid */ // 44 AA
        0xF909A, /* Magic Flair Mermaid */ // 44 AA
        0xF9280, /* Mermaid Queen */ // 44 AA
        0xF9874, /* Red-Hot Stick Mermaid */ // 44 AA
        0xF9C13, /* Lue */ // 44 AA
        0xFA9C6, /* Rockbird crystal */ // 44 AA
        0xF9C40, /* Seabed crystal near Blester */ // 44 AA
        0xFA060  /* Seabed crystal near Durean */ // 44 AA
    };

    static int NPCAlreadyHaveItemTextAddressList[60] = {
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
        0x2342F,  /* Chest of drawers (Herb) */
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
        0xF98C7, /* Red-Hot Stick Mermaid */
        0xF9D87, /* Lue */
        0, /* Rockbird crystal */
        0, /* Seabed crystal near Blester */
        0  /* Seabed crystal near Durean */
    };

    static int NPCItemAddressList[60] = {
        0x183AB, /* Tool shop owner */
        0x1875E, /* Emblem A tile */
        0x18960, /* Goat pen corner */
        0x19253, /* Tool shop owner's son Teddy */
        0x1983B, /* A Pass */
        0x19914, /* Tile in child's secret cave */
        0x1A12D, /* Village Chief - address modified */
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
        0x23311, /* Chest of drawers (Herb) */
        0x2354D, /* Marie */
        0x238F6, /* Spark Bomb mouse */
        0x23DFA, /* Leo's Lab Basement crystal */
        0x23E4E, /* Model Town 1 crystal */
        0x23E7E, /* Power Plant crystal */
        0x242A3, /* Elemental Mail soldier */
        0x2499B, /* Super Bracelet tile */
        0x249C2, /* Queen Magridd (VIP card) */
        0x24C3B, /* Platinum Card soldier - address modified */
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

    static const char* ItemNameList[65] = {
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

    static const char* MasterIntroTextList[NB_MASTER_INTRO_TEXTS] = {
        "420 Soul blaze it!",
        "I`m calling\rSoul Blade in its\rvanilla location.",
        "Good luck!\rYou`ll need it!",
        "Are we on\rSpeedGaming yet?",
        "This run is\rZantetsu-less until\rZantetsu.",
        "It`s show time!",
        "Dancing grandmas\r    HYPE!!!!    ",
        "Thank you so much\rfor playing my game!",
        "Extra credit if\ryou get sub-2h!",
        "I`m calling pedestal\rseed on this one.",
        "Fun fact: there are\rexactly 400 monster\rlairs in this game.",
        "GLHF!",
        "Look mom,\rI`m on stream!!!",
        "Go and bring back\rthe peace!\r.....Or whatever this\rgame is about.",
        "This is totally\rnot a trolly seed.",
        "This seed is\rarmor-less until\rWorld of Evil.",
        "I can`t wait to\rsee the forced\rmagic-less Laynole.",
        "Break a leg!",
        "Initializing\rvideogame.....\r     .....complete!",
        "Super Nintendo is\rthe best console.\rFite me.",
        "So you think you\rhave what it takes?",
        "Let`s go!\rYou can do it!",
        "My PB on this\rseed is 35:37.\rBlindfolded.",
        "Help me,\rObi-Wan Kenobi.\rYou`re my only hope.",
        "Phoenix in hype cave.\rI`m calling it.",
        "Any resemblance with\rActRaiser is purely\rcoincidental."
    };

    static const char* MasterDeathTextList[NB_MASTER_DEATH_TEXTS] = {
        "Ouch! Tough luck :/",
        "Better luck next time!",
        "Don`t be patient.\rThis is a speedrun\rafter all.",
        "Cheer up!\rYou can do it!",
        "Trolly seed, huh?",
        "Git gud n00b\rLOL",
        "Don`t push yourself\rtoo hard.",
        "That death was\rRNG manipulation.",
        "I`m sorry for\rmaking this seed...",
        "Be more careful\rnext time!",
        "I`m sure this\rwas a deathwarp!\r       .....Right?",
        "Come on! Let`s go!",
        "Don`t give up.\rYou got this!"
    };

    static const char* Deathtoll1TextList[NB_DEATHTOLL_1_TEXTS] = {
        "Peekaboo!",
        "Guess who!",
        "Your adventure\rends here.",
        "Time for an\repic battle!",
        "In case you haven`t\rguessed, I`m the\rfinal boss.",
        "How did you make it\rhere? This seed was\rsupposed to be\rsuper trolly!",
        "Surpriiiiise!",
        "Incoming the game`s\rhardest fight.",
        "So we meet again,\rMr. Bond.",
        "I`ll put an end\rto your misery."
    };

    static const char* Deathtoll2TextList[NB_DEATHTOLL_2_TEXTS] = {
        "This is not even\rmy final form!",
        "All right.\rNow this is\rserious business.",
        "I hope you didn`t\rforget Phoenix!",
        "Okay, time for an\ractually challenging\rbattle.",
        "Time to die!",
        "You didn`t think\rit was that easy,\rdid you?",
        "Loading Deathtoll.exe\r   .......Complete!",
        "Don`t tell me\ryou also got the\rSuper Bracelet!?",
        "And now behold...\rmy true power!",
        "Dang, I hope my next\rphase is better..."
    };

    static const char* VictoryTextList[NB_VICTORY_TEXTS] = {
        "\r         G  G",
        "Thank you Mario.\rBut our princess\ris in another castle!",
        "Congratulations!",
        "Woohoo!!\rYou made it!!!",
        "Thanks a lot for\rplaying this\rrandomizer.",
        "Hope you enjoyed\rthis seed!",
        "A winner is you!",
        "Dang, I really\rthought this seed was\rtoo trolly for you.",
        "  ...and this is\rthe end of our story.",
        "The last Metroid\ris in captivity.\rThe galaxy is\rat peace.",
        "Well done!\rNow try Hard mode.\r\r   ...just kidding!",
        "I`m sorry for\rthis seed...",
        "Hi YouTube!",
        "This is the end.\rMy only friend,\rthe end.",
        "You are a true hero!",
        "Hyrule is saved!\r\r ...Wait, wrong game.",
        "That`s all folks!",
        "Well done!\rBut the next seed\rwon`t be that easy!"
    };

    static const char* ItemLocations[NUMBER_OF_ITEMS] =
        {"Trial Room",
         "Grass Valley\rsecret cave",
         "Grass Valley\rsecret cave",
         "Underground Castle",
         "Underground Castle",
         "Underground Castle",
         "Underground Castle",
         "Leo`s Paintings",
         "Leo`s Paintings",
         "Greenwood",
         "Greenwood tunnels",
         "Water Shrine",
         "Water Shrine",
         "Water Shrine",
         "Water Shrine",
         "Water Shrine",
         "Fire Shrine",
         "Fire Shrine",
         "Fire Shrine",
         "Fire Shrine",
         "Fire Shrine",
         "Light Shrine",
         "St. Elles",
         "St. Elles",
         "Seabed secret cave",
         "Seabed secret cave",
         "Seabed secret cave",
         "Seabed secret cave",
         "Southerta",
         "Rockbird",
         "Rockbird",
         "Durean",
         "Durean",
         "Ghost Ship",
         "Northern Seabed",
         "Mountain of Souls",
         "Mountain of Souls",
         "Mountain of Souls",
         "Mountain of Souls",
         "Mountain of Souls",
         "Laynole",
         "Laynole",
         "Laynole",
         "Leo`s Lab",
         "Leo`s Lab\rPower Plant",
         "Model Town 1",
         "Model Town 1",
         "Model Town 1",
         "Model Town 2",
         "Model Town 2",
         "Magridd Castle\rBasement",
         "Magridd Castle\rBasement",
         "Magridd Castle\rBasement",
         "Magridd Castle\rBasement",
         "Magridd Castle\rBasement",
         "Magridd Castle\rBasement",
         "Magridd Castle\rRight Tower",
         "Magridd Castle\rRight Tower",
         "Magridd Castle\rRight Tower",
         "Magridd Castle\rRight Tower",
         "World of Evil",
         "World of Evil",
         "World of Evil",
         "World of Evil",
         "Dazzling Space",
         "Dazzling Space",
         "Tool Shop Owner",
         "Emblem A tile",
         "Goat Pen tile",
         "Teddy",
         "Sleeping Tulip",
         "Grass Valley\rSecret hideout",
         "Village Chief",
         "Magician",
         "Recovery Sword\rcrystal fairy",
         "Grass Valley\rcrystal fairy",
         "Underground Castle\rcrystal fairy",
         "Red-Hot Mirror bird",
         "Master`s Emblems\rcrystal fairy",
         "Woodstin Trio",
         "Greenwood`s Guardian",
         "Turbo`s bones",
         "Shield Bracelet mole",
         "Psycho Sword\rsquirrel",
         "Emblem C squirrel",
         "Water Shrine\rsecret tile",
         "Light Arrow\rcrystal fairy",
         "Lost Marsh\rcrystal fairy",
         "Water Shrine\rcrystal fairy",
         "Fire Shrine\rcrystal fairy",
         "Mountain King",
         "Mushroom Shoes boy",
         "Nome",
         "Emblem E snail",
         "Emblem F tile\rin Lune",
         "Mountain of Souls\rcrystal fairy",
         "Lune\rcrystal fairy",
         "Locked room\rChest of Drawers",
         "Locked room\rChest of Drawers",
         "Leo`s Lab\rMedical Herb plant",
         "Leo`s Cat",
         "Actinidia Leaf\rplant",
         "Leo`s Attic\rChest of Drawers",
         "Marie",
         "Spark Bomb Mouse",
         "Leo`s Lab basement\rcrystal fairy",
         "Model Town 1\rcrystal fairy",
         "Power Plant\rcrystal fairy",
         "Sleeping Soldier",
         "Tile under\rQueen Magridd",
         "Queen Magridd",
         "Platinum Card\rSoldier",
         "Magridd Castle\rMedical Herb Maid",
         "Magridd Castle\rEmblem H tile",
         "King Magridd",
         "Dr.Leo",
         "Magridd Castle\rHarp String tile",
         "St. Elles\rMedical Herb Mermaid",
         "Bubble Armor Mermaid",
         "Magic Flare Mermaid",
         "Mermaid Queen",
         "Red-Hot Stick\rMermaid",
         "Lue",
         "Rockbird\rcrystal fairy",
         "Northern Seabed\rcrystal fairy",
         "Southern Seabed\rcrystal fairy"
    };

    static const char* GetRegionName(const Lair &Lair) {
        switch (Lair.PositionData[0]) {
        case 0x05:
        case 0x06:
            return "Underground Castle";
        case 0x07:
        case 0x08:
        case 0x09:
        case 0x0B:
        case 0x0C:
            return "Leo's Paintings";
        case 0x0D:
            return "Trial Room";
        case 0x19:
            return "Lost Marshes";
        case 0x1A:
        case 0x1B:
        case 0x1C:
            return "Water Shrine";
        case 0x1D:
        case 0x1E:
        case 0x1F:
            return "Fire Shrine";
        case 0x20:
        case 0x21:
        case 0x22:
            return "Light Shrine";
        case 0x2A:
            return "Southern Seabed";
        case 0x2E:
            return "Southerta";
        case 0x2F:
            return "Rockbird";
        case 0x30:
            return "Durean";
        case 0x31:
            return "Blester";
        case 0x32:
            return "Ghost Ship";
        case 0x34:
            return "Northern Seabed";
        case 0x3E:
        case 0x3F:
        case 0x45:
            return "Mountain of Souls";
        case 0x41:
        case 0x42:
        case 0x44:
            return "Lune";
        case 0x46:
        case 0x47:
            return "Laynole";
        case 0x56:
        case 0x57:
            return "Leo`s Lab Basement";
        case 0x58:
        case 0x59:
            return "Leo`s Lab Power Plant";
        case 0x5B:
            return "Model Town 1";
        case 0x5C:
            return "Model Town 2";
        case 0x66:
        case 0x67:
        case 0x68:
            return "Magridd Castle\rBasement";
        case 0x6A:
        case 0x6B:
        case 0x6C:
            return "Magridd Castle\rLeft Tower";
        case 0x6D:
        case 0x6E:
        case 0x6F:
        case 0x72:
            return "Magridd Castle\rRight Tower";
        default:
            /* Should not happen */
            return "";
        }
    };




    static int PickEndTextCode(int NPCItemID) {
        int EndTextCode = TEXT_ENDTYPE_44AA;
        if (NPCItemID <= 10) {
            EndTextCode = TEXT_ENDTYPE_88B9;
        }
        else if (NPCItemID <= 23) {
            EndTextCode = TEXT_ENDTYPE_46EC;
        }
        else if (NPCItemID <= 30) {
            EndTextCode = TEXT_ENDTYPE_1EA5;
        }
        else if (NPCItemID <= 41) {
            EndTextCode = TEXT_ENDTYPE_A3BF;
        }
        else if (NPCItemID <= 50) {
            EndTextCode = TEXT_ENDTYPE_DFF0;
        }
        return EndTextCode;
    }


    int ConvertToHex(int Dec) {
        /* Converts a decimal integer into its hex "equivalent"
           This is useful where the ROM stores the data as decimal values (like gems in chests). */
        return (((Dec / 10) << 4) + (Dec % 10)) & 0xFF;
    }


    void GeneralTextUpdate(const std::vector<Lair>& RandomizedLairList,
                           const std::vector<Item>& RandomizedItemList,
                           std::fstream &ROMFile,
                           long Seed) {

        unsigned char Byte;
        const char* Text;

        /*** NPC actions to disable (mostly to remove NPC revival text) */
        for (int i=0; i<NB_NPC_TO_DISABLE_ADDRESSES; ++i) {
            ROMFile.seekp(NPCToDisableAddressList[i], std::ios::beg);

            /* For leader NPCs, keep the 02 37 code to heal the hero */
            if (i == 5  || /* Village Chief */
                i == 8  || /* Greenwood's Guardian */
                i == 12 || /* Marie */
                i == 17) { /* King Magridd */
                TEXT_WriteByte(0x37); /* Heal the hero completely */
                TEXT_WriteByte(0x02);
            }
            TEXT_WriteByte(0x86);
            TEXT_WriteByte(0x6B); /* End code byte */
        }

        /*** Deathtoll's text */
        /* First text */
        ROMFile.seekp(0x4EF9, std::ios::beg);
        Text = Deathtoll1TextList[Random::RandomInteger(NB_DEATHTOLL_1_TEXTS)];
        TEXT_WriteString(Text);
        TEXT_EndText12;
        /* Text after first phase */
        ROMFile.seekp(0x4FB7, std::ios::beg);
        Text = Deathtoll2TextList[Random::RandomInteger(NB_DEATHTOLL_2_TEXTS)];
        TEXT_WriteString(Text);
        TEXT_EndText12;
        /* Victory text */
        ROMFile.seekp(0x5388, std::ios::beg);
        TEXT_WriteByte(0x0B); /* Change text address */
        /* Note: there seems to be slightly different text here... diff between Any% and 100%??? */
        ROMFile.seekp(0x53C7, std::ios::beg);
        TEXT_EndText12;
        ROMFile.seekp(0x540C, std::ios::beg);
        Text = VictoryTextList[Random::RandomInteger(NB_VICTORY_TEXTS)];
        TEXT_WriteString(Text);
        TEXT_EndText12;
        /* DEBUG!!! Put Deathtoll's HP to 1 */
//        ROMFile.seekp(0x997E, std::ios::beg);TEXT_WriteByte(0x01);

        /*** Master's text when hero dies */
        ROMFile.seekp(0x786B, std::ios::beg);
        Text = MasterDeathTextList[Random::RandomInteger(NB_MASTER_DEATH_TEXTS)];
        TEXT_WriteString(Text);
        TEXT_EndText(TEXT_ENDTYPE_52FA);

        /*** Master's text after Brown Stone */
        ROMFile.seekp(0x78BC, std::ios::beg);
        TEXT_WriteString("One down,\rfive to go!");
        TEXT_EndText(TEXT_ENDTYPE_52FA);

        /*** Master's first text */
        ROMFile.seekp(0x7999, std::ios::beg);
        Text = MasterIntroTextList[Random::RandomInteger(NB_MASTER_INTRO_TEXTS)];
        TEXT_WriteString(Text);
        TEXT_EndText(TEXT_ENDTYPE_52FA);
        ROMFile.seekp(0x7A07, std::ios::beg);
        TEXT_EndText(TEXT_ENDTYPE_52FA);

        /*** Title + file selection screens */
        ROMFile.seekp(0x13B2B, std::ios::beg);
        TEXT_WriteString("RANDO HYPE");
        ROMFile.seekp(0x13B3C, std::ios::beg);
        TEXT_WriteString("RandoBlazer v0.5c  ");
        ROMFile.seekp(0x143B9, std::ios::beg);
        TEXT_WriteString("Seed ");
        char SeedChr[11] = {'\0'};
        sprintf(SeedChr, "%10lu", Seed);
        TEXT_WriteString(SeedChr);

        /*** Correct Magic Flare typo + Greenwood/Actinidia leaves + "received" typo */
        ROMFile.seekp(0x150EC, std::ios::beg);
        TEXT_WriteString("re");
        ROMFile.seekp(0x1514C, std::ios::beg);
        TEXT_WriteString("G.Leaf");
        ROMFile.seekp(0x151B2, std::ios::beg);
        TEXT_WriteString("A.Leaf");
        ROMFile.seekp(0x1621E, std::ios::beg);
        TEXT_WriteString("ei");

        /*** Old Woman */
        ROMFile.seekp(0x18121, std::ios::beg);
        TEXT_WriteByte(0x3C); /* Move her to a different location around Lisa's bed */
        TEXT_WriteByte(0x20);

        /*** Tool shop owner - change text condition */
        ROMFile.seekp(0x1839B, std::ios::beg);
        TEXT_WriteItemByte(ITEM_TOOL_SHOP_OWNER);

        /*** Bridge guard */
        ROMFile.seekp(0x18644, std::ios::beg);
        TEXT_WriteString("Please pass.");
        TEXT_EndText(TEXT_ENDTYPE_88B9);

        /*** Water mill keeper */
        ROMFile.seekp(0x1877C, std::ios::beg);
        TEXT_WriteString("Could you please\rturn this wheel?");
        TEXT_EndText(TEXT_ENDTYPE_88B9);
        ROMFile.seekp(0x188B9, std::ios::beg);
        TEXT_EndText(TEXT_ENDTYPE_88B9);

        /*** Lisa - Hack so her dream is always accessible */
        ROMFile.seekp(0x18A6F, std::ios::beg);
        TEXT_WriteByte(0x00); /* Require to not have a non-existing item */
        ROMFile.seekp(0x18A7D, std::ios::beg);
        TEXT_WriteByte(0x7F); /* Change pointer when Village Chief is revived */
        TEXT_WriteByte(0x8A);

        /*** Tool shop owner's son Teddy */
        ROMFile.seekp(0x1922E, std::ios::beg);
        TEXT_WriteItemByte(ITEM_TEDDY); /* change text condition */
        ROMFile.seekp(0x19256, std::ios::beg);
        TEXT_WriteString("Fancy ");
        TEXT_WriteByte(0x97); /* "a " */
        TEXT_WriteByte(0x0D); /* Carriage return */
        TEXT_YellowStyle;
        TEXT_WriteItemString(ITEM_TEDDY);
        TEXT_EndStyle;
        TEXT_WriteString("\rfor a billion dollars?");
        TEXT_WriteByte(0x0C); /* Question prompt */

        /*** Sleeping tulip (move this text to make room for the Pass tile text) */
        ROMFile.seekp(0x1984E, std::ios::beg);
        TEXT_WriteByte(0x9A); /* Change text pointer */
        ROMFile.seekp(0x1989A, std::ios::beg);
        TEXT_WriteByte(0x10);
        TEXT_WriteString("Hello...");

        /*** Gourmet Goat's clue */
        /* First, decide which item the clue will be about */
        int RandomInt = Random::RandomInteger(3);
        ItemID ClueItem;
        int ItemIndex;
        switch (RandomInt) {
        case 0:
            ClueItem = ItemID::SOUL_BLADE;
            break;
        case 1:
            ClueItem = ItemID::SOUL_ARMOR;
            break;
        default:
            ClueItem = ItemID::PHOENIX;
            break;
        }
        /* Now find where this item is */
        for (ItemIndex=0; ItemIndex<NUMBER_OF_ITEMS; ItemIndex++) {
            if (RandomizedItemList[ItemIndex].Contents == ClueItem) break;
        }
        /* Update text */
        ROMFile.seekp(0x19D74, std::ios::beg);
        TEXT_WriteString("If you give me food,\rI will tell you\rwhere ");
        TEXT_YellowStyle;
        TEXT_WriteString(ItemNameList[(size_t)ClueItem]);
        TEXT_EndStyle;
        TEXT_WriteString(" is!");
        TEXT_EndText(TEXT_ENDTYPE_88B9);
        ROMFile.seekp(0x19DCB, std::ios::beg);
        TEXT_WriteString("You`ve got food!\rwill you give it\rto me?");
        TEXT_WriteByte(0x0C); /* Question prompt */
        ROMFile.seekp(0x19E0E, std::ios::beg);
        TEXT_WriteByte(0x10); /* Start new textbox */
        TEXT_YellowStyle;
        TEXT_WriteString(ItemNameList[(size_t)ClueItem]);
        TEXT_EndStyle;
        TEXT_WriteString(" is\r");
        if (ItemIndex < NUMBER_OF_CHESTS) {
            TEXT_WriteString("in a chest in\r");
        }
        else {
            TEXT_WriteString("held by\r");
        }
        TEXT_YellowStyle;
        TEXT_WriteString(ItemLocations[ItemIndex]);
        TEXT_EndStyle;
        TEXT_WriteString("!");
        TEXT_EndText(TEXT_ENDTYPE_88B9);

        /*** Village Chief */
        ROMFile.seekp(0x1A0C0, std::ios::beg);
        TEXT_WriteByte(0x00); /* "Impossible" Item ID to make sure this condition is never fulfilled */

        /* Hack to open up Act2 regardless of what item Village Chief gives */
        ROMFile.seekp(0x1A123, std::ios::beg);
        TEXT_WriteByte(0x33); /* Change pointer */
        ROMFile.seekp(0x1A125, std::ios::beg);
        Byte = (unsigned char)RandomizedItemList[ITEM_VILLAGE_CHIEF].Contents; /* Get the item */
        unsigned char VillageChiefBuffer[19] = {
            0x02, 0x01, 0x91, 0xA1,         /* Text "Gives item" */
            0x00, 0x5E,
            0x02, 0x0A, Byte,               /* Actually give the item */
            0x02, 0x09, 0x00, 0x9B, 0x6B,   /* Set flag: item has been given */
            0x02, 0x01, 0x72, 0xA2, 0x6B};  /* Text when item is already given */
        ROMFile.write((char*)(&VillageChiefBuffer), 19);

        /*** Lisa's dream */
        ROMFile.seekp(0x1A522, std::ios::beg);
        TEXT_WriteByte(0x3C); /* Opening quotation marks */
        TEXT_WriteString("Lisa, you must\rhelp this man.");
        TEXT_WriteByte(0x3E); /* Closing quotation marks */
        TEXT_EndText(TEXT_ENDTYPE_88B9);
        ROMFile.seekp(0x1A5AF, std::ios::beg);
        TEXT_EndText(TEXT_ENDTYPE_88B9);

        /*** Magician text 2 */
        ROMFile.seekp(0x1A914, std::ios::beg);
        TEXT_WriteString("Good luck and/or\rblame Everhate.");
        TEXT_EndText(TEXT_ENDTYPE_88B9);

        /*** Revival of first Underground Castle Crystal Fairy */
        ROMFile.seekp(0x1AC5B, std::ios::beg);
        TEXT_WriteByte(0x00);

        /*** Underground Castle east part Crystal fairy */
        ROMFile.seekp(0x1AE14, std::ios::beg);
        TEXT_WriteString("I`ve got nothing\rfor you.");
        TEXT_WriteByte(0x11);
        TEXT_WriteByte(0x0C);

        /*** Red-Hot Mirror bird - change text condition */
        ROMFile.seekp(0x1BE3D, std::ios::beg);
        TEXT_WriteItemByte(ITEM_BIRD_RED_HOT_MIRROR);
        ROMFile.seekp(0x1BE44, std::ios::beg);
        TEXT_WriteItemByte(ITEM_BIRD_RED_HOT_MIRROR);

        /*** Magic Bell crystal fairy's text */
        ROMFile.seekp(0x1C0C6, std::ios::beg);
        TEXT_WriteItemByte(ITEM_CRYSTAL_MAGIC_BELL); /* change text condition */
        ROMFile.seekp(0x1C11E, std::ios::beg);
        TEXT_WriteString("If you bring me all 8\rMaster`s Emblems,\rI will give you a\r");
        TEXT_YellowStyle;
        TEXT_WriteItemString(ITEM_CRYSTAL_MAGIC_BELL);
        TEXT_EndStyle;
        TEXT_WriteString(".");
        TEXT_EndText(TEXT_ENDTYPE_46EC);

        /*** Woodstin Trio */
        ROMFile.seekp(0x1D135, std::ios::beg);
        TEXT_WriteString("Let`s start the show!\rThe prize is a\r");
        TEXT_YellowStyle;
        TEXT_WriteItemString(ITEM_WOODSTIN_TRIO);
        TEXT_EndStyle;
        TEXT_WriteString(".");
        TEXT_EndText(TEXT_ENDTYPE_46EC);
        ROMFile.seekp(0x1D175, std::ios::beg);
        TEXT_WriteString("Show time!");
        TEXT_EndText(TEXT_ENDTYPE_46EC);
        ROMFile.seekp(0x1D1E6, std::ios::beg);
        TEXT_WriteString("Tadaaa!\rTry to guess where\rI go.");
        TEXT_EndText(TEXT_ENDTYPE_46EC);

        /*** Fix for Mole's Ribbon chest */
        ROMFile.seekp(0x1D9B3, std::ios::beg);
        TEXT_WriteByte(0x00);

        /*** Shield Bracelet mole - change text condition */
        ROMFile.seekp(0x1DC07, std::ios::beg);
        TEXT_WriteItemByte(ITEM_MOLE_SHIELD_BRACELET);

        /*** Monmo */
        ROMFile.seekp(0x1DE76, std::ios::beg);
        TEXT_WriteString("It is so bright\rout here. Please lead\rme to my home!");
        TEXT_EndText(TEXT_ENDTYPE_46EC);

        /*** Psycho Sword squirrel's revival text */
        ROMFile.seekp(0x1E01C, std::ios::beg);
        TEXT_WriteItemByte(ITEM_SQUIRREL_PSYCHO_SWORD); /* change text condition */
        ROMFile.seekp(0x1E14D, std::ios::beg);
        TEXT_WriteString("I will exchange this\r");
        TEXT_YellowStyle;
        TEXT_WriteItemString(ITEM_SQUIRREL_PSYCHO_SWORD);
        TEXT_EndStyle;
        TEXT_WriteString("\rfor food...");
        TEXT_EndText(TEXT_ENDTYPE_88B9);

        /*** Turbo's text in sleeping bird's dream */
        ROMFile.seekp(0x1E344, std::ios::beg);
        TEXT_WriteString("Peace, guys.");
        TEXT_EndText(TEXT_ENDTYPE_46EC);

        /*** Lost Marsh raft */
        ROMFile.seekp(0x1E68E, std::ios::beg);
        TEXT_WriteString("You`re missing the\rGreenwood Leaves!");
        TEXT_EndText(TEXT_ENDTYPE_46EC);
        ROMFile.seekp(0x1E713, std::ios::beg);
        TEXT_WriteString("Get on!");
        TEXT_EndText(TEXT_ENDTYPE_46EC);

        /*** Sleeping stump */
        ROMFile.seekp(0x1E427, std::ios::beg);
        TEXT_WriteString("I believe I can\rflyyyyy...");
        TEXT_EndText(TEXT_ENDTYPE_46EC);

        /*** Act 2 crystal fairies: change text pointers */
        ROMFile.seekp(0x1E4DF, std::ios::beg); /* Lost Marsh crystal */
        TEXT_WriteByte(0x60);
        TEXT_WriteByte(0xE3);
        ROMFile.seekp(0x1E533, std::ios::beg); /* Water Shrine crystal */
        TEXT_WriteByte(0xC0);
        TEXT_WriteByte(0xE6);

        /*** Master's text after World of Evil is opened */
        ROMFile.seekp(0x1EC55, std::ios::beg); /* Hack to make all 6 stones required */
        unsigned char MasterBuffer[220] = {
            0x02, 0x07, 0x02, 0x9F, 0x05, 0xED,                   /* Test flag: are we in the ending sequence? */
            0x02, 0x07, 0x05, 0x9F, 0xC6, 0xEC,                   /* Test flag: go to another flag test later */
            0x02, 0x18, 0x3A, 0x7C, 0xED,                         /* \   */
            0x02, 0x18, 0x3B, 0x7C, 0xED,                         /*  \  */
            0x02, 0x18, 0x3C, 0x7C, 0xED,                         /*   | */
            0x02, 0x18, 0x3D, 0x7C, 0xED,                         /* New code to test it player has all 6 Stones */
            0x02, 0x18, 0x3E, 0x7C, 0xED,                         /*  /  */
            0x02, 0x18, 0x3F, 0x7C, 0xED,                         /* /   */
            0xA9, 0xC0, 0x7F, 0x0C, 0x26, 0x03,
            0x02, 0x03, 0x03,
            0x02, 0x04,
            0x02, 0x01, 0x31, 0xED,                               /* "Stones swallowed into the aurora" */
            0x02, 0x0B, 0x3A,                                     /* \   */
            0x02, 0x0B, 0x3B,                                     /*  \  */
            0x02, 0x0B, 0x3C,                                     /*   | */
            0x02, 0x0B, 0x3D,                                     /* Remove all stones from the player */
            0x02, 0x0B, 0x3E,                                     /*  /  */
            0x02, 0x0B, 0x3F,                                     /* /   */
            0xA9, 0x20, 0x00, 0x8D, 0xB4, 0x03,
            0x02, 0x09, 0x04, 0x9F,                               /* Set flag (?) */
            0x02, 0x85, 0xEB, 0xEC, 0x03,                         /* Unconditional call (for what?) */
            0x02, 0x85, 0xF8, 0xEC, 0x03,                         /* Unconditional call (for what?) */
            0x02, 0x31, 0x00,
            0x02, 0x03, 0xF1,
            0x02, 0x04,
            0x02, 0x10, 0x00, 0x09, 0x00, 0xD0, 0x00, 0x50, 0x00, 0x6B, /* Teleport (to see the WoE opening probably) */
            0x02, 0x07, 0x03, 0x1F, 0xCF, 0xEC,                   /* Test flag: go to the "Gate to WoE is opened" cutscene */
            0x02, 0x86, 0x6B,
            0xA9, 0xC0, 0x7F, 0x0C, 0x26, 0x03,
            0x02, 0x03, 0x1F,
            0x02, 0x04,
            0x02, 0x01, 0x63, 0xED,                               /* "Gate to the world of evil is finally opened" */
            0x02, 0x09, 0x03, 0x9F,                               /* Set flag: World of Evil is opened */
            0xA9, 0xC0, 0x7F, 0x1C, 0x26, 0x03,
            0x02, 0x86, 0x6B,
            0x02, 0xA8, 0x00, 0x80, 0x0D,
            0x02, 0x92, 0x48,
            0x02, 0x94,
            0x02, 0x86, 0x6B,
            0x02, 0xA8, 0x00, 0x80, 0x0D,
            0x02, 0x92, 0x4A,
            0x02, 0x94,
            0x02, 0x86, 0x6B,
            0xA9, 0xC0, 0xBF, 0x0C, 0x26, 0x03,
            0x02, 0x03, 0x29,
            0x02, 0x04,
            0x02, 0x01, 0x04, 0xEE,                               /* Ending sequence "You did it!" */
            0x02, 0x03, 0x3D,
            0x02, 0x04,
            0xA9, 0x20, 0x00, 0x8D, 0xB4, 0x03,
            0x02, 0x10, 0x10, 0x01, 0x01, 0xF0, 0x01, 0x50, 0x00, /* Teleport (to the next ending cutscene probably) */
            0xA9, 0xC0, 0xBF, 0x1C, 0x26, 0x03,
            0x02, 0x91, 0x6B};
        ROMFile.write((char*)(&MasterBuffer), 220);

        /* Copy cutscene text */
        TEXT_WriteByte(0x10); /* Open textbox */
        TEXT_WriteByte(0x91); /* "The " */
        TEXT_WriteString("6 stones ");
        TEXT_WriteByte(0xD9); /* "started " */
        TEXT_WriteByte(0x0D); /* Carriage return */
        TEXT_WriteByte(0xE2); /* "to " */
        TEXT_WriteString("shine ");
        TEXT_WriteByte(0x96); /* "and " */
        TEXT_WriteByte(0xFD); /* "were " */
        TEXT_WriteByte(0x0D); /* Carriage return */
        TEXT_WriteString("swallowed into ");
        TEXT_WriteByte(0xE1); /* "the " */
        TEXT_WriteByte(0x0D); /* Carriage return */
        TEXT_WriteString("aurora.");
        TEXT_EndText(TEXT_ENDTYPE_C5EE);

        /* "World of Evil is open" cutscene */
        TEXT_WriteByte(0x10); /* Open textbox */
        TEXT_WriteString("Ready for the finale?");
        TEXT_EndText(TEXT_ENDTYPE_C5EE);

        /* New code to call text and move player away if he doesn't have all 6 Stones */
        unsigned char MasterBuffer2[14] = {
            0x02, 0x01, 0x8A, 0xED,                                       /* New text */
            0x02, 0x10, 0x00, 0x07, 0x01, 0x70, 0x00, 0x50, 0x00, 0x6B};  /* Teleport player away from center tile */
        ROMFile.write((char*)(&MasterBuffer2), 14);
        TEXT_WriteByte(0x10); /* Open textbox */
        TEXT_WriteString("Whoa there, Bucko.\rAre you sure you`ve\rgot all 6 stones?");
        TEXT_EndText(TEXT_ENDTYPE_C5EE);

        /*** Mountain King */
        ROMFile.seekp(0x203C1, std::ios::beg);
        TEXT_WriteItemByte(ITEM_MOUNTAIN_KING); /* change text conditions */
        ROMFile.seekp(0x204DF, std::ios::beg);
        TEXT_WriteItemByte(ITEM_MOUNTAIN_KING);
        ROMFile.seekp(0x2058A, std::ios::beg);
        TEXT_WriteItemByte(ITEM_MOUNTAIN_KING);
        ROMFile.seekp(0x20635, std::ios::beg);
        TEXT_WriteItemByte(ITEM_MOUNTAIN_KING);
        ROMFile.seekp(0x20742, std::ios::beg);
        TEXT_WriteString("Did you find the\rthree Red-Hot items?");
        TEXT_EndText(TEXT_ENDTYPE_1EA5);
        ROMFile.seekp(0x20905, std::ios::beg);
        TEXT_WriteString("- DANCING GRANDMAS! -");
        TEXT_EndText(TEXT_ENDTYPE_1EA5);
        ROMFile.seekp(0x2092B, std::ios::beg);
        TEXT_WriteString("Final boss time!");
        TEXT_EndText(TEXT_ENDTYPE_1EA5);
        ROMFile.seekp(0x209F3, std::ios::beg);
        TEXT_WriteString("Good luck!");
        TEXT_EndText(TEXT_ENDTYPE_1EA5);

        /*** Mushroom Shoes boy's revival text */
        ROMFile.seekp(0x20D53, std::ios::beg);
        TEXT_WriteItemByte(ITEM_BOY_MUSHROOM_SHOES); /* change text condition */
        ROMFile.seekp(0x20D74, std::ios::beg);
        TEXT_WriteString("I have a nice\r");
        TEXT_YellowStyle;
        TEXT_WriteItemString(ITEM_BOY_MUSHROOM_SHOES);
        TEXT_EndStyle;
        TEXT_WriteString("\rfor you!");
        TEXT_EndText(TEXT_ENDTYPE_1EA5);

        /*** Lune gatekeeper */
        ROMFile.seekp(0x21859, std::ios::beg);
        TEXT_WriteString("Hold on, tiger.\rWhere is your\r");
        TEXT_YellowStyle;
        TEXT_WriteString("Lucky Blade ");
        TEXT_EndStyle;
        TEXT_WriteString("???");
        TEXT_EndText(TEXT_ENDTYPE_1EA5);

        /*** Act 4 crystal fairies: change text pointers */
        ROMFile.seekp(0x21F45, std::ios::beg); /* Lune crystal */
        TEXT_WriteByte(0x00);
        TEXT_WriteByte(0x91);

        /*** Mouse (mouse hole entrance) */
        ROMFile.seekp(0x22751, std::ios::beg);
        TEXT_WriteString("Thanks!\rPlease come in!");
        TEXT_EndText(TEXT_ENDTYPE_A3BF);

        /*** Chest of drawers (Mystic Armor) */
        ROMFile.seekp(0x22A6B, std::ios::beg);
        TEXT_WriteItemByte(ITEM_CHEST_OF_DRAWERS_MYSTIC_ARMOR); /* change text condition */
        ROMFile.seekp(0x22A9E, std::ios::beg);
        TEXT_WriteString("Hope you like it!");
        TEXT_EndText(TEXT_ENDTYPE_A3BF);

        /*** Leo's Lab herb plant's first question */
        ROMFile.seekp(0x22BB1, std::ios::beg);
        TEXT_WriteItemByte(ITEM_PLANT_HERB); /* change text condition */
        ROMFile.seekp(0x22BC9, std::ios::beg);
        TEXT_WriteString("Fancy ");
        TEXT_WriteByte(0x97); /* "a " */
        TEXT_WriteByte(0x0D); /* Carriage return */
        TEXT_WriteItemString(ITEM_PLANT_HERB);
        TEXT_WriteString("? ");
        TEXT_WriteByte(0x0C); /* Question prompt */

        /*** Actinidia plant - change text condition */
        ROMFile.seekp(0x2319B, std::ios::beg);
        TEXT_WriteItemByte(ITEM_PLANT_ACTINIDIA_LEAVES);

        /*** Chest of drawers - Hack to turn it into a simple NPC giving an item */
        ROMFile.seekp(0x23301, std::ios::beg);
        TEXT_WriteItemByte(ITEM_CHEST_OF_DRAWERS_HERB); /* change text condition */
        ROMFile.seekp(0x232CE, std::ios::beg);
        TEXT_WriteByte(0x08); /* Invert flag check */
        ROMFile.seekp(0x23312, std::ios::beg);
        TEXT_WriteByte(0x6B); /* Prevent setting the flag */

        /*** Spark Bomb mouse - change text condition */
        ROMFile.seekp(0x238E6, std::ios::beg);
        TEXT_WriteItemByte(ITEM_MOUSE_SPARK_BOMB);

        /*** Leo's Lab entrance door */
        ROMFile.seekp(0x23A7B, std::ios::beg);
        TEXT_WriteString("Welcome!");
        TEXT_EndText(TEXT_ENDTYPE_A3BF);

        /*** Leo's cat's dream */
        ROMFile.seekp(0x23B89, std::ios::beg);
        TEXT_WriteString("We can`t let him make\ra deal with Deathtoll!");
        TEXT_EndText(TEXT_ENDTYPE_A3BF);
        ROMFile.seekp(0x23C90, std::ios::beg);
        TEXT_WriteString("Do it for Lisa.");
        TEXT_EndText(TEXT_ENDTYPE_A3BF);
        ROMFile.seekp(0x23D14, std::ios::beg);
        TEXT_WriteString("Oh Master, please\rforgive me.");
        TEXT_EndText(TEXT_ENDTYPE_A3BF);

        /*** Act 5 crystal fairies: change text pointers */
        ROMFile.seekp(0x23E4A, std::ios::beg); /* Model Town 1 crystal */
        TEXT_WriteByte(0xC0);
        TEXT_WriteByte(0xBB);
        ROMFile.seekp(0x23E7A, std::ios::beg); /* Power Plant crystal */
        TEXT_WriteByte(0x00);
        TEXT_WriteByte(0xBC);

        /*** Soldier text: tells what item the sleeping soldier has */
        ROMFile.seekp(0x24119, std::ios::beg);
        TEXT_WriteString("I know a sleeping\rsoldier who has a\r");
        TEXT_YellowStyle;
        TEXT_WriteItemString(ITEM_SOLDIER_ELEMENTAL_MAIL);
        TEXT_EndStyle;
        TEXT_WriteString("...");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);

        /*** Singer's text */
        ROMFile.seekp(0x24677, std::ios::beg);
        TEXT_WriteString("Let`s jam it!");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);

        /*** Super Bracelet tile - change text condition */
        ROMFile.seekp(0x24983, std::ios::beg);
        TEXT_WriteItemByte(ITEM_SUPER_BRACELET);

        /*** Queen Magridd's item - change text condition */
        ROMFile.seekp(0x249A1, std::ios::beg);
        TEXT_WriteItemByte(ITEM_QUEEN_MAGRIDD);

        /*** Platinum Card Soldier - Hack so his item is not permanently missable */
        ROMFile.seekp(0x24BE3, std::ios::beg);
        Byte = (unsigned char)RandomizedItemList[ITEM_SOLDIER_PLATINUM_CARD].Contents; /* Get the item */
        unsigned char PlatCardSoldierBuffer[92] = {
            0x02, 0x17, 0x1A, 0xCC,
            0x02, 0x18, Byte, 0xF2, 0xCB,                         /* If you don't have the item, jump */
            0x02, 0x17, 0x24, 0xCC,
            0x80, 0x07,
            0x02, 0xB0, 0x29, 0xCC, 0x04, 0x00, 0x20,
            0x02, 0x15,
            0x02, 0x05, 0x01, 0x00,                                 /* Loop until... ? */
            0x02, 0x16,                                         /* This is probably to wait for the guard */
            0x02, 0x17, 0x00, 0x00,                             /* to reach his final position after */
            0x02, 0x81, 0x17, 0x02,                             /* the singer has started his music */
            0x02, 0x83,
            0x02, 0x81, 0x16, 0x03,
            0x02, 0x83,
            0x02, 0x17, 0x1F, 0xCC,
            0x02, 0x15,
            0x02, 0x91, 0x6B,
            0x02, 0x01, 0x3F, 0xCC, 0x6B,                       /* Text: tell what item is under the guard's feet */
            0x02, 0x01, 0xDF, 0xCC, 0x6B,                       /* Text when guard is at the front row */
            0x02, 0x01, 0x1B, 0xCD, 0x6B,                        /* Text: guard tired of his job */
            0x02, 0x91,
            0x02, 0x0D, 0x00, 0x0B, 0x30, 0x33, 0xCC, 0x6B,         /* Conditional branch (?) */
            0x02, 0x01, 0x80, 0xCC,                                /* Move the item text location */
            0x00, 0x5E,
            0x02, 0x0A, Byte,                                   /* Get the item */
            0x02, 0x86, 0x6B};
        ROMFile.write((char*)(&PlatCardSoldierBuffer), 92);

        ROMFile.seekp(0x24C3F, std::ios::beg);
        TEXT_WriteByte(0x10); /* Start textbox */
        TEXT_WriteByte(0x93); /* "There " */
        TEXT_WriteByte(0xBA); /* "is " */
        TEXT_WriteByte(0x97); /* "a " */
        TEXT_WriteByte(0x0D); /* Carriage return */
        TEXT_YellowStyle;
        TEXT_WriteItemString(ITEM_SOLDIER_PLATINUM_CARD);
        TEXT_EndStyle;
        TEXT_WriteString(" under\rthe guard`s feet.");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);

        /*** Magridd Castle herb maid's first question */
        ROMFile.seekp(0x24E83, std::ios::beg);
        TEXT_WriteItemByte(ITEM_MAID_HERB); /* change text condition */
        ROMFile.seekp(0x24E9E, std::ios::beg);
        TEXT_WriteString("Would ");
        TEXT_WriteByte(0xFE); /* "you " */
        TEXT_WriteByte(0xBE); /* "like " */
        TEXT_WriteByte(0x97); /* "a " */
        TEXT_WriteByte(0x0D); /* Carriage return */
        TEXT_WriteItemString(ITEM_MAID_HERB);
        TEXT_WriteString("? ");
        TEXT_WriteByte(0x0C); /* Question prompt */

        /*** Soldier with Dr. Leo */
        /* Clue to Leo's location */
        ROMFile.seekp(0x25BB4, std::ios::beg);
        TEXT_WriteString("Dr.Leo must be in\r");
        TEXT_YellowStyle;
        TEXT_WriteString(GetRegionName(RandomizedLairList[NPC_DR_LEO]));
        TEXT_EndStyle;
        TEXT_WriteString("!");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        /* Clue to other soldier's location */
        ROMFile.seekp(0x25BEC, std::ios::beg);
        TEXT_WriteString("My friend is in\r");
        TEXT_YellowStyle;
        TEXT_WriteString(GetRegionName(RandomizedLairList[NPC_SOLDIER_WITH_LEO]));
        TEXT_EndStyle;
        TEXT_WriteString("!");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);

        /*** Dr. Leo - Hack so he doesn't disappear after beating Demon Bird */
        ROMFile.seekp(0x25E73, std::ios::beg);
        TEXT_WriteByte(0x75); /* Change address of this COP command */
        TEXT_WriteByte(0xDE);

        /*** Dr. Leo/Queen Magridd cutscene */
        ROMFile.seekp(0x26145, std::ios::beg);
        TEXT_WriteString("We have to defeat\rDeathtoll!");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x25C7F, std::ios::beg);
        TEXT_WriteString("We`re ready to roll!");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x25D4E, std::ios::beg);
        TEXT_WriteString("Queen Magridd!");
        TEXT_WriteByte(0x11); /* Input prompt */
        TEXT_WriteByte(0x10); /* New textbox */
        TEXT_WriteString("Muahahahaaa!!!\rKill this loser.");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x26241, std::ios::beg);
        TEXT_WriteByte(0x95); /* "You " */
        TEXT_WriteByte(0x98); /* "are " */
        TEXT_WriteString("worse than\revil!");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x2626E, std::ios::beg);
        TEXT_WriteString("Wait, there`s more!");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x262BA, std::ios::beg);
        TEXT_WriteString("Daddy!");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x262EE, std::ios::beg);
        TEXT_WriteString("Dr.Leo, will you\rcooperate now?");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x26383, std::ios::beg);
        TEXT_WriteString("All right. But please\rlet her go.");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x26459, std::ios::beg);
        TEXT_WriteString("Deal.");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x264EC, std::ios::beg);
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x2650A, std::ios::beg);
        TEXT_WriteString("This is it.");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x26773, std::ios::beg);
        TEXT_WriteString("Thanks for everything\ryou are doing for us.\rAre you a real human\rbeing like me?");
        TEXT_WriteByte(0x0C); /* Question prompt */
        ROMFile.seekp(0x26874, std::ios::beg);
        TEXT_WriteString("I see.........\rPlease leave while my\rback is turned.");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);

        /*** Sleeping Soldier's dream */
        ROMFile.seekp(0x26AA7, std::ios::beg);
        TEXT_WriteString("My inventions should\rmake people happy,\rbut they are being\rused for evil.");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x26C9E, std::ios::beg);
        TEXT_WriteByte(0x91); /* "The " */
        TEXT_WriteByte(0xF6); /* "world " */
        TEXT_WriteByte(0xF1); /* "will " */
        TEXT_WriteByte(0xA1); /* "be " */
        TEXT_WriteString("\rdestroyed!");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);
        ROMFile.seekp(0x26CBF, std::ios::beg);
        TEXT_WriteString("I am sorry but\rI have orders.");
        TEXT_EndText(TEXT_ENDTYPE_DFF0);

        /*** Airship - Hack so it still works after King Magridd has been revived */
        int LairIndex;
        ROMFile.seekp(0x26D82, std::ios::beg);
        for (LairIndex=0; LairIndex<NUMBER_OF_LAIRS; LairIndex++) {
            /* Find the index of the lair on the airship */
            if (RandomizedLairList[LairIndex].PositionData[0] == 0x72 /* Airship map */) break;
        }
        TEXT_WriteByte(LairIndex % 0x100);
        TEXT_WriteByte(LairIndex / 0x100);
        ROMFile.seekp(0x26D88, std::ios::beg);
        TEXT_WriteByte(LairIndex % 0x100);
        TEXT_WriteByte(LairIndex / 0x100);

        /*** Left Tower Crystal fairy */
        ROMFile.seekp(0x26DF3, std::ios::beg);
        TEXT_WriteString("I`ve got nothing\rfor you.");
        TEXT_WriteByte(0x11);
        TEXT_WriteByte(0x0C);

        /*** Magridd Castle basement Crystal fairy - Clue on a sword */
        /* First, decide which sword the clue will be about */
        RandomInt = Random::RandomInteger(3);
        switch (RandomInt) {
        case 0:
            ClueItem = ItemID::LUCKY_BLADE;
            break;
        case 1:
            ClueItem = ItemID::ZANTETSU_SWORD;
            break;
        default:
            ClueItem = ItemID::SPIRIT_SWORD;
            break;
        }
        /* Now find where this sword is */
        for (ItemIndex=0; ItemIndex<NUMBER_OF_ITEMS; ItemIndex++) {
            if (RandomizedItemList[ItemIndex].Contents == ClueItem) break;
        }
        /* Update text */
        ROMFile.seekp(0x26EB9, std::ios::beg);
        TEXT_WriteByte(0x91); /* "The " */
        TEXT_YellowStyle;
        TEXT_WriteString(ItemNameList[(size_t)ClueItem]);
        TEXT_EndStyle;
        TEXT_WriteString(" is\r");
        if (ItemIndex < NUMBER_OF_CHESTS) {
            TEXT_WriteString("in a chest in\r");
        }
        else {
            TEXT_WriteString("held by\r");
        }
        TEXT_YellowStyle;
        TEXT_WriteString(ItemLocations[ItemIndex]);
        TEXT_EndStyle;
        TEXT_WriteString("!");
        TEXT_WriteByte(0x11);
        TEXT_WriteByte(0x0C);

        /*** Airship Dock map arrangement hack:
           add a tile to access the ship even if Dr. Leo is there */
        ROMFile.seekp(0xD7E98, std::ios::beg);
        TEXT_WriteByte(0x35); /* Replace a railing tile with a bridge tile */
        TEXT_WriteByte(0xE5);

        /*** St. Elles herb mermaid */
        ROMFile.seekp(0xF8305, std::ios::beg);
        TEXT_WriteItemByte(ITEM_MERMAID_HERB); /* change text condition */
        ROMFile.seekp(0xF831D, std::ios::beg);
        TEXT_WriteString("Would ");
        TEXT_WriteByte(0xFE); /* "you " */
        TEXT_WriteByte(0xBE); /* "like " */
        TEXT_WriteByte(0x97); /* "a " */
        TEXT_WriteByte(0x0D); /* Carriage return */
        TEXT_WriteItemString(ITEM_MERMAID_HERB);
        TEXT_WriteString("? ");
        TEXT_WriteByte(0x0C); /* Question prompt */
        ROMFile.seekp(0xF8356, std::ios::beg);
        TEXT_WriteString("Here you go!");
        TEXT_EndText(TEXT_ENDTYPE_44AA);

        /*** Mermaid statues */
        ROMFile.seekp(0xF8801, std::ios::beg);
        TEXT_WriteString("Blester is open!");
        TEXT_EndText(TEXT_ENDTYPE_44AA);
        ROMFile.seekp(0xF8AD6, std::ios::beg);
        TEXT_WriteString("Rockbird is open!");
        TEXT_EndText(TEXT_ENDTYPE_44AA);
        ROMFile.seekp(0xF8EF1, std::ios::beg);
        TEXT_WriteString("Durean is open!");
        TEXT_EndText(TEXT_ENDTYPE_44AA);
        ROMFile.seekp(0xF9674, std::ios::beg);
        TEXT_WriteString("Ghost Ship is open!");
        TEXT_EndText(TEXT_ENDTYPE_44AA);
        ROMFile.seekp(0xFA029, std::ios::beg);
        TEXT_WriteByte(0x31); /* Change pointer to open Southerta even if other statues are present */
        ROMFile.seekp(0xFA040, std::ios::beg);
        TEXT_WriteString("Southerta is open!");
        TEXT_EndText(TEXT_ENDTYPE_44AA);

        /*** Bubble Armor mermaid's revival text */
        ROMFile.seekp(0xF8BBD, std::ios::beg);
        TEXT_WriteString("Does anyone want my\r");
        TEXT_YellowStyle;
        TEXT_WriteItemString(ITEM_MERMAID_BUBBLE_ARMOR);
        TEXT_EndStyle;
        TEXT_WriteString("?");
        TEXT_EndText(TEXT_ENDTYPE_44AA);

        /*** Fix for Mermaid Tears chest */
        ROMFile.seekp(0xF8CA4, std::ios::beg);
        TEXT_WriteByte(0x00);

        /*** Magic Flare mermaid - change text condition */
        ROMFile.seekp(0xF9087, std::ios::beg);
        TEXT_WriteItemByte(ITEM_MERMAID_MAGIC_FLARE);

        /*** Mermaid Queen - Hack to open Southerta in case the initial Mermaid Statue disappeared */
        ROMFile.seekp(0xF9219, std::ios::beg);
        TEXT_WriteByte(0x2F); /* Change pointer */
        ROMFile.seekp(0xF921D, std::ios::beg);
        TEXT_WriteByte(0x80); /* Change text pointer */
        ROMFile.seekp(0xF9228, std::ios::beg);
        unsigned char MermaidQueenBuffer[57] = {
            0x02, 0x08, 0x02, 0x85, 0x58, 0x92, 0x6B, /* If Southerta isn't open, jump to F9258 */
            0x02, 0x01, 0x8C, 0x93, 0x6B,             /* Point to chunk of weird empty text before "Queen!", not sure what this does */
            0xA9, 0xFF, 0xFF, 0x8D, 0xFB, 0x03,       /* This is not a COP routine call, no idea what it does */
            0x02, 0x14, 0xBD, 0x00, 0x48, 0x92,       /* If one of the mermaids by the Queen's side is released, jump to F9248 */
            0x02, 0x14, 0x92, 0x00, 0x48, 0x92,       /* If the other mermaid by the Queen's side is released, jump to F9248 */
            0x80, 0x04,                               /* This is not a COP routine call, no idea what it does */
            0x02, 0x01, 0x90, 0x93,                   /* Point to mermaids' text "Queen!" */
            0x02, 0x01, 0x9F, 0x93,                   /* Point to mermaid Queen's revival text "A beautiful voice..." */
            0x02, 0x37,                               /* Heal the player fully */
            0x9C, 0xFB, 0x03,                         /* This is not a COP routine call, no idea what it does */
            0x02, 0x86, 0x6B,                         /* Some "quit" function maybe? */
            0x02, 0x01, 0x61, 0x92,                   /* Point to our new text "Southerta is open!" at F9261 */
            0x02, 0x09, 0x02, 0x85, 0x6B};            /* Set the flag to open Southerta */
        ROMFile.write((char*)(&MermaidQueenBuffer), 57);
        TEXT_WriteByte(0x10); /* Open textbox */
        TEXT_WriteString("Southerta is open!");
        TEXT_EndText(TEXT_ENDTYPE_44AA);

        /*** Red-Hot Stick mermaid - change text conditions */
        ROMFile.seekp(0xF985A, std::ios::beg);
        TEXT_WriteItemByte(ITEM_MERMAID_RED_HOT_STICK);
        ROMFile.seekp(0xF9861, std::ios::beg);
        TEXT_WriteItemByte(ITEM_MERMAID_RED_HOT_STICK);

        /*** Lue - change text conditions */
        ROMFile.seekp(0xF9BB7, std::ios::beg);
        TEXT_WriteItemByte(ITEM_LUE);
        /* Modify text conditions for Mermaid guarding Lue's prison entrance */
        ROMFile.seekp(0xF98FE, std::ios::beg);
        TEXT_WriteItemByte(ITEM_LUE);
        ROMFile.seekp(0xF99D3, std::ios::beg);
        TEXT_WriteItemByte(ITEM_LUE);

        /*** Act 3 crystal fairies: change text pointers */
        ROMFile.seekp(0xFA4B7, std::ios::beg); /* Seabed crystal near Blester */
        TEXT_WriteByte(0x40);
        TEXT_WriteByte(0x9C);
        ROMFile.seekp(0xFA4E7, std::ios::beg); /* Seabed crystal near Durean */
        TEXT_WriteByte(0x60);
        TEXT_WriteByte(0xA0);

        /*** Blester Crystal fairy */
        ROMFile.seekp(0xFA517, std::ios::beg);
        TEXT_WriteString("I`ve got nothing\rfor you.");
        TEXT_WriteByte(0x11);
        TEXT_WriteByte(0x0C);
    }


    void NPCItemTextUpdate(int ItemIndex, ItemID itemID, std::fstream &ROMFile) {

        unsigned int Byte;
        const char* ItemName;
        int NPCItemIndex = ItemIndex - NUMBER_OF_CHESTS;

        /* Get the item name */
        if (itemID != ItemID::GEMS_EXP) {
            ItemName = ItemNameList[(size_t)itemID];
        }
        else {
            ItemName = "EXP";
        }

        /* Update text when NPC gives the item */
        if (NPCItemTextAddressList[NPCItemIndex] != 0) {

            ROMFile.seekp(NPCItemTextAddressList[NPCItemIndex], std::ios::beg);
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
            else {
                /* Normal case */
                if (ItemIndex == ITEM_CRYSTAL_LOST_MARSH ||
                    ItemIndex == ITEM_CRYSTAL_WATER_SHRINE ||
                    ItemIndex == ITEM_CRYSTAL_LUNE ||
                    ItemIndex == ITEM_CRYSTAL_MODEL_TOWN ||
                    ItemIndex == ITEM_CRYSTAL_POWER_PLANT ||
                    ItemIndex == ITEM_CRYSTAL_SEABED_NEAR_BLESTER ||
                    ItemIndex == ITEM_CRYSTAL_SEABED_NEAR_DUREAN ||
                    ItemIndex == ITEM_SOLDIER_PLATINUM_CARD ||
                    ItemIndex == ITEM_MERMAID_QUEEN) {
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
        }

        /* Update text when NPC doesn't give its item because the hero already has it */
        if (NPCAlreadyHaveItemTextAddressList[NPCItemIndex] != 0) {

            ROMFile.seekp(NPCAlreadyHaveItemTextAddressList[NPCItemIndex], std::ios::beg);
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
    }


    void NPCTextUpdateMain(const std::vector<Lair>& RandomizedLairList,
                           const std::vector<Item>& RandomizedItemList,
                           std::fstream &ROMFile,
                           long Seed) {
        unsigned char GemsExpValue[2];
        unsigned char Byte;
        int GemsExp_TensAndUnits;
        int ItemAddress;

        /* General text updates and some specific Item NPC text */
        GeneralTextUpdate(RandomizedLairList, RandomizedItemList, ROMFile, Seed);

        /* Fill NPC items */
        for (int i=NUMBER_OF_CHESTS; i<NUMBER_OF_ITEMS; i++) {
            ItemAddress = NPCItemAddressList[i-NUMBER_OF_CHESTS];
            ItemID itemID = RandomizedItemList[i].Contents;

            if (itemID == ItemID::GEMS_EXP || itemID == ItemID::NOTHING) {

                if (ROMData::NPCOriginallyGivesEXP(i)) {
                    ROMFile.seekp (ItemAddress, std::ios::beg);
                    GemsExp_TensAndUnits = RandomizedItemList[i].GemsExp % 100;
                    GemsExpValue[0] = ConvertToHex(GemsExp_TensAndUnits);
                    GemsExpValue[1] = ConvertToHex((RandomizedItemList[i].GemsExp - GemsExp_TensAndUnits) / 100);
                    ROMFile.write((char*)(&GemsExpValue[0]), 2);
                }
                else {
                    /* If the NPC doesn't normally give EXP, let us turn the
                       prize into a Medical Herb for now. */
                    ROMFile.seekp (ItemAddress, std::ios::beg);
                    itemID = ItemID::MEDICAL_HERB;
                    ROMFile.write((char*)(&itemID), 1);
                }
            }
            else {

                if (ROMData::NPCOriginallyGivesEXP(i)) {
                    /* If the NPC is a crystal fairy which normally gives EXP,
                       we need to do some tweaking to make it give an item. */
                    ROMFile.seekp (ItemAddress-1, std::ios::beg);
                    TEXT_WriteByte(0x0A);
                    ROMFile.write((char*)(&itemID), 1);

                    if (i == ITEM_CRYSTAL_FIRE_SHRINE) {
                        /* This one is really weird, the textbox sometimes glitches out */
                        TEXT_WriteByte(0x02);
                    }
                    else {
                        TEXT_WriteByte(0x00);
                    }
                }
                else {
                    ROMFile.seekp (ItemAddress, std::ios::beg);
                    ROMFile.write((char*)(&itemID), 1);
                }
            }

            /* Update the NPC's text accordingly */
            NPCItemTextUpdate(i, itemID, ROMFile);
        }

    }

}
