#ifndef __CHEST_H__
#define __CHEST_H__

#define TYPE_CHEST  0
#define TYPE_NPC    1

typedef enum {
    NOTHING         = 0x00,
    SWORD_OF_LIFE   = 0x01,
    PSYCHO_SWORD    = 0x02,
    CRITICAL_SWORD  = 0x03,
    LUCKY_BLADE     = 0x04,
    ZANTETSU_SWORD  = 0x05,
    SPIRIT_SWORD    = 0x06,
    RECOVERY_SWORD  = 0x07,
    SOUL_BLADE      = 0x08,
    IRON_ARMOR      = 0x09,
    ICE_ARMOR       = 0x0A,
    BUBBLE_ARMOR    = 0x0B,
    MAGIC_ARMOR     = 0x0C,
    MYSTIC_ARMOR    = 0x0D,
    LIGHT_ARMOR     = 0x0E,
    ELEMENTAL_MAIL  = 0x0F,
    SOUL_ARMOR      = 0x10,
    FLAME_BALL      = 0x11,
    LIGHT_ARROW     = 0x12,
    MAGIC_FLARE     = 0x13,
    ROTATOR         = 0x14,
    SPARK_BOMB      = 0x15,
    FLAME_PILLAR    = 0x16,
    TORNADO         = 0x17,
    PHOENIX         = 0x18,
    GOATS_FOOD      = 0x19,
    HARP_STRING     = 0x1A,
    PASS            = 0x1B,
    DREAM_ROD       = 0x1C,
    LEOS_BRUSH      = 0x1D,
    GREENWOOD_LEAF  = 0x1E,
    MOLES_RIBBON    = 0x1F,
    BIG_PEARL       = 0x20,
    MERMAIDS_TEARS  = 0x21,
    MUSHROOM_SHOES  = 0x22,
    MOBILE_KEY      = 0x23,
    THUNDER_RING    = 0x24,
    DELICIOUS_SEEDS = 0x25,
    ACTINIDIA_LEAF  = 0x26,
    DOOR_KEY        = 0x27,
    PLATINUM_CARD   = 0x28,
    VIP_CARD        = 0x29,
    EMBLEM_A        = 0x2A,
    EMBLEM_B        = 0x2B,
    EMBLEM_C        = 0x2C,
    EMBLEM_D        = 0x2D,
    EMBLEM_E        = 0x2E,
    EMBLEM_F        = 0x2F,
    EMBLEM_G        = 0x30,
    EMBLEM_H        = 0x31,
    RED_HOT_MIRROR  = 0x32,
    RED_HOT_BALL    = 0x33,
    RED_HOT_STICK   = 0x34,
    POWER_BRACELET  = 0x35,
    SHIELD_BRACELET = 0x36,
    SUPER_BRACELET  = 0x37,
    MEDICAL_HERB    = 0x38,
    STRANGE_BOTTLE  = 0x39,
    BROWN_STONE     = 0x3A,
    GREEN_STONE     = 0x3B,
    BLUE_STONE      = 0x3C,
    SILVER_STONE    = 0x3D,
    PURPLE_STONE    = 0x3E,
    BLACK_STONE     = 0x3F,
    MAGIC_BELL      = 0x40,
    EXP             = 0xFE,
    GEMS            = 0xFF,
} ItemIndex;

class Item {
public:
    Item();
    ~Item();

    Item& operator= (const Item& OtherChest);
    int Type;
    ItemIndex Contents;
    int Gems;
    int Exp;
};

#endif // __CHEST_H__
