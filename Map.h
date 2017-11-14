#ifndef __MAP_H__
#define __MAP_H__

#include <list>
#include <vector>


#define LAIR 0
#define ITEM 1

#define NUMBER_OF_REGIONS   42
#define NUMBER_OF_GOALS     41

#define NUMBER_OF_NON_KEY_NPCS   110
#define NUMBER_OF_NON_KEY_CHESTS  56


enum {
    CHEST_SWORD_OF_LIFE                = 0,
    CHEST_DREAM_ROD                    = 5,
    CHEST_LEOS_BRUSH                   = 6,
    CHEST_BIG_PEARL                    = 23,
    CHEST_LUCKY_BLADE                  = 40,
    CHEST_ZANTETSU_SWORD               = 43,
    CHEST_SPIRIT_SWORD                 = 51,
    CHEST_RED_HOT_BALL                 = 62,
    CHEST_SOUL_ARMOR                   = 63,
    CHEST_SOUL_BLADE                   = 64,
    ITEM_TOOL_SHOP_OWNER               = 66,
    ITEM_EMBLEM_A                      = 67,
    ITEM_GOAT_PEN                      = 68,
    ITEM_TEDDY                         = 69,
    ITEM_PASS                          = 70,
    ITEM_SECRET_CAVE_TILE              = 71,
    ITEM_VILLAGE_CHIEF                 = 72,
    ITEM_MAGICIAN                      = 73,
    ITEM_CRYSTAL_RECOVERY_SWORD        = 74,
    ITEM_BIRD_RED_HOT_MIRROR           = 75,
    ITEM_CRYSTAL_MAGIC_BELL            = 76,
    ITEM_WOODSTIN_TRIO                 = 77,
    ITEM_GREENWOODS_GUARDIAN           = 78,
    ITEM_GREENWOOD_LEAVES              = 79,
    ITEM_MOLE_SHIELD_BRACELET          = 80,
    ITEM_SQUIRREL_PSYCHO_SWORD         = 81,
    ITEM_SQUIRREL_EMBLEM_C             = 82,
    ITEM_WATER_SHRINE_TILE             = 83,
    ITEM_CRYSTAL_LIGHT_ARROW           = 84,
    ITEM_MOUNTAIN_KING                 = 85,
    ITEM_BOY_MUSHROOM_SHOES            = 86,
    ITEM_NOME                          = 87,
    ITEM_SNAIL_EMBLEM_E                = 88,
    ITEM_EMBLEM_F                      = 89,
    ITEM_EMBLEM_G                      = 90,
    ITEM_CHEST_OF_DRAWERS_MYSTIC_ARMOR = 91,
    ITEM_PLANT_HERB                    = 92,
    ITEM_CAT_DOOR_KEY                  = 93,
    ITEM_PLANT_ACTINIDIA_LEAVES        = 94,
    ITEM_MARIE                         = 95,
    ITEM_MOUSE_SPARK_BOMB              = 96,
    ITEM_SOLDIER_ELEMENTAL_MAIL        = 97,
    ITEM_SUPER_BRACELET                = 98,
    ITEM_QUEEN_MAGRIDD                 = 99,
    ITEM_SOLDIER_PLATINUM_CARD         = 100,
    ITEM_MAID_HERB                     = 101,
    ITEM_EMBLEM_H                      = 102,
    ITEM_KING_MAGRIDD                  = 103,
    ITEM_DR_LEO                        = 104,
    ITEM_HARP_STRING                   = 105,
    ITEM_MERMAID_HERB                  = 106,
    ITEM_MERMAID_BUBBLE_ARMOR          = 107,
    ITEM_MERMAID_MAGIC_FLARE           = 108,
    ITEM_MERMAID_QUEEN                 = 109,
    ITEM_MERMAID_RED_HOT_STICK         = 110,
    ITEM_LUE                           = 111,
    ITEM_CRYSTAL_GRASS_VALLEY          = 112,
    ITEM_CRYSTAL_UNDERGROUND_CASTLE    = 113,
    ITEM_CRYSTAL_LOST_MARSH            = 114,
    ITEM_CRYSTAL_WATER_SHRINE          = 115,
    ITEM_CRYSTAL_MOUNTAIN_OF_SOULS     = 116,
    ITEM_CRYSTAL_LUNE                  = 117,
    ITEM_CRYSTAL_LEOS_LAB_BASEMENT     = 118,
    ITEM_CRYSTAL_MODEL_TOWN            = 119,
    ITEM_CRYSTAL_POWER_PLANT           = 120,
    ITEM_CRYSTAL_ROCKBIRD              = 121,
    ITEM_CRYSTAL_SEABED_NEAR_BLESTER   = 122,
    ITEM_CRYSTAL_SEABED_NEAR_DUREAN    = 123
};

