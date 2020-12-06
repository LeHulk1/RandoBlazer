#include "Map.h"


#define WEIGHT_CAP  8


Element::Element(GoalType NewType, int NewIndex) {
    Type  = NewType;
    Index = NewIndex;
}

Element::~Element() {}

Region::Region()  {}
Region::~Region() {}

void Region::InsertElement(GoalType Type, int Index) {
    Element NewElement(Type, Index);
    Contents.push_back(NewElement);
}

void Region::InsertGoal(int Index) {
    NextGoals.push_back(Index);
}

Goal::Goal()  {
    Weight = 0;
}
Goal::~Goal() {}

void Goal::InsertElement(GoalType Type, int Index) {
    Element NewElement(Type, Index);
    Requirements.push_back(NewElement);
}



namespace Map {

    void InitMap(std::vector<Region> &RegionList, std::vector<Goal> &GoalList) {

        /******** REGIONS ********/

        /* Region 0 - Act 1 start, Underground Castle before elevator */
        RegionList[0].InsertElement(GoalType::ITEM, ITEM_MAGICIAN);
        RegionList[0].InsertElement(GoalType::LAIR, NPC_OLD_WOMAN);
        RegionList[0].InsertElement(GoalType::LAIR, NPC_TOOL_SHOP_OWNER);
        RegionList[0].InsertElement(GoalType::LAIR, NPC_TULIP);
        RegionList[0].InsertElement(GoalType::LAIR, NPC_BRIDGE_GUARD);
        RegionList[0].InsertElement(GoalType::LAIR, NPC_IVY_CHEST_ROOM);
        RegionList[0].InsertElement(GoalType::LAIR, NPC_WATER_MILL);
        RegionList[0].InsertElement(GoalType::ITEM, 3);
        RegionList[0].InsertElement(GoalType::ITEM, 4);
        RegionList[0].InsertElement(GoalType::ITEM, CHEST_DREAM_ROD);
        RegionList[0].InsertElement(GoalType::ITEM, ITEM_CRYSTAL_UNDERGROUND_CASTLE);
        RegionList[0].InsertGoal(0);  /* Underground chest room */
        RegionList[0].InsertGoal(1);  /* Water Mill */
        RegionList[0].InsertGoal(2);  /* Leo's Paintings */
        RegionList[0].InsertGoal(3);  /* Village Chief ==> Act 2 */
        RegionList[0].InsertGoal(41); /* Tool Shop Owner */
        RegionList[0].InsertGoal(43); /* Emblem A tile */
        RegionList[0].InsertGoal(44); /* Secret Cave pass */
        RegionList[0].InsertGoal(45); /* Goat pen corner tile */
        RegionList[0].InsertGoal(46); /* Secret Cave access */

        /* Region 1 - Underground chest room */
        RegionList[1].InsertElement(GoalType::ITEM, 1);
        RegionList[1].InsertElement(GoalType::ITEM, 2);
        RegionList[1].InsertElement(GoalType::ITEM, ITEM_CRYSTAL_GRASS_VALLEY);

        /* Region 2 - Underground Castle after elevator */
        RegionList[2].InsertElement(GoalType::LAIR, NPC_OLD_MAN2);
        RegionList[2].InsertElement(GoalType::LAIR, NPC_GOAT_HERB);
        RegionList[2].InsertElement(GoalType::LAIR, NPC_LISA);
        RegionList[2].InsertElement(GoalType::LAIR, NPC_TULIP2);
        RegionList[2].InsertElement(GoalType::LAIR, NPC_ARCHITECT);
        RegionList[2].InsertElement(GoalType::LAIR, NPC_IVY2);
        RegionList[2].InsertElement(GoalType::LAIR, NPC_TEDDY);
        RegionList[2].InsertElement(GoalType::LAIR, NPC_GOAT);
        RegionList[2].InsertElement(GoalType::LAIR, NPC_TULIP3);
        RegionList[2].InsertElement(GoalType::LAIR, NPC_LEOS_HOUSE);
        RegionList[2].InsertGoal(4); /* Lisa, Last part of Underground Castle */

        /* Region 3 - Leo's Paintings */
        RegionList[3].InsertElement(GoalType::LAIR, NPC_TULIP4);
        RegionList[3].InsertElement(GoalType::LAIR, NPC_LONELY_GOAT);
        RegionList[3].InsertElement(GoalType::LAIR, NPC_IVY);
        RegionList[3].InsertElement(GoalType::LAIR, NPC_GOAT2);
        RegionList[3].InsertElement(GoalType::LAIR, NPC_BOY_CABIN);
        RegionList[3].InsertElement(GoalType::LAIR, NPC_TULIP_PASS);
        RegionList[3].InsertElement(GoalType::LAIR, NPC_BOY_CAVE);
        RegionList[3].InsertElement(GoalType::LAIR, NPC_VILLAGE_CHIEF);
        RegionList[3].InsertElement(GoalType::ITEM, 7);
        RegionList[3].InsertGoal(5); /* Leo's Paintings Metal Enemies */

        /* Region 4 - Act 2 start, Lost Marshes + Water/Fire Shrines */
        RegionList[4].InsertElement(GoalType::ITEM, ITEM_VILLAGE_CHIEF);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_BIRD);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_DOG);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_SQUIRREL_PSYCHO_SWORD);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_BIRD2);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_MOLE_SOUL_OF_LIGHT);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_CROCODILE);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_SQUIRREL);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_MOLE);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_DEER);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_DOG2);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_DOG4);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_DOG5);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_CROCODILE2);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_SQUIRREL_ICE_ARMOR);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_MOLE2);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_SQUIRREL3);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_BIRD_GREENWOOD_LEAF);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_MOLE3);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_DEER_MAGIC_BELL);
        RegionList[4].InsertElement(GoalType::LAIR, NPC_SQUIRREL2);
        RegionList[4].InsertElement(GoalType::ITEM, 11);
        RegionList[4].InsertElement(GoalType::ITEM, 12);
        RegionList[4].InsertElement(GoalType::ITEM, 13);
        RegionList[4].InsertElement(GoalType::ITEM, 14);
        RegionList[4].InsertElement(GoalType::ITEM, 15);
        RegionList[4].InsertElement(GoalType::ITEM, 16);
        //RegionList[4].InsertElement(GoalType::ITEM, 17);  WARNING: this (normally empty) chest can disappear!!!
        RegionList[4].InsertElement(GoalType::ITEM, 19);
        RegionList[4].InsertElement(GoalType::ITEM, 20);
        RegionList[4].InsertElement(GoalType::ITEM, ITEM_CRYSTAL_WATER_SHRINE);
        RegionList[4].InsertElement(GoalType::ITEM, ITEM_WATER_SHRINE_TILE);
        RegionList[4].InsertElement(GoalType::ITEM, ITEM_CRYSTAL_LIGHT_ARROW);
        RegionList[4].InsertGoal(6);  /* Get Greenwood Leaf */
        RegionList[4].InsertGoal(7);  /* Fire Shrine Scorpions */
        RegionList[4].InsertGoal(8);  /* Monmo's Treasure */
        RegionList[4].InsertGoal(9);  /* Ice Armor chest */
        RegionList[4].InsertGoal(10); /* Greenwood's Guardian ==> Act 3 */
        RegionList[4].InsertGoal(48); /* Red-Hot Mirror bird */
        RegionList[4].InsertGoal(49); /* Magic Bell crystal */
        RegionList[4].InsertGoal(50); /* Woodstin Trio */
        RegionList[4].InsertGoal(51); /* Shield Bracelet mole */
        RegionList[4].InsertGoal(52); /* Psycho Sword squirrel */
        RegionList[4].InsertGoal(74); /* Greenwood Leaf tile */

        /* Region 5 - Last part of Underground Castle */
        RegionList[5].InsertElement(GoalType::LAIR, NPC_OLD_MAN);
        RegionList[5].InsertElement(GoalType::ITEM, CHEST_LEOS_BRUSH);

        /* Region 6 - Leo's Paintings Metal Enemies */
        RegionList[6].InsertElement(GoalType::LAIR, NPC_IVY_EMBLEM_A);
        RegionList[6].InsertElement(GoalType::LAIR, NPC_IVY_RECOVERY_SWORD);
        RegionList[6].InsertElement(GoalType::ITEM, 8);

        /* Region 7 - Last part of Marshes + Light Shrine */
        RegionList[7].InsertElement(GoalType::LAIR, NPC_MOLE_SHIELD_BRACELET);
        RegionList[7].InsertElement(GoalType::LAIR, NPC_DOG3);
        RegionList[7].InsertElement(GoalType::LAIR, NPC_SQUIRREL_EMBLEM_C);
        RegionList[7].InsertElement(GoalType::LAIR, NPC_CROCODILE3);
        RegionList[7].InsertElement(GoalType::LAIR, NPC_MONMO);
        RegionList[7].InsertElement(GoalType::LAIR, NPC_GREENWOODS_GUARDIAN);
        RegionList[7].InsertElement(GoalType::ITEM, ITEM_CRYSTAL_LOST_MARSH);
        RegionList[7].InsertGoal(11); /* Light Shrine Fire Spirits */

        /* Region 8 - Fire Shrine Scorpions */
        RegionList[8].InsertElement(GoalType::LAIR, NPC_BIRD3);
        RegionList[8].InsertElement(GoalType::ITEM, 18);
        RegionList[8].InsertElement(GoalType::ITEM, ITEM_CRYSTAL_FIRE_SHRINE);

        /* Region 9 - Monmo's Treasure */
        RegionList[9].InsertElement(GoalType::ITEM, 10);

        /* Region 10 - Ice Armor chest */
        RegionList[10].InsertElement(GoalType::ITEM, 9);

        /* Region 11 - Act 3 start, Southerta */
        RegionList[11].InsertElement(GoalType::ITEM, ITEM_GREENWOODS_GUARDIAN);
        RegionList[11].InsertElement(GoalType::LAIR, NPC_DOLPHIN2);
        RegionList[11].InsertElement(GoalType::LAIR, NPC_MERMAID4);
        RegionList[11].InsertElement(GoalType::LAIR, NPC_MERMAID5);
        RegionList[11].InsertElement(GoalType::LAIR, NPC_MERMAID6);
        RegionList[11].InsertElement(GoalType::LAIR, NPC_MERMAID_BUBBLE_ARMOR);
        RegionList[11].InsertElement(GoalType::ITEM, 28);
        RegionList[11].InsertGoal(13); /* Mermaid Queen ==> Act 4 */
        RegionList[11].InsertGoal(54); /* Medical Herb mermaid */
        RegionList[11].InsertGoal(55); /* Common Mermaid house */

        /* Region 12 - Light Shrine Fire Spirits */
        RegionList[12].InsertElement(GoalType::LAIR, NPC_BIRD_RED_HOT_MIRROR);
        RegionList[12].InsertElement(GoalType::ITEM, 21);

        /* Region 13 - Last soul of Southerta */
        RegionList[13].InsertElement(GoalType::LAIR, NPC_MERMAID_STATUE_ROCKBIRD);
        RegionList[13].InsertGoal(15); /* Rockbird */
        RegionList[13].InsertGoal(16); /* Blester */

        /* Region 14 - Act 4 start, Mountain of Souls */
        RegionList[14].InsertElement(GoalType::ITEM, ITEM_MERMAID_QUEEN);
        RegionList[14].InsertElement(GoalType::LAIR, NPC_GIRL);
        RegionList[14].InsertElement(GoalType::LAIR, NPC_GRANDPA);
        RegionList[14].InsertElement(GoalType::LAIR, NPC_MUSHROOM);
        RegionList[14].InsertElement(GoalType::LAIR, NPC_BOY);
        RegionList[14].InsertElement(GoalType::LAIR, NPC_GRANDPA2);
        RegionList[14].InsertElement(GoalType::LAIR, NPC_SNAIL_JOCKEY);
        RegionList[14].InsertElement(GoalType::LAIR, NPC_BOY_MUSHROOM_SHOES);
        RegionList[14].InsertElement(GoalType::LAIR, NPC_GIRL2);
        RegionList[14].InsertElement(GoalType::ITEM, 35);
        RegionList[14].InsertElement(GoalType::ITEM, 36);
        RegionList[14].InsertElement(GoalType::ITEM, 37);
        RegionList[14].InsertElement(GoalType::ITEM, 38);
        RegionList[14].InsertElement(GoalType::ITEM, 39);
        RegionList[14].InsertElement(GoalType::ITEM, ITEM_CRYSTAL_MOUNTAIN_OF_SOULS);
        RegionList[14].InsertGoal(23); /* Get Mushroom Shoes */
        RegionList[14].InsertGoal(24); /* Tunnel to Lune */
        RegionList[14].InsertGoal(25); /* Nome + Mountain King ==> Act 5 */
        RegionList[14].InsertGoal(59); /* Emblem E snail */
        RegionList[14].InsertGoal(60); /* Mushroom Shoes boy */
        RegionList[14].InsertGoal(61); /* Mountain King's item */

        /* Region 15 - Big Pearl chest */
        RegionList[15].InsertElement(GoalType::ITEM, CHEST_BIG_PEARL);
        RegionList[15].InsertGoal(56); /* Magic Flare mermaid */

        /* Region 16 - Rockbird */
        RegionList[16].InsertElement(GoalType::LAIR, NPC_MERMAID9);
        RegionList[16].InsertElement(GoalType::LAIR, NPC_MERMAID_TEARS);
        RegionList[16].InsertElement(GoalType::LAIR, NPC_MERMAID_MAGIC_FLARE);
        RegionList[16].InsertElement(GoalType::LAIR, NPC_ANGELFISH_SOUL_OF_SHIELD);
        RegionList[16].InsertElement(GoalType::LAIR, NPC_MERMAID_STATUE_DUREAN);
        RegionList[16].InsertElement(GoalType::ITEM, 29);
        RegionList[16].InsertElement(GoalType::ITEM, 30);
        RegionList[16].InsertElement(GoalType::ITEM, ITEM_CRYSTAL_ROCKBIRD);
        RegionList[16].InsertGoal(17); /* Durean */
        RegionList[16].InsertGoal(18); /* Ghost Ship */

        /* Region 17 - Blester */
        RegionList[17].InsertElement(GoalType::LAIR, NPC_ANGELFISH);
        RegionList[17].InsertElement(GoalType::LAIR, NPC_ANGELFISH2);
        RegionList[17].InsertElement(GoalType::LAIR, NPC_MERMAID);
        RegionList[17].InsertElement(GoalType::LAIR, NPC_MERMAID7);
        RegionList[17].InsertElement(GoalType::LAIR, NPC_ANGELFISH4);
        RegionList[17].InsertElement(GoalType::LAIR, NPC_MERMAID8);
        RegionList[17].InsertElement(GoalType::LAIR, NPC_DOLPHIN_SECRET_CAVE);
        RegionList[17].InsertElement(GoalType::ITEM, ITEM_CRYSTAL_SEABED_NEAR_BLESTER);
        RegionList[17].InsertGoal(19); /* Blester Metal Gorillas */

        /* Region 18 - Durean */
        RegionList[18].InsertElement(GoalType::LAIR, NPC_DOLPHIN_PEARL);
        RegionList[18].InsertElement(GoalType::LAIR, NPC_LUE);
        RegionList[18].InsertElement(GoalType::LAIR, NPC_MERMAID_PEARL);
        RegionList[18].InsertElement(GoalType::LAIR, NPC_MERMAID2);
        RegionList[18].InsertElement(GoalType::LAIR, NPC_MERMAID_NANA);
        RegionList[18].InsertElement(GoalType::LAIR, NPC_DOLPHIN_SAVES_LUE);
        RegionList[18].InsertElement(GoalType::LAIR, NPC_MERMAID3);
        RegionList[18].InsertElement(GoalType::LAIR, NPC_MERMAID_STATUE_BLESTER);
        RegionList[18].InsertElement(GoalType::ITEM, 31);
        RegionList[18].InsertElement(GoalType::ITEM, 32);
        RegionList[18].InsertElement(GoalType::ITEM, ITEM_CRYSTAL_SEABED_NEAR_DUREAN);
        RegionList[18].InsertGoal(20); /* Durean Metal Gorillas */

        /* Region 19 - Ghost Ship */
        RegionList[19].InsertElement(GoalType::LAIR, NPC_ANGELFISH3);
        RegionList[19].InsertElement(GoalType::LAIR, NPC_DOLPHIN);
        RegionList[19].InsertElement(GoalType::LAIR, NPC_MERMAID_QUEEN);
        RegionList[19].InsertElement(GoalType::ITEM, 33);
        RegionList[19].InsertElement(GoalType::ITEM, 34);
        RegionList[19].InsertGoal(21); /* Seabed Secret Cave */

        /* Region 20 - Blester Metal Gorillas */
        RegionList[20].InsertElement(GoalType::LAIR, NPC_MERMAID_STATUE_GHOST_SHIP);

        /* Region 21 - Durean Metal Gorillas */
        RegionList[21].InsertElement(GoalType::LAIR, NPC_MERMAID_RED_HOT_STICK);

        /* Region 22 - Seabed Secret Cave */
        RegionList[22].InsertElement(GoalType::ITEM, 24);
        RegionList[22].InsertElement(GoalType::ITEM, 25);
        RegionList[22].InsertElement(GoalType::ITEM, 26);
        RegionList[22].InsertElement(GoalType::ITEM, 27);

        /* Region 23 - Used to be Mermaid's Tears chest, now useless */

        /* Region 24 - Mountain of Souls last lair + Laynole */
        RegionList[24].InsertElement(GoalType::LAIR, NPC_GRANDMA);
        RegionList[24].InsertElement(GoalType::LAIR, NPC_MUSHROOM2);
        RegionList[24].InsertElement(GoalType::LAIR, NPC_SNAIL_RACER);
        RegionList[24].InsertElement(GoalType::LAIR, NPC_SNAIL_RACER2);
        RegionList[24].InsertElement(GoalType::LAIR, NPC_GIRL3);
        RegionList[24].InsertElement(GoalType::LAIR, NPC_MUSHROOM3);
        RegionList[24].InsertElement(GoalType::LAIR, NPC_SNAIL);
        RegionList[24].InsertElement(GoalType::LAIR, NPC_GRANDPA3);
        RegionList[24].InsertElement(GoalType::LAIR, NPC_GRANDPA4);
        RegionList[24].InsertElement(GoalType::LAIR, NPC_GRANDPA_LUNE);
        RegionList[24].InsertElement(GoalType::LAIR, NPC_SNAIL2);
        RegionList[24].InsertElement(GoalType::LAIR, NPC_GRANDPA5);
        RegionList[24].InsertElement(GoalType::ITEM, CHEST_LUCKY_BLADE);

        /* Region 25 - Last part of Laynole + Lune */
        RegionList[25].InsertElement(GoalType::LAIR, NPC_BOY2);
        RegionList[25].InsertElement(GoalType::LAIR, NPC_NOME);
        RegionList[25].InsertElement(GoalType::LAIR, NPC_MUSHROOM_EMBLEM_F);
        RegionList[25].InsertElement(GoalType::LAIR, NPC_DANCING_GRANDMA);
        RegionList[25].InsertElement(GoalType::LAIR, NPC_DANCING_GRANDMA2);
        RegionList[25].InsertElement(GoalType::LAIR, NPC_MOUNTAIN_KING);
        RegionList[25].InsertElement(GoalType::ITEM, 41);
        RegionList[25].InsertElement(GoalType::ITEM, 42);
        RegionList[25].InsertElement(GoalType::ITEM, ITEM_CRYSTAL_LUNE);
        RegionList[25].InsertGoal(39); /* Last lair of Lune */

        /* Region 26 - Act 5 start, first lairs of Leo's Lab Basement */
        RegionList[26].InsertElement(GoalType::ITEM, ITEM_NOME);
        RegionList[26].InsertElement(GoalType::LAIR, NPC_PLANT);
        RegionList[26].InsertElement(GoalType::LAIR, NPC_CAT);
        RegionList[26].InsertElement(GoalType::LAIR, NPC_GREAT_DOOR_ZANTETSU_SWORD);
        RegionList[26].InsertGoal(26); /* Leo's Lab main room */
        RegionList[26].InsertGoal(27); /* Rest of Leo's Lab Basement */
        RegionList[26].InsertGoal(30); /* Light Armor chest */
        RegionList[26].InsertGoal(65); /* Model Town room access */

        /* Region 27 - Leo's Lab main room */
        RegionList[27].InsertElement(GoalType::ITEM, CHEST_ZANTETSU_SWORD);
        RegionList[27].InsertGoal(62); /* Locked dining room */
        RegionList[27].InsertGoal(63); /* Spark Magic mouse */
        RegionList[27].InsertGoal(64); /* Medical Herb plant */

        /* Region 28 - Leo's Lab Basement */
        RegionList[28].InsertElement(GoalType::LAIR, NPC_PLANT_HERB);
        RegionList[28].InsertElement(GoalType::LAIR, NPC_CAT2);
        RegionList[28].InsertElement(GoalType::LAIR, NPC_CAT3);
        RegionList[28].InsertElement(GoalType::LAIR, NPC_GREAT_DOOR);
        RegionList[28].InsertElement(GoalType::LAIR, NPC_CHEST_OF_DRAWERS_MYSTIC_ARMOR);
        RegionList[28].InsertElement(GoalType::LAIR, NPC_CAT_DOOR_KEY);
        RegionList[28].InsertElement(GoalType::LAIR, NPC_STEPS_UPSTAIRS);
        RegionList[28].InsertElement(GoalType::LAIR, NPC_MOUSE);
        RegionList[28].InsertElement(GoalType::LAIR, NPC_GREAT_DOOR_MODEL_TOWNS);
        RegionList[28].InsertElement(GoalType::LAIR, NPC_MODEL_TOWN1);
        RegionList[28].InsertElement(GoalType::ITEM, ITEM_CRYSTAL_LEOS_LAB_BASEMENT);

        /* Region 29 - Model Town 1 */
        RegionList[29].InsertElement(GoalType::LAIR, NPC_CHEST_OF_DRAWERS);
        RegionList[29].InsertElement(GoalType::LAIR, NPC_PLANT2);
        RegionList[29].InsertElement(GoalType::LAIR, NPC_MOUSE2);
        RegionList[29].InsertElement(GoalType::LAIR, NPC_MOUSE3);
        RegionList[29].InsertElement(GoalType::LAIR, NPC_MOUSE4);
        RegionList[29].InsertElement(GoalType::LAIR, NPC_MOUSE_SPARK_BOMB);
        RegionList[29].InsertElement(GoalType::LAIR, NPC_GREAT_DOOR_SOUL_OF_DETECTION);
        RegionList[29].InsertElement(GoalType::LAIR, NPC_MODEL_TOWN2);
        RegionList[29].InsertElement(GoalType::LAIR, NPC_STEPS_MARIE);
        RegionList[29].InsertElement(GoalType::ITEM, 45);
        RegionList[29].InsertElement(GoalType::ITEM, 46);
        RegionList[29].InsertElement(GoalType::ITEM, 47);
        RegionList[29].InsertElement(GoalType::ITEM, ITEM_CRYSTAL_MODEL_TOWN);

        /* Region 30 - Model Town 2 */
        RegionList[30].InsertElement(GoalType::LAIR, NPC_CHEST_OF_DRAWERS2);
        RegionList[30].InsertElement(GoalType::LAIR, NPC_PLANT_ACTINIDIA_LEAVES);
        RegionList[30].InsertElement(GoalType::LAIR, NPC_MOUSE5);
        RegionList[30].InsertElement(GoalType::LAIR, NPC_CAT4);
        RegionList[30].InsertElement(GoalType::LAIR, NPC_STAIRS_POWER_PLANT);
        RegionList[30].InsertElement(GoalType::ITEM, 48);
        RegionList[30].InsertElement(GoalType::ITEM, 49);

        /* Region 31 - Light Armor chest */
        RegionList[31].InsertElement(GoalType::ITEM, 44);
        RegionList[31].InsertGoal(31); /* Power Plant Metal enemies */

        /* Region 32 - Power Plant */
        RegionList[32].InsertElement(GoalType::LAIR, NPC_DOLL);
        RegionList[32].InsertElement(GoalType::LAIR, NPC_MARIE);
        RegionList[32].InsertElement(GoalType::ITEM, ITEM_CRYSTAL_POWER_PLANT);

        /* Region 33 - Act 6 start, first lairs of Magridd Castle Basement */
        RegionList[33].InsertElement(GoalType::ITEM, ITEM_MARIE);
        RegionList[33].InsertElement(GoalType::LAIR, NPC_SOLDIER);
        RegionList[33].InsertElement(GoalType::ITEM, 50);
        RegionList[33].InsertElement(GoalType::ITEM, 51);
        RegionList[33].InsertElement(GoalType::ITEM, ITEM_HARP_STRING);
        RegionList[33].InsertGoal(33); /* Spirit Sword */
        RegionList[33].InsertGoal(34); /* Left Tower */
        RegionList[33].InsertGoal(35); /* Right Tower */
        RegionList[33].InsertGoal(68); /* Elemental Mail soldier */
        RegionList[33].InsertGoal(69); /* Queen Magridd */
        RegionList[33].InsertGoal(70); /* Platinum Card soldier */
        RegionList[33].InsertGoal(71); /* Medical Herb maid */
        RegionList[33].InsertGoal(72); /* Castle + Emblem H tile */

        /* Region 34 - Magridd Castle Basement */
        RegionList[34].InsertElement(GoalType::LAIR, NPC_SOLDIER2);
        RegionList[34].InsertElement(GoalType::LAIR, NPC_SINGER_CONCERT_HALL);
        RegionList[34].InsertElement(GoalType::LAIR, NPC_SOLDIER3);
        RegionList[34].InsertElement(GoalType::LAIR, NPC_SOLDIER4);
        RegionList[34].InsertElement(GoalType::LAIR, NPC_SOLDIER5);
        RegionList[34].InsertElement(GoalType::LAIR, NPC_SOLDIER6);
        RegionList[34].InsertElement(GoalType::LAIR, NPC_SOLDIER_ELEMENTAL_MAIL);
        RegionList[34].InsertElement(GoalType::LAIR, NPC_MAID);
        RegionList[34].InsertElement(GoalType::LAIR, NPC_SOLDIER_LEFT_TOWER);
        RegionList[34].InsertElement(GoalType::LAIR, NPC_SOLDIER_DOK);
        RegionList[34].InsertElement(GoalType::LAIR, NPC_SOLDIER_PLATINUM_CARD);
        RegionList[34].InsertElement(GoalType::LAIR, NPC_SINGER);
        RegionList[34].InsertElement(GoalType::ITEM, 52);
        RegionList[34].InsertElement(GoalType::ITEM, 53);
        RegionList[34].InsertElement(GoalType::ITEM, 54);
        RegionList[34].InsertElement(GoalType::ITEM, 55);

        /* Region 35 - Magridd Castle Left Tower */
        RegionList[35].InsertElement(GoalType::LAIR, NPC_SOLDIER_SOUL_OF_REALITY);
        RegionList[35].InsertElement(GoalType::LAIR, NPC_QUEEN_MAGRIDD);
        RegionList[35].InsertElement(GoalType::LAIR, NPC_MAID2);
        RegionList[35].InsertElement(GoalType::LAIR, NPC_SOLDIER_WITH_LEO);
        RegionList[35].InsertElement(GoalType::LAIR, NPC_SOLDIER_RIGHT_TOWER);
        RegionList[35].InsertElement(GoalType::LAIR, NPC_DR_LEO);
        RegionList[35].InsertElement(GoalType::LAIR, NPC_SOLDIER7);

        /* Region 36 - Magridd Castle Right Tower */
        RegionList[36].InsertElement(GoalType::LAIR, NPC_MAID_HERB);
        RegionList[36].InsertElement(GoalType::LAIR, NPC_SOLDIER8);
        RegionList[36].InsertElement(GoalType::LAIR, NPC_SOLDIER_CASTLE);
        RegionList[36].InsertElement(GoalType::LAIR, NPC_SOLDIER9);
        RegionList[36].InsertElement(GoalType::LAIR, NPC_SOLDIER10);
        RegionList[36].InsertElement(GoalType::LAIR, NPC_SOLDIER11);
        RegionList[36].InsertElement(GoalType::ITEM, 56);
        RegionList[36].InsertElement(GoalType::ITEM, 57);
        RegionList[36].InsertElement(GoalType::ITEM, 58);
        RegionList[36].InsertElement(GoalType::ITEM, 59);
        RegionList[36].InsertGoal(37); /* Dr Leo + Queen Magridd cutscene */
        RegionList[36].InsertGoal(75); /* Demon Bird */

        /* Region 37 - Act 7 start, World of Evil */
        RegionList[37].InsertElement(GoalType::ITEM, ITEM_KING_MAGRIDD);
        RegionList[37].InsertElement(GoalType::ITEM, 60);
        RegionList[37].InsertElement(GoalType::ITEM, 61);
        RegionList[37].InsertElement(GoalType::ITEM, CHEST_RED_HOT_BALL);
        RegionList[37].InsertElement(GoalType::ITEM, CHEST_SOUL_ARMOR);
        RegionList[37].InsertGoal(38); /* Soul Armor */

        /* Region 38 - Dr Leo + Queen Magridd cutscene */
        RegionList[38].InsertElement(GoalType::ITEM, ITEM_DR_LEO);
        RegionList[38].InsertGoal(73); /* Super Bracelet tile */

        /* Region 39 - Dazzling Space */
        RegionList[39].InsertElement(GoalType::ITEM, CHEST_SOUL_BLADE);
        RegionList[39].InsertElement(GoalType::ITEM, 65);
        RegionList[39].InsertGoal(40); /* End-game required items */

        /* Region 40 - Last Lair in Lune */
        RegionList[40].InsertElement(GoalType::LAIR, NPC_SNAIL_EMBLEM_E);
        RegionList[40].InsertElement(GoalType::ITEM, ITEM_EMBLEM_F);

        /* Region 41 - End of the game (NOTHING here) */

        /* Region 42 - Tool Shop Owner */
        RegionList[42].InsertElement(GoalType::ITEM, ITEM_TOOL_SHOP_OWNER);
        RegionList[42].InsertGoal(42);

        /* Region 43 - Tool Shop Owner's son Teddy */
        RegionList[43].InsertElement(GoalType::ITEM, ITEM_TEDDY);

        /* Region 44 - Emblem A tile */
        RegionList[44].InsertElement(GoalType::ITEM, ITEM_EMBLEM_A);

        /* Region 45 - Secret Cave pass */
        RegionList[45].InsertElement(GoalType::ITEM, ITEM_PASS);

        /* Region 46 - Goat pen corner tile */
        RegionList[46].InsertElement(GoalType::ITEM, ITEM_GOAT_PEN);

        /* Region 47 - Secret Cave */
        RegionList[47].InsertElement(GoalType::ITEM, ITEM_SECRET_CAVE_TILE);
        RegionList[47].InsertGoal(47); /* Recovery Sword crystal */

        /* Region 48 - Recovery Sword crystal */
        RegionList[48].InsertElement(GoalType::ITEM, ITEM_CRYSTAL_RECOVERY_SWORD);

        /* Region 49 - Red-Hot Mirror bird */
        RegionList[49].InsertElement(GoalType::ITEM, ITEM_BIRD_RED_HOT_MIRROR);

        /* Region 50 - Magic Bell crystal */
        RegionList[50].InsertElement(GoalType::ITEM, ITEM_CRYSTAL_MAGIC_BELL);

        /* Region 51 - Woodstin Trio */
        RegionList[51].InsertElement(GoalType::ITEM, ITEM_WOODSTIN_TRIO);

        /* Region 52 - Shield Bracelet mole */
        RegionList[52].InsertElement(GoalType::ITEM, ITEM_MOLE_SHIELD_BRACELET);

        /* Region 53 - Psycho Sword squirrel */
        RegionList[53].InsertElement(GoalType::ITEM, ITEM_SQUIRREL_PSYCHO_SWORD);
        RegionList[53].InsertGoal(53); /* Emblem C squirrel */

        /* Region 54 - Emblem C squirrel */
        RegionList[54].InsertElement(GoalType::ITEM, ITEM_SQUIRREL_EMBLEM_C);

        /* Region 55 - Medical Herb mermaid (north-eastern house of St Elles) */
        RegionList[55].InsertElement(GoalType::ITEM, ITEM_MERMAID_HERB);

        /* Region 56 - Common Mermaid house */
        RegionList[56].InsertElement(GoalType::ITEM, 22);  /* WARNING: I hope this chest is safe */
        RegionList[56].InsertElement(GoalType::ITEM, ITEM_MERMAID_BUBBLE_ARMOR);
        RegionList[56].InsertGoal(12); /* Last soul of Southerta */
        RegionList[56].InsertGoal(14); /* Big Pearl chest */
        //RegionList[56].InsertGoal(22); /* Mermaid's Tears chest */
        RegionList[56].InsertGoal(57); /* Red-Hot Stick mermaid */
        RegionList[56].InsertGoal(58); /* Lue */

        /* Region 57 - Magic Flare mermaid */
        RegionList[57].InsertElement(GoalType::ITEM, ITEM_MERMAID_MAGIC_FLARE);

        /* Region 58 - Red-Hot Stick mermaid */
        RegionList[58].InsertElement(GoalType::ITEM, ITEM_MERMAID_RED_HOT_STICK);

        /* Region 59 - Lue */
        RegionList[59].InsertElement(GoalType::ITEM, ITEM_LUE);

        /* Region 60 - Emblem E snail */
        RegionList[60].InsertElement(GoalType::ITEM, ITEM_SNAIL_EMBLEM_E);

        /* Region 61 - Mushroom Shoes boy */
        RegionList[61].InsertElement(GoalType::ITEM, ITEM_BOY_MUSHROOM_SHOES);

        /* Region 62 - Mountain King's item + Deathtoll appears! */
        RegionList[62].InsertElement(GoalType::ITEM, ITEM_MOUNTAIN_KING);

        /* Region 63 - Locked dining room */
        RegionList[63].InsertElement(GoalType::ITEM, ITEM_CHEST_OF_DRAWERS_MYSTIC_ARMOR);
        RegionList[63].InsertElement(GoalType::ITEM, ITEM_EMBLEM_G);

        /* Region 64 - Spark Magic mouse */
        RegionList[64].InsertElement(GoalType::ITEM, ITEM_MOUSE_SPARK_BOMB);

        /* Region 65 - Medical Herb plant */
        RegionList[65].InsertElement(GoalType::ITEM, ITEM_PLANT_HERB);

        /* Region 66 - Model Town room */
        RegionList[66].InsertGoal(28); /* Model Town 1 */
        RegionList[66].InsertGoal(29); /* Model Town 2 */
        RegionList[66].InsertGoal(66); /* Leo's Cat */
        RegionList[66].InsertGoal(67); /* Actinidia Plant */
        RegionList[66].InsertGoal(76); /* Attic */

        /* Region 67 - Leo's Cat */
        RegionList[67].InsertElement(GoalType::ITEM, ITEM_CAT_DOOR_KEY);

        /* Region 68 - Actinidia Plant */
        RegionList[68].InsertElement(GoalType::ITEM, ITEM_PLANT_ACTINIDIA_LEAVES);

        /* Region 69 - Elemental Mail soldier */
        RegionList[69].InsertElement(GoalType::ITEM, ITEM_SOLDIER_ELEMENTAL_MAIL);

        /* Region 70 - Queen Magridd */
        //RegionList[70].InsertElement(GoalType::ITEM, ITEM_QUEEN_MAGRIDD); WARNING: this item can be lost if the Queen is already dead

        /* Region 71 - Platinum Card soldier */
        RegionList[71].InsertElement(GoalType::ITEM, ITEM_SOLDIER_PLATINUM_CARD); /* Back into logic */

        /* Region 72 - Medical Herb maid */
        RegionList[72].InsertElement(GoalType::ITEM, ITEM_MAID_HERB);

        /* Region 73 - Magridd Castle + Emblem H tile */
        RegionList[73].InsertElement(GoalType::ITEM, ITEM_EMBLEM_H);
        RegionList[73].InsertGoal(36); /* King Magridd ==> Act 7 */

        /* Region 74 - Super Bracelet tile */
        RegionList[74].InsertElement(GoalType::ITEM, ITEM_SUPER_BRACELET);

        /* Region 75 - Greenwood Leaf tile */
        RegionList[75].InsertElement(GoalType::ITEM, ITEM_GREENWOOD_LEAVES);

        /* Region 76 - Demon Bird's lair */
        RegionList[76].InsertElement(GoalType::LAIR, NPC_KING_MAGRIDD);

        /* Region 77 - Leo's Lab Attic */
        RegionList[77].InsertGoal(32); /* Marie ==> Act 6 */
        RegionList[77].InsertGoal(77); /* Chest of Drawers */

        /* Region 78 - Chest of Drawers in Attic */
        RegionList[78].InsertElement(GoalType::ITEM, ITEM_CHEST_OF_DRAWERS_HERB);



        /******** GOALS ********/

        /* Goal 0 - Underground chest room */
        GoalList[0].InsertElement(GoalType::LAIR, NPC_BRIDGE_GUARD);
        GoalList[0].InsertElement(GoalType::LAIR, NPC_IVY_CHEST_ROOM);
        GoalList[0].Target = 1;

        /* Goal 1 - Water Mill */
        GoalList[1].InsertElement(GoalType::LAIR, NPC_BRIDGE_GUARD);
        GoalList[1].InsertElement(GoalType::LAIR, NPC_WATER_MILL);
        GoalList[1].Target = 2;

        /* Goal 2 - Leo's Paintings */
        GoalList[2].InsertElement(GoalType::LAIR, NPC_BRIDGE_GUARD);
        GoalList[2].InsertElement(GoalType::LAIR, NPC_ARCHITECT);
        GoalList[2].InsertElement(GoalType::LAIR, NPC_LEOS_HOUSE);
        GoalList[2].InsertElement(GoalType::ITEM, CHEST_LEOS_BRUSH);
        GoalList[2].Target = 3;

        /* Goal 3 - Village Chief ==> Act 2 */
        GoalList[3].InsertElement(GoalType::LAIR, NPC_VILLAGE_CHIEF);
        GoalList[3].InsertElement(GoalType::LAIR, NPC_OLD_WOMAN);
        GoalList[3].Target = 4;

        /* Goal 4 - Lisa, Last part of Underground Castle */
        GoalList[4].InsertElement(GoalType::LAIR, NPC_LISA);
        GoalList[4].InsertElement(GoalType::ITEM, CHEST_DREAM_ROD);
        GoalList[4].Target = 5;

        /* Goal 5 - Leo's Paintings Metal Enemies */
        GoalList[5].InsertElement(GoalType::ITEM, CHEST_ZANTETSU_SWORD);
        GoalList[5].Target = 6;

        /* Goal 6 - Greenwood Leaf */
        GoalList[6].InsertElement(GoalType::ITEM, ITEM_GREENWOOD_LEAVES);
        GoalList[6].Target = 7;

        /* Goal 7 - Fire Shrine Scorpions */
        GoalList[7].InsertElement(GoalType::ITEM, CHEST_ZANTETSU_SWORD);
        GoalList[7].Target = 8;

        /* Goal 8 - Monmo's Treasure */
        GoalList[8].InsertElement(GoalType::LAIR, NPC_MONMO);
        GoalList[8].InsertElement(GoalType::LAIR, NPC_MOLE3);
        GoalList[8].Target = 9;

        /* Goal 9 - Ice Armor chest */
        GoalList[9].InsertElement(GoalType::LAIR, NPC_MOLE);
        GoalList[9].InsertElement(GoalType::LAIR, NPC_SQUIRREL_ICE_ARMOR);
        GoalList[9].InsertElement(GoalType::ITEM, CHEST_DREAM_ROD);
        GoalList[9].Target = 10;

        /* Goal 10 - Greenwood's Guardian ==> Act 3 */
        GoalList[10].InsertElement(GoalType::LAIR, NPC_GREENWOODS_GUARDIAN);
        GoalList[10].Target = 11;

        /* Goal 11 - Light Shrine Fire Spirits */
        GoalList[11].InsertElement(GoalType::ITEM, CHEST_SPIRIT_SWORD);
        GoalList[11].Target = 12;

        /* Goal 12 - Last soul of Southerta */
        GoalList[12].InsertElement(GoalType::ITEM, ITEM_MERMAID_BUBBLE_ARMOR);
        GoalList[12].Target = 13;

        /* Goal 13 - Mermaid Queen ==> Act 4 */
        GoalList[13].InsertElement(GoalType::LAIR, NPC_MERMAID_QUEEN);
        GoalList[13].Target = 14;

        /* Goal 14 - Big Pearl chest */
        GoalList[14].InsertElement(GoalType::LAIR, NPC_MERMAID_PEARL);
        GoalList[14].InsertElement(GoalType::LAIR, NPC_DOLPHIN_PEARL);
        GoalList[14].Target = 15;

        /* Goal 15 - Rockbird */
        GoalList[15].InsertElement(GoalType::LAIR, NPC_MERMAID_STATUE_ROCKBIRD);
        GoalList[15].Target = 16;

        /* Goal 16 - Blester */
        GoalList[16].InsertElement(GoalType::LAIR, NPC_MERMAID_STATUE_BLESTER);
        GoalList[16].Target = 17;

        /* Goal 17 - Durean */
        GoalList[17].InsertElement(GoalType::LAIR, NPC_MERMAID_STATUE_DUREAN);
        GoalList[17].Target = 18;

        /* Goal 18 - Ghost Ship */
        GoalList[18].InsertElement(GoalType::LAIR, NPC_MERMAID_STATUE_GHOST_SHIP);
        GoalList[18].Target = 19;

        /* Goal 19 - Blester Metal Gorillas */
        GoalList[19].InsertElement(GoalType::ITEM, ITEM_LUE);
        GoalList[19].Target = 20;

        /* Goal 20 - Durean Metal Gorillas */
        GoalList[20].InsertElement(GoalType::ITEM, CHEST_ZANTETSU_SWORD);
        GoalList[20].Target = 21;

        /* Goal 21 - Seabed Secret Cave */
        GoalList[21].InsertElement(GoalType::LAIR, NPC_MERMAID_PEARL);
        GoalList[21].InsertElement(GoalType::LAIR, NPC_DOLPHIN_SECRET_CAVE);
        GoalList[21].InsertElement(GoalType::ITEM, CHEST_DREAM_ROD);
        GoalList[21].InsertElement(GoalType::ITEM, CHEST_BIG_PEARL);
        GoalList[21].Target = 22;

        /* Goal 22 - Mermaid's Tears chest */
        //GoalList[22].InsertElement(GoalType::LAIR, NPC_MERMAID_TEARS); /* We actually don't need this mermaid */
        GoalList[22].Target = 23;

        /* Goal 23 - Get Mushroom Shoes */
        GoalList[23].InsertElement(GoalType::ITEM, ITEM_BOY_MUSHROOM_SHOES);
        GoalList[23].Target = 24;

        /* Goal 24 - Tunnel to Lune */
        GoalList[24].InsertElement(GoalType::LAIR, NPC_GIRL3);
        GoalList[24].InsertElement(GoalType::LAIR, NPC_GRANDPA4);
        GoalList[24].InsertElement(GoalType::LAIR, NPC_GRANDPA_LUNE);
        GoalList[24].InsertElement(GoalType::ITEM, CHEST_LUCKY_BLADE);
        GoalList[24].Target = 25;

        /* Goal 25 - Nome + Mountain King ==> Act 5 */
        GoalList[25].InsertElement(GoalType::LAIR, NPC_GIRL3);
        GoalList[25].InsertElement(GoalType::LAIR, NPC_GRANDPA4);
        GoalList[25].InsertElement(GoalType::LAIR, NPC_MUSHROOM2);
        GoalList[25].InsertElement(GoalType::LAIR, NPC_GRANDPA5);
        GoalList[25].InsertElement(GoalType::LAIR, NPC_NOME);
        GoalList[25].InsertElement(GoalType::LAIR, NPC_MOUNTAIN_KING);
        GoalList[25].Target = 26;

        /* Goal 26 - Zantetsu Sword chest */
        GoalList[26].InsertElement(GoalType::LAIR, NPC_GREAT_DOOR_ZANTETSU_SWORD);
        GoalList[26].Target = 27;

        /* Goal 27 - Rest of Leo's Lab Basement */
        GoalList[27].InsertElement(GoalType::ITEM, CHEST_ZANTETSU_SWORD);
        GoalList[27].Target = 28;

        /* Goal 28 - Model Town 1 */
        GoalList[28].InsertElement(GoalType::LAIR, NPC_MODEL_TOWN1);
        GoalList[28].Target = 29;

        /* Goal 29 - Model Town 2 */
        GoalList[29].InsertElement(GoalType::LAIR, NPC_MODEL_TOWN2);
        GoalList[29].Target = 30;

        /* Goal 30 - Light Armor chest */
        GoalList[30].InsertElement(GoalType::LAIR, NPC_STAIRS_POWER_PLANT);
        GoalList[30].Target = 31;

        /* Goal 31 - Power Plant */
        GoalList[31].InsertElement(GoalType::ITEM, CHEST_ZANTETSU_SWORD);
        GoalList[31].Target = 32;

        /* Goal 32 - Marie ==> Act 6 */
        GoalList[32].InsertElement(GoalType::LAIR, NPC_MARIE);
        GoalList[32].Target = 33;

        /* Goal 33 - Spirit Sword */
        GoalList[33].InsertElement(GoalType::ITEM, CHEST_SPIRIT_SWORD);
        GoalList[33].Target = 34;

        /* Goal 34 - Magridd Castle Left Tower */
        GoalList[34].InsertElement(GoalType::LAIR, NPC_SOLDIER_LEFT_TOWER);
        GoalList[34].InsertElement(GoalType::ITEM, ITEM_SOLDIER_PLATINUM_CARD);
        GoalList[34].Target = 35;

        /* Goal 35 - Magridd Castle Right Tower */
        GoalList[35].InsertElement(GoalType::LAIR, NPC_SOLDIER_RIGHT_TOWER);
        GoalList[35].InsertElement(GoalType::ITEM, ITEM_QUEEN_MAGRIDD);
        GoalList[35].Target = 36;

        /* Goal 36 - King Magridd ==> Act 7 */
        GoalList[36].InsertElement(GoalType::LAIR, NPC_KING_MAGRIDD);
        GoalList[36].InsertElement(GoalType::ITEM, ITEM_VILLAGE_CHIEF);
        GoalList[36].InsertElement(GoalType::ITEM, ITEM_GREENWOODS_GUARDIAN);
        GoalList[36].InsertElement(GoalType::ITEM, ITEM_MERMAID_QUEEN);
        GoalList[36].InsertElement(GoalType::ITEM, ITEM_NOME);
        GoalList[36].InsertElement(GoalType::ITEM, ITEM_MARIE);
        GoalList[36].InsertElement(GoalType::ITEM, ITEM_KING_MAGRIDD);
        GoalList[36].Target = 37;

        /* Goal 37 - Dr Leo + Queen Magridd cutscene */
        GoalList[37].InsertElement(GoalType::LAIR, NPC_SOLDIER_WITH_LEO);
        GoalList[37].InsertElement(GoalType::LAIR, NPC_SOLDIER_DOK);
        GoalList[37].InsertElement(GoalType::LAIR, NPC_DR_LEO);
        GoalList[37].Target = 38;

        /* Goal 38 - Soul Armor */
        GoalList[38].InsertElement(GoalType::ITEM, CHEST_SOUL_ARMOR);
        GoalList[38].Target = 39;

        /* Goal 39 - Last Lair in Lune */
        GoalList[39].InsertElement(GoalType::LAIR, NPC_MUSHROOM2);
        GoalList[39].InsertElement(GoalType::LAIR, NPC_GRANDPA5);
        GoalList[39].InsertElement(GoalType::LAIR, NPC_MUSHROOM_EMBLEM_F);
        GoalList[39].InsertElement(GoalType::ITEM, CHEST_DREAM_ROD);
        GoalList[39].Target = 40;

        /* Goal 40 - End of game required items: Soul Blade / Phoenix
          (mostly to ensure that they don't get into potentially inaccessible areas) */
        GoalList[40].InsertElement(GoalType::ITEM, CHEST_SOUL_BLADE);
        GoalList[40].InsertElement(GoalType::ITEM, ITEM_MOUNTAIN_KING);
        GoalList[40].Target = 41;

        /* Goal 41 - Tool Shop Owner */
        GoalList[41].InsertElement(GoalType::LAIR, NPC_TOOL_SHOP_OWNER);
        GoalList[41].Target = 42;

        /* Goal 42 - Tool Shop Owner's son Teddy */
        GoalList[42].InsertElement(GoalType::LAIR, NPC_TEDDY);
        GoalList[42].Target = 43;

        /* Goal 43 - Emblem A tile */
        GoalList[43].InsertElement(GoalType::LAIR, NPC_BRIDGE_GUARD);
        GoalList[43].InsertElement(GoalType::LAIR, NPC_WATER_MILL);
        GoalList[43].InsertElement(GoalType::LAIR, NPC_IVY);
        GoalList[43].InsertElement(GoalType::LAIR, NPC_IVY_EMBLEM_A);
        GoalList[43].Target = 44;

        /* Goal 44 - Secret Cave pass */
        GoalList[44].InsertElement(GoalType::LAIR, NPC_BRIDGE_GUARD);
        GoalList[44].InsertElement(GoalType::LAIR, NPC_IVY);
        GoalList[44].InsertElement(GoalType::LAIR, NPC_TULIP_PASS);
        GoalList[44].Target = 45;

        /* Goal 45 - Goat pen tile */
        GoalList[45].InsertElement(GoalType::LAIR, NPC_BRIDGE_GUARD);
        GoalList[45].InsertElement(GoalType::LAIR, NPC_GOAT_HERB);
        GoalList[45].Target = 46;

        /* Goal 46 - Secret Cave access */
        GoalList[46].InsertElement(GoalType::LAIR, NPC_BRIDGE_GUARD);
        GoalList[46].InsertElement(GoalType::LAIR, NPC_BOY_CAVE);
        GoalList[46].InsertElement(GoalType::ITEM, ITEM_PASS);
        GoalList[46].Target = 47;

        /* Goal 47 - Recovery Sword crystal */
        GoalList[47].InsertElement(GoalType::LAIR, NPC_IVY_RECOVERY_SWORD);
        GoalList[47].Target = 48;

        /* Goal 48 - Red-Hot Mirror bird */
        GoalList[48].InsertElement(GoalType::LAIR, NPC_BIRD_RED_HOT_MIRROR);
        GoalList[48].Target = 49;

        /* Goal 49 - Magic Bell crystal */
        GoalList[49].InsertElement(GoalType::LAIR, NPC_DEER_MAGIC_BELL);
        GoalList[49].InsertElement(GoalType::LAIR, NPC_CROCODILE3);
        GoalList[49].InsertElement(GoalType::ITEM, ITEM_EMBLEM_A);
        GoalList[49].InsertElement(GoalType::ITEM, CHEST_EMBLEM_B);
        GoalList[49].InsertElement(GoalType::ITEM, ITEM_SQUIRREL_EMBLEM_C);
        GoalList[49].InsertElement(GoalType::ITEM, CHEST_EMBLEM_D);
        GoalList[49].InsertElement(GoalType::ITEM, ITEM_SNAIL_EMBLEM_E);
        GoalList[49].InsertElement(GoalType::ITEM, ITEM_EMBLEM_F);
        GoalList[49].InsertElement(GoalType::ITEM, ITEM_EMBLEM_G);
        GoalList[49].InsertElement(GoalType::ITEM, ITEM_EMBLEM_H);
        GoalList[49].Target = 50;

        /* Goal 50 - Woodstin Trio */
        GoalList[50].InsertElement(GoalType::LAIR, NPC_DEER);
        GoalList[50].InsertElement(GoalType::LAIR, NPC_SQUIRREL3);
        GoalList[50].InsertElement(GoalType::LAIR, NPC_DOG3);
        GoalList[50].Target = 51;

        /* Goal 51 - Shield Bracelet mole */
        GoalList[51].InsertElement(GoalType::LAIR, NPC_MOLE);
        GoalList[51].InsertElement(GoalType::LAIR, NPC_MOLE_SHIELD_BRACELET);
        GoalList[51].InsertElement(GoalType::ITEM, CHEST_MOLES_RIBBON);
        GoalList[51].Target = 52;

        /* Goal 52 - Psycho Sword squirrel */
        GoalList[52].InsertElement(GoalType::LAIR, NPC_SQUIRREL_PSYCHO_SWORD);
        GoalList[52].InsertElement(GoalType::ITEM, CHEST_DELICIOUS_SEEDS);
        GoalList[52].Target = 53;

        /* Goal 53 - Red-Hot Mirror bird */
        GoalList[53].InsertElement(GoalType::LAIR, NPC_SQUIRREL_EMBLEM_C);
        GoalList[53].Target = 54;

        /* Goal 54 - Medical Herb mermaid (north-eastern house of St Elles) */
        GoalList[54].InsertElement(GoalType::LAIR, NPC_DOLPHIN2);
        GoalList[54].InsertElement(GoalType::LAIR, NPC_MERMAID);
        GoalList[54].Target = 55;

        /* Goal 55 - Common Mermaid house */
        GoalList[55].InsertElement(GoalType::LAIR, NPC_MERMAID_BUBBLE_ARMOR);
        GoalList[55].Target = 56;

        /* Goal 56 - Magic Flare mermaid */
        GoalList[56].InsertElement(GoalType::LAIR, NPC_MERMAID_MAGIC_FLARE);
        GoalList[56].Target = 57;

        /* Goal 57 - Red-Hot Stick mermaid */
        GoalList[57].InsertElement(GoalType::LAIR, NPC_MERMAID_RED_HOT_STICK);
        GoalList[57].Target = 58;

        /* Goal 58 - Lue */
        GoalList[58].InsertElement(GoalType::LAIR, NPC_MERMAID_PEARL);
        GoalList[58].InsertElement(GoalType::LAIR, NPC_LUE);
        GoalList[58].InsertElement(GoalType::LAIR, NPC_DOLPHIN_SAVES_LUE);
        GoalList[58].Target = 59;

        /* Goal 59 - Emblem E snail */
        GoalList[59].InsertElement(GoalType::LAIR, NPC_SNAIL_EMBLEM_E);
        GoalList[59].Target = 60;

        /* Goal 60 - Mushroom Shoes boy */
        GoalList[60].InsertElement(GoalType::LAIR, NPC_BOY_MUSHROOM_SHOES);
        GoalList[60].Target = 61;

        /* Goal 61 - Mountain King's item + get Deathtoll to appear */
        GoalList[61].InsertElement(GoalType::LAIR, NPC_BOY_MUSHROOM_SHOES);
        GoalList[61].InsertElement(GoalType::LAIR, NPC_BOY);
        GoalList[61].InsertElement(GoalType::LAIR, NPC_GRANDPA3);
        GoalList[61].InsertElement(GoalType::LAIR, NPC_DANCING_GRANDMA);
        GoalList[61].InsertElement(GoalType::LAIR, NPC_DANCING_GRANDMA2);
        GoalList[61].InsertElement(GoalType::LAIR, NPC_MOUNTAIN_KING);
        GoalList[61].InsertElement(GoalType::ITEM, CHEST_RED_HOT_BALL);
        GoalList[61].InsertElement(GoalType::ITEM, ITEM_BIRD_RED_HOT_MIRROR);
        GoalList[61].InsertElement(GoalType::ITEM, ITEM_MERMAID_RED_HOT_STICK);
        GoalList[61].Target = 62;

        /* Goal 62 - Locked dining room */
        GoalList[62].InsertElement(GoalType::LAIR, NPC_GREAT_DOOR);
        GoalList[62].InsertElement(GoalType::LAIR, NPC_CHEST_OF_DRAWERS_MYSTIC_ARMOR);
        GoalList[62].InsertElement(GoalType::ITEM, ITEM_CAT_DOOR_KEY);
        GoalList[62].Target = 63;

        /* Goal 63 - Spark Magic mouse */
        GoalList[63].InsertElement(GoalType::LAIR, NPC_CAT);
        GoalList[63].InsertElement(GoalType::LAIR, NPC_CAT2);
        GoalList[63].InsertElement(GoalType::LAIR, NPC_MOUSE);
        GoalList[63].InsertElement(GoalType::LAIR, NPC_MOUSE_SPARK_BOMB);
        GoalList[63].InsertElement(GoalType::ITEM, ITEM_PLANT_ACTINIDIA_LEAVES);
        GoalList[63].Target = 64;

        /* Goal 64 - Medical Herb plant */
        GoalList[64].InsertElement(GoalType::LAIR, NPC_CAT);
        GoalList[64].InsertElement(GoalType::LAIR, NPC_CAT2);
        GoalList[64].InsertElement(GoalType::LAIR, NPC_MOUSE);
        GoalList[64].InsertElement(GoalType::LAIR, NPC_PLANT_HERB);
        GoalList[64].InsertElement(GoalType::ITEM, ITEM_PLANT_ACTINIDIA_LEAVES);
        GoalList[64].Target = 65;

        /* Goal 65 - Model Town room access */
        GoalList[65].InsertElement(GoalType::LAIR, NPC_STEPS_UPSTAIRS);
        GoalList[65].InsertElement(GoalType::LAIR, NPC_GREAT_DOOR_MODEL_TOWNS);
        GoalList[65].Target = 66;

        /* Goal 66 - Leo's Cat */
        GoalList[66].InsertElement(GoalType::LAIR, NPC_CAT_DOOR_KEY);
        GoalList[66].InsertElement(GoalType::ITEM, CHEST_DREAM_ROD);
        GoalList[66].Target = 67;

        /* Goal 67 - Actinidia Plant */
        GoalList[67].InsertElement(GoalType::LAIR, NPC_PLANT_ACTINIDIA_LEAVES);
        GoalList[67].Target = 68;

        /* Goal 68 - Elemental Mail soldier */
        GoalList[68].InsertElement(GoalType::LAIR, NPC_SOLDIER_ELEMENTAL_MAIL);
        GoalList[68].InsertElement(GoalType::ITEM, CHEST_DREAM_ROD);
        GoalList[68].Target = 69;

        /* Goal 69 - Queen Magridd */
        GoalList[69].InsertElement(GoalType::LAIR, NPC_QUEEN_MAGRIDD);
        GoalList[69].Target = 70;

        /* Goal 70 - Platinum Card soldier */
        GoalList[70].InsertElement(GoalType::LAIR, NPC_SINGER_CONCERT_HALL);
        GoalList[70].InsertElement(GoalType::LAIR, NPC_SOLDIER_PLATINUM_CARD);
        GoalList[70].InsertElement(GoalType::ITEM, ITEM_HARP_STRING);
        GoalList[70].Target = 71;

        /* Goal 71 - Medical Herb maid */
        GoalList[71].InsertElement(GoalType::LAIR, NPC_MAID_HERB);
        GoalList[71].Target = 72;

        /* Goal 72 - Magridd Castle + Emblem H tile */
        GoalList[72].InsertElement(GoalType::LAIR, NPC_SOLDIER_CASTLE);
        GoalList[72].Target = 73;

        /* Goal 73 - Super Bracelet tile */
        GoalList[73].InsertElement(GoalType::LAIR, NPC_QUEEN_MAGRIDD);
        GoalList[73].Target = 74;

        /* Goal 74 - Greenwood Leaf tile */
        GoalList[74].InsertElement(GoalType::LAIR, NPC_CROCODILE);
        GoalList[74].InsertElement(GoalType::LAIR, NPC_CROCODILE2);
        GoalList[74].InsertElement(GoalType::LAIR, NPC_BIRD_GREENWOOD_LEAF);
        GoalList[74].InsertElement(GoalType::LAIR, NPC_MOLE_SOUL_OF_LIGHT);
        GoalList[74].InsertElement(GoalType::ITEM, CHEST_DREAM_ROD);
        GoalList[74].Target = 75;

        /* Goal 75 - Demon Bird */
        GoalList[75].InsertElement(GoalType::ITEM, ITEM_DR_LEO);
        GoalList[75].Target = 76;

        /* Goal 76 - Leo's Lab Attic */
        GoalList[76].InsertElement(GoalType::LAIR, NPC_STEPS_MARIE);
        GoalList[76].Target = 77;

        /* Goal 77 - Chest of Drawers in the Attic */
        GoalList[77].InsertElement(GoalType::LAIR, NPC_CHEST_OF_DRAWERS2);
        GoalList[77].Target = 78;
    }


    void CalculateWeights(std::vector<Region> &RegionList, std::vector<Goal> &GoalList, int GoalID) {

        int TargetRegionIndex = (GoalID == GOAL_TO_FIRST_REGION ? 0 : GoalList[GoalID].Target);

        /* If this region doesn't have any goal, the goal leading to it only has a weight of 1 */
        if (RegionList[TargetRegionIndex].NextGoals.empty()) {
            if (GoalID != GOAL_TO_FIRST_REGION) {
                GoalList[GoalID].Weight = 1;
            }
            return;
        }

        int MaximumDepth = 1;
        for (const auto& CurrentSubGoalID : RegionList[TargetRegionIndex].NextGoals) {
            /* Calculate weights of all sub-goals, and keep the maximum depth found */
            CalculateWeights(RegionList, GoalList, CurrentSubGoalID);
            if (GoalList[CurrentSubGoalID].Weight > MaximumDepth) {
                MaximumDepth = GoalList[CurrentSubGoalID].Weight;
            }
        }

        if (GoalID != GOAL_TO_FIRST_REGION) {
            /* This goal's weight will be the maximum depth of the attached sub-tree + 1 */
            GoalList[GoalID].Weight = MaximumDepth + 1;

            /* Cap the weight */
            if (WEIGHT_CAP != 0 && GoalList[GoalID].Weight > WEIGHT_CAP) {
                GoalList[GoalID].Weight = WEIGHT_CAP;
            }
        }
    }


    void GetNonKeyNPCList(std::vector<int> &NonKeyNPCList) {
        NonKeyNPCList.push_back(NPC_TULIP);
        NonKeyNPCList.push_back(NPC_TULIP2);
        NonKeyNPCList.push_back(NPC_GOAT);
        NonKeyNPCList.push_back(NPC_TULIP3);
        NonKeyNPCList.push_back(NPC_LONELY_GOAT);
        NonKeyNPCList.push_back(NPC_BOY_CABIN);
        NonKeyNPCList.push_back(NPC_OLD_MAN);
        NonKeyNPCList.push_back(NPC_OLD_MAN2);
        NonKeyNPCList.push_back(NPC_IVY2);
        NonKeyNPCList.push_back(NPC_TULIP4);
        NonKeyNPCList.push_back(NPC_GOAT2);
        NonKeyNPCList.push_back(NPC_BIRD);
        NonKeyNPCList.push_back(NPC_DOG);
        NonKeyNPCList.push_back(NPC_DOG2);
        NonKeyNPCList.push_back(NPC_BIRD2);
        NonKeyNPCList.push_back(NPC_SQUIRREL);
        NonKeyNPCList.push_back(NPC_DOG4);
        NonKeyNPCList.push_back(NPC_SQUIRREL2);
        NonKeyNPCList.push_back(NPC_DOG5);
        NonKeyNPCList.push_back(NPC_MOLE2);
        NonKeyNPCList.push_back(NPC_BIRD3);
        NonKeyNPCList.push_back(NPC_DOLPHIN);
        NonKeyNPCList.push_back(NPC_ANGELFISH);
        NonKeyNPCList.push_back(NPC_ANGELFISH2);
        NonKeyNPCList.push_back(NPC_MERMAID2);
        NonKeyNPCList.push_back(NPC_MERMAID3);
        NonKeyNPCList.push_back(NPC_MERMAID_NANA);
        NonKeyNPCList.push_back(NPC_MERMAID4);
        NonKeyNPCList.push_back(NPC_MERMAID5);
        NonKeyNPCList.push_back(NPC_MERMAID6);
        NonKeyNPCList.push_back(NPC_MERMAID_TEARS);
        NonKeyNPCList.push_back(NPC_ANGELFISH3);
        NonKeyNPCList.push_back(NPC_ANGELFISH_SOUL_OF_SHIELD);
        NonKeyNPCList.push_back(NPC_MERMAID7);
        NonKeyNPCList.push_back(NPC_ANGELFISH4);
        NonKeyNPCList.push_back(NPC_MERMAID8);
        NonKeyNPCList.push_back(NPC_MERMAID9);
        NonKeyNPCList.push_back(NPC_GRANDPA);
        NonKeyNPCList.push_back(NPC_GIRL);
        NonKeyNPCList.push_back(NPC_MUSHROOM);
        NonKeyNPCList.push_back(NPC_GRANDPA2);
        NonKeyNPCList.push_back(NPC_SNAIL_JOCKEY);
        NonKeyNPCList.push_back(NPC_BOY2);
        NonKeyNPCList.push_back(NPC_GRANDMA);
        NonKeyNPCList.push_back(NPC_GIRL2);
        NonKeyNPCList.push_back(NPC_SNAIL_RACER);
        NonKeyNPCList.push_back(NPC_SNAIL_RACER2);
        NonKeyNPCList.push_back(NPC_MUSHROOM3);
        NonKeyNPCList.push_back(NPC_SNAIL);
        NonKeyNPCList.push_back(NPC_SNAIL2);
        NonKeyNPCList.push_back(NPC_PLANT);
        NonKeyNPCList.push_back(NPC_CAT3);
        NonKeyNPCList.push_back(NPC_DOLL);
        NonKeyNPCList.push_back(NPC_CHEST_OF_DRAWERS);
        NonKeyNPCList.push_back(NPC_PLANT2);
        NonKeyNPCList.push_back(NPC_MOUSE2);
        NonKeyNPCList.push_back(NPC_MOUSE3);
        NonKeyNPCList.push_back(NPC_GREAT_DOOR_SOUL_OF_DETECTION);
        NonKeyNPCList.push_back(NPC_MOUSE4);
        NonKeyNPCList.push_back(NPC_MOUSE5);
        NonKeyNPCList.push_back(NPC_CAT4);
        NonKeyNPCList.push_back(NPC_SOLDIER);
        NonKeyNPCList.push_back(NPC_SOLDIER2);
        NonKeyNPCList.push_back(NPC_SOLDIER3);
        NonKeyNPCList.push_back(NPC_SOLDIER4);
        NonKeyNPCList.push_back(NPC_SOLDIER5);
        NonKeyNPCList.push_back(NPC_SOLDIER6);
        NonKeyNPCList.push_back(NPC_MAID);
        NonKeyNPCList.push_back(NPC_SINGER);
        NonKeyNPCList.push_back(NPC_SOLDIER_SOUL_OF_REALITY);
        NonKeyNPCList.push_back(NPC_MAID2);
        NonKeyNPCList.push_back(NPC_SOLDIER7);
        NonKeyNPCList.push_back(NPC_SOLDIER8);
        NonKeyNPCList.push_back(NPC_SOLDIER9);
        NonKeyNPCList.push_back(NPC_SOLDIER10);
        NonKeyNPCList.push_back(NPC_SOLDIER11);
    }


    void GetNonKeyItemList(std::vector<int> &NonKeyChestList) {
        NonKeyChestList.push_back(1);
        NonKeyChestList.push_back(2);
        NonKeyChestList.push_back(3);
        NonKeyChestList.push_back(4);
        NonKeyChestList.push_back(7);
        NonKeyChestList.push_back(8);
        NonKeyChestList.push_back(9);
        NonKeyChestList.push_back(11);
        NonKeyChestList.push_back(12);
        NonKeyChestList.push_back(13);
        NonKeyChestList.push_back(15);
        NonKeyChestList.push_back(16);
        NonKeyChestList.push_back(17);
        NonKeyChestList.push_back(18);
        NonKeyChestList.push_back(19);
        NonKeyChestList.push_back(20);
        NonKeyChestList.push_back(21);
        NonKeyChestList.push_back(22);
        NonKeyChestList.push_back(25);
        NonKeyChestList.push_back(26);
        NonKeyChestList.push_back(27);
        NonKeyChestList.push_back(28);
        NonKeyChestList.push_back(29);
        NonKeyChestList.push_back(30);
        NonKeyChestList.push_back(31);
        NonKeyChestList.push_back(32);
        NonKeyChestList.push_back(33);
        NonKeyChestList.push_back(34);
        NonKeyChestList.push_back(35);
        NonKeyChestList.push_back(36);
        NonKeyChestList.push_back(37);
        NonKeyChestList.push_back(38);
        NonKeyChestList.push_back(39);
        NonKeyChestList.push_back(41);
        NonKeyChestList.push_back(42);
        NonKeyChestList.push_back(44);
        NonKeyChestList.push_back(45);
        NonKeyChestList.push_back(46);
        NonKeyChestList.push_back(47);
        NonKeyChestList.push_back(48);
        NonKeyChestList.push_back(49);
        NonKeyChestList.push_back(50);
        NonKeyChestList.push_back(52);
        NonKeyChestList.push_back(53);
        NonKeyChestList.push_back(54);
        NonKeyChestList.push_back(56);
        NonKeyChestList.push_back(57);
        NonKeyChestList.push_back(58);
        NonKeyChestList.push_back(59);
        NonKeyChestList.push_back(60);
        NonKeyChestList.push_back(61);
        NonKeyChestList.push_back(65);
        NonKeyChestList.push_back(ITEM_TOOL_SHOP_OWNER);
        NonKeyChestList.push_back(ITEM_GOAT_PEN);
        NonKeyChestList.push_back(ITEM_TEDDY);
        NonKeyChestList.push_back(ITEM_SECRET_CAVE_TILE);
        //NonKeyChestList.push_back(ITEM_VILLAGE_CHIEF);
        NonKeyChestList.push_back(ITEM_MAGICIAN);
        NonKeyChestList.push_back(ITEM_CRYSTAL_RECOVERY_SWORD);
        NonKeyChestList.push_back(ITEM_CRYSTAL_MAGIC_BELL);
        NonKeyChestList.push_back(ITEM_WOODSTIN_TRIO);
        //NonKeyChestList.push_back(ITEM_GREENWOODS_GUARDIAN);
        NonKeyChestList.push_back(ITEM_MOLE_SHIELD_BRACELET);
        NonKeyChestList.push_back(ITEM_SQUIRREL_PSYCHO_SWORD);
        NonKeyChestList.push_back(ITEM_WATER_SHRINE_TILE);
        NonKeyChestList.push_back(ITEM_CRYSTAL_LIGHT_ARROW);
        //NonKeyChestList.push_back(ITEM_NOME);
        NonKeyChestList.push_back(ITEM_CHEST_OF_DRAWERS_MYSTIC_ARMOR);
        NonKeyChestList.push_back(ITEM_PLANT_HERB);
        NonKeyChestList.push_back(ITEM_CHEST_OF_DRAWERS_HERB);
        //NonKeyChestList.push_back(ITEM_MARIE);
        NonKeyChestList.push_back(ITEM_MOUSE_SPARK_BOMB);
        NonKeyChestList.push_back(ITEM_SOLDIER_ELEMENTAL_MAIL);
        NonKeyChestList.push_back(ITEM_SUPER_BRACELET);
        NonKeyChestList.push_back(ITEM_MAID_HERB);
        //NonKeyChestList.push_back(ITEM_KING_MAGRIDD);
        NonKeyChestList.push_back(ITEM_MERMAID_HERB);
        NonKeyChestList.push_back(ITEM_MERMAID_MAGIC_FLARE);
        //NonKeyChestList.push_back(ITEM_MERMAID_QUEEN);
        NonKeyChestList.push_back(ITEM_CRYSTAL_GRASS_VALLEY);
        NonKeyChestList.push_back(ITEM_CRYSTAL_UNDERGROUND_CASTLE);
        NonKeyChestList.push_back(ITEM_CRYSTAL_LOST_MARSH);
        NonKeyChestList.push_back(ITEM_CRYSTAL_WATER_SHRINE);
        NonKeyChestList.push_back(ITEM_CRYSTAL_FIRE_SHRINE);
        NonKeyChestList.push_back(ITEM_CRYSTAL_MOUNTAIN_OF_SOULS);
        NonKeyChestList.push_back(ITEM_CRYSTAL_LUNE);
        NonKeyChestList.push_back(ITEM_CRYSTAL_LEOS_LAB_BASEMENT);
        NonKeyChestList.push_back(ITEM_CRYSTAL_MODEL_TOWN);
        NonKeyChestList.push_back(ITEM_CRYSTAL_POWER_PLANT);
        NonKeyChestList.push_back(ITEM_CRYSTAL_ROCKBIRD);
        NonKeyChestList.push_back(ITEM_CRYSTAL_SEABED_NEAR_BLESTER);
        NonKeyChestList.push_back(ITEM_CRYSTAL_SEABED_NEAR_DUREAN);
    }
}
