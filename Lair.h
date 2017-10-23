#ifndef __LAIR_H__
#define __LAIR_H__

#define POSITION_DATA_SIZE  3
#define LAIR_TYPE_SIZE      2


enum {
    ACT_1 = 0,
    ACT_2,
    ACT_3,
    ACT_4,
    ACT_5,
    ACT_6,
    ACT_7
};

typedef enum {
    LAIR_ONE_BY_ONE      = 0xF3,
    LAIR_MULTISPAWN      = 0x1B,
    LAIR_ONE_BY_ONE_PROX = 0x13,
    LAIR_TWO_UP_TWO_DOWN = 0xD2,
    LAIR_ALREADY_THERE   = 0x52
} LairType;

enum {
    LAIR_ONE_BY_ONE_SECOND_BYTE      = 0xA6,
    LAIR_MULTISPAWN_SECOND_BYTE      = 0xA7,
    LAIR_ONE_BY_ONE_PROX_SECOND_BYTE = 0xA8,
    LAIR_TWO_UP_TWO_DOWN_SECOND_BYTE = 0xA7,
    LAIR_ALREADY_THERE_SECOND_BYTE   = 0xA7,
};

typedef enum {
    NO_ENEMY           = 0x00,
    SOLID_ARM          = 0x1F,
    ACT1_GOBLIN        = 0x20,
    ACT1_IMP           = 0x21,
    ACT1_FLY           = 0x22,
    ACT1_PLANT         = 0x23,
    ACT1_SLIME         = 0x24,
    ACT1_TORCH         = 0x25,
    ACT1_ARMOR         = 0x26,
    ACT1_SPIKEY        = 0x27,
    ACT1_BIRD          = 0x28,
    ACT1_TORCH2        = 0x29,
    ACT1_BLOCK         = 0x2E,
    ACT2_WATER_DRAGON  = 0x2C,
    ACT2_MUDMAN        = 0x2D,
    ACT2_BUSH          = 0x2E,
    ACT2_STATUE        = 0x2F,
    ACT2_FIRE_SPIRIT   = 0x33,
    ACT2_GHOST         = 0x35,
    ACT2_LIZARDMAN     = 0x37,
    ACT2_TP_LIZARDMAN  = 0x38,
    ELEMENTAL_STATUE   = 0x3D,
    ACT2_FIREMAN       = 0x48,
    ACT2_SCORPION      = 0x49,
    ACT3_URCHIN        = 0x32,
    ACT3_JELLYFISH     = 0x34,
    ACT3_CRAB          = 0x35,
    ACT3_RAY           = 0x36,
    ACT3_SEAHORSE      = 0x37,
    ACT3_PALM_TREE     = 0x3A,
    ACT3_ROCK          = 0x3B,
    ACT3_FISH          = 0x3C,
    ACT3_GORILLA       = 0x3D,
    ACT3_METAL_GORILLA = 0x3E,
    ACT3_EAGLE         = 0x3F,
    FLOATING_SKULL     = 0x41,
    ACT4_RAT           = 0x2D,
    ACT4_MOOSE         = 0x2E,
    ACT4_YETI          = 0x2F,
    ACT4_BAT           = 0x30,
    ACT4_SNOWBALL      = 0x31,
    ACT4_PURPLE_WIZARD = 0x32,
    ACT4_RED_WIZARD    = 0x33,
    ACT4_ICE_HEAD      = 0x34,
    ACT4_CIRCLING_BAT  = 0x36,
    ACT4_SLIME         = 0x37,
    POSEIDON           = 0x38,
    TIN_DOLL           = 0x1E,
    ACT5_METAL_MOUSE   = 0x1F,
    ACT5_BULLDOZER     = 0x20,
    ACT5_HELICOPTER    = 0x21,
    ACT5_WORM          = 0x22,
    ACT5_ROBOT         = 0x24,
    ACT5_MINI_KNIGHT   = 0x25,
    ACT5_MINI_ARCHER   = 0x26,
    ACT5_MINI_HORSEMAN = 0x27,
    ACT5_CATAPULT      = 0x28,
    ACT5_TOWER         = 0x29,
    DEMON_BIRD         = 0x09,
    ACT6_ORB           = 0x0C,
    ACT6_GHOST         = 0x0D,
    ACT6_SNAKE         = 0x0E,
    ACT6_SKELETON      = 0x10,
    ACT6_PURPLE_KNIGHT = 0x11,
    ACT6_RED_KNIGHT    = 0x12,
    ACT6_MIMIC         = 0x15,
    ACT6_DOLL          = 0x16,
    ACT6_CHESS_KNIGHT  = 0x17,
    ACT7_DEMON         = 0x0A,
    ACT7_FLY           = 0x0B,
    ACT7_BRICK         = 0x0C
} EnemyType;


class Lair {
public:
    Lair();
    ~Lair();

    Lair& operator= (const Lair& OtherLair);
    bool NoFishLairPosition(void);
    bool NoMetalLairPosition(void);
    bool NoGhostLairPosition(void);
    bool MustBeMetalLairPosition(void);
    bool MustBeGhostLairPosition(void);

    unsigned char Act;                              /* 0A */
    unsigned char PositionData[POSITION_DATA_SIZE]; /* from 0B to 0D */
    unsigned char Type[LAIR_TYPE_SIZE];             /* from 10 to 11 */
    unsigned char NbEnemies;                        /* 13 */
    unsigned char SpawnRate;                        /* 14 */
    unsigned char Enemy;                            /* 15 */
    unsigned char UpsideDownFlag;                   /* 17 */
};


#endif // __LAIR_H__