enum {
    NPC_OLD_WOMAN                     = 2,
    NPC_TOOL_SHOP_OWNER               = 6,
    NPC_TULIP                         = 7,
    NPC_BRIDGE_GUARD                  = 8,
    NPC_VILLAGE_CHIEF                 = 9,
    NPC_IVY_CHEST_ROOM                = 13,
    NPC_WATER_MILL                    = 14,
    NPC_GOAT_HERB                     = 15,
    NPC_LISA                          = 16,
    NPC_TULIP2                        = 17,
    NPC_ARCHITECT                     = 18,
    NPC_IVY                           = 19,
    NPC_GOAT                          = 21,
    NPC_TEDDY                         = 22,
    NPC_TULIP3                        = 24,
    NPC_LEOS_HOUSE                    = 26,
    NPC_LONELY_GOAT                   = 29,
    NPC_TULIP_PASS                    = 34,
    NPC_BOY_CABIN                     = 35,
    NPC_BOY_CAVE                      = 37,
    NPC_OLD_MAN                       = 38,
    NPC_OLD_MAN2                      = 39,
    NPC_IVY2                          = 40,
    NPC_IVY_EMBLEM_A                  = 41,
    NPC_IVY_RECOVERY_SWORD            = 42,
    NPC_TULIP4                        = 43,
    NPC_GOAT2                         = 44,
    NPC_BIRD_RED_HOT_MIRROR           = 50,
    NPC_BIRD                          = 51,
    NPC_DOG                           = 55,
    NPC_DOG2                          = 56,
    NPC_DOG3                          = 58,
    NPC_MOLE_SHIELD_BRACELET          = 59,
    NPC_SQUIRREL_EMBLEM_C             = 60,
    NPC_SQUIRREL_PSYCHO_SWORD         = 62,
    NPC_BIRD2                         = 65,
    NPC_MOLE_SOUL_OF_LIGHT            = 68,
    NPC_DEER                          = 69,
    NPC_CROCODILE                     = 73,
    NPC_SQUIRREL                      = 74,
    NPC_GREENWOODS_GUARDIAN           = 75,
    NPC_MOLE                          = 76,
    NPC_DOG4                          = 81,
    NPC_SQUIRREL_ICE_ARMOR            = 83,
    NPC_SQUIRREL2                     = 84,
    NPC_DOG5                          = 85,
    NPC_CROCODILE2                    = 86,
    NPC_MOLE2                         = 87,
    NPC_SQUIRREL3                     = 88,
    NPC_BIRD_GREENWOOD_LEAF           = 92,
    NPC_MOLE3                         = 93,
    NPC_DEER_MAGIC_BELL               = 94,
    NPC_BIRD3                         = 95,
    NPC_CROCODILE3                    = 106,
    NPC_MONMO                         = 109,
    NPC_DOLPHIN                       = 116,
    NPC_ANGELFISH                     = 123,
    NPC_MERMAID                       = 124,
    NPC_ANGELFISH2                    = 126,
    NPC_MERMAID_PEARL                 = 130,
    NPC_MERMAID2                      = 131,
    NPC_DOLPHIN_SAVES_LUE             = 132,
    NPC_MERMAID_STATUE_BLESTER        = 133,
    NPC_MERMAID_RED_HOT_STICK         = 134,
    NPC_LUE                           = 135,
    NPC_MERMAID3                      = 138,
    NPC_MERMAID_NANA                  = 141,
    NPC_MERMAID4                      = 145,
    NPC_DOLPHIN2                      = 147,
    NPC_MERMAID_STATUE_ROCKBIRD       = 149,
    NPC_MERMAID_BUBBLE_ARMOR          = 153,
    NPC_MERMAID5                      = 156,
    NPC_MERMAID6                      = 157,
    NPC_MERMAID_TEARS                 = 159,
    NPC_MERMAID_STATUE_DUREAN         = 163,
    NPC_ANGELFISH3                    = 165,
    NPC_ANGELFISH_SOUL_OF_SHIELD      = 169,
    NPC_MERMAID_MAGIC_FLARE           = 173,
    NPC_MERMAID_QUEEN                 = 174,
    NPC_MERMAID_STATUE_GHOST_SHIP     = 177,
    NPC_DOLPHIN_SECRET_CAVE           = 179,
    NPC_MERMAID7                      = 181,
    NPC_ANGELFISH4                    = 182,
    NPC_MERMAID8                      = 184,
    NPC_DOLPHIN_PEARL                 = 185,
    NPC_MERMAID9                      = 186,
    NPC_GRANDPA                       = 193,
    NPC_GIRL                          = 194,
    NPC_MUSHROOM                      = 195,
    NPC_BOY                           = 196,
    NPC_GRANDPA2                      = 203,
    NPC_SNAIL_JOCKEY                  = 204,
    NPC_NOME                          = 206,
    NPC_BOY2                          = 207,
    NPC_MUSHROOM_EMBLEM_F             = 213,
    NPC_DANCING_GRANDMA               = 217,
    NPC_DANCING_GRANDMA2              = 222,
    NPC_SNAIL_EMBLEM_E                = 223,
    NPC_BOY_MUSHROOM_SHOES            = 224,
    NPC_GRANDMA                       = 225,
    NPC_GIRL2                         = 226,
    NPC_MUSHROOM2                     = 229,
    NPC_SNAIL_RACER                   = 230,
    NPC_SNAIL_RACER2                  = 231,
    NPC_GIRL3                         = 233,
    NPC_MUSHROOM3                     = 237,
    NPC_SNAIL                         = 238,
    NPC_GRANDPA3                      = 239,
    NPC_SNAIL2                        = 241,
    NPC_GRANDPA4                      = 243,
    NPC_GRANDPA_LUNE                  = 245,
    NPC_GRANDPA5                      = 246,
    NPC_MOUNTAIN_KING                 = 250,
    NPC_PLANT_HERB                    = 251,
    NPC_PLANT                         = 253,
    NPC_CHEST_OF_DRAWERS_MYSTIC_ARMOR = 254,
    NPC_CAT                           = 255,
    NPC_GREAT_DOOR_ZANTETSU_SWORD     = 260,
    NPC_CAT2                          = 262,
    NPC_GREAT_DOOR                    = 268,
    NPC_CAT3                          = 269,
    NPC_MODEL_TOWN1                   = 272,
    NPC_GREAT_DOOR_MODEL_TOWNS        = 274,
    NPC_STEPS_UPSTAIRS                = 276,
    NPC_CAT_DOOR_KEY                  = 280,
    NPC_MOUSE                         = 283,
    NPC_MARIE                         = 287,
    NPC_DOLL                          = 294,
    NPC_CHEST_OF_DRAWERS              = 295,
    NPC_PLANT2                        = 297,
    NPC_MOUSE2                        = 299,
    NPC_MOUSE_SPARK_BOMB              = 300,
    NPC_MOUSE3                        = 302,
    NPC_GREAT_DOOR_SOUL_OF_DETECTION  = 306,
    NPC_MODEL_TOWN2                   = 309,
    NPC_MOUSE4                        = 313,
    NPC_STEPS_MARIE                   = 314,
    NPC_CHEST_OF_DRAWERS2             = 315,
    NPC_PLANT_ACTINIDIA_LEAVES        = 316,
    NPC_MOUSE5                        = 321,
    NPC_CAT4                          = 322,
    NPC_STAIRS_POWER_PLANT            = 324,
    NPC_SOLDIER                       = 326,
    NPC_SOLDIER2                      = 327,
    NPC_SOLDIER3                      = 332,
    NPC_SOLDIER_ELEMENTAL_MAIL        = 334,
    NPC_SOLDIER4                      = 335,
    NPC_SOLDIER5                      = 339,
    NPC_SINGER_CONCERT_HALL           = 340,
    NPC_SOLDIER6                      = 341,
    NPC_MAID                          = 344,
    NPC_SOLDIER_LEFT_TOWER            = 346,
    NPC_SOLDIER_DOK                   = 347,
    NPC_SOLDIER_PLATINUM_CARD         = 349,
    NPC_SINGER                        = 351,
    NPC_SOLDIER_SOUL_OF_REALITY       = 358,
    NPC_MAID2                         = 363,
    NPC_QUEEN_MAGRIDD                 = 364,
    NPC_SOLDIER_WITH_LEO              = 366,
    NPC_SOLDIER_RIGHT_TOWER           = 367,
    NPC_DR_LEO                        = 368,
    NPC_SOLDIER7                      = 370,
    NPC_SOLDIER8                      = 371,
    NPC_MAID_HERB                     = 372,
    NPC_SOLDIER_CASTLE                = 377,
    NPC_SOLDIER9                      = 378,
    NPC_SOLDIER10                     = 380,
    NPC_SOLDIER11                     = 383,
    NPC_KING_MAGRIDD                  = 385,
};


using namespace std;

class Element {
public:
    Element(int NewType, int NewIndex);
    ~Element();
    int Type;
    int Index;
};

class Region {
public:
    Region();
    ~Region();
    void InsertElement(int Type, int Index);
    void InsertGoal(int Index);
    list<Element> Contents;
    list<int> NextGoals;
};

class Goal {
public:
    Goal();
    ~Goal();
    void InsertElement(int Type, int Index);
    list<Element> Requirements;
    int Target;
};


namespace Map {
    void InitMap(vector<Region> &RegionList, vector<Goal> &GoalList);
    void InitMap_v2(vector<Region> &RegionList, vector<Goal> &GoalList);
    void GetNonKeyNPCList(vector<int> &NonKeyNPCList);
    void GetNonKeyChestList(vector<int> &NonKeyChestList);
}

#endif // __MAP_H__
