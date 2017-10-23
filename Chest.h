#ifndef __CHEST_H__
#define __CHEST_H__

typedef enum {
    NOTHING         = 0x00,
    GEMS            = 0xFF,
    SWORD_OF_LIFE   = 0x01,
    IRON_ARMOR      = 0x09,
    MEDICAL_HERB    = 0x38,
    DREAM_ROD       = 0x1C,
    LEOS_BRUSH      = 0x1D,
    TORNADO         = 0x17,
    ICE_ARMOR       = 0x0A,
    MOLES_RIBBON    = 0x1F,
    DELICIOUS_SEEDS = 0x25,
    STRANGE_BOTTLE  = 0x39,
    FLAME_PILLAR    = 0x16,
    MERMAIDS_TEARS  = 0x21,
    BIG_PEARL       = 0x20,
    EMBLEM_D        = 0x2D,
    CRITICAL_SWORD  = 0x03,
    POWER_BRACELET  = 0x35,
    MAGIC_ARMOR     = 0x0C,
    LUCKY_BLADE     = 0x04,
    ROTATOR         = 0x14,
    ZANTETSU_SWORD  = 0x05,
    LIGHT_ARMOR     = 0x0E,
    SPIRIT_SWORD    = 0x06,
    EMBLEM_B        = 0x2B,
    RED_HOT_BALL    = 0x33,
    SOUL_ARMOR      = 0x10,
    SOUL_BLADE      = 0x08
} ChestContents;

class Chest {
public:
    Chest();
    ~Chest();

    Chest& operator= (const Chest& OtherChest);
    ChestContents Contents;
    int Gems;
};

#endif // __CHEST_H__
