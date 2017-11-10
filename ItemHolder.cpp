#include "ItemHolder.h"

ItemHolder::ItemHolder() {
    Type     = TYPE_CHEST;
    Contents = NOTHING;
    Gems     = 0;
    EXP      = 0;
}

ItemHolder::~ItemHolder() {}


ItemHolder& ItemHolder::operator= (const ItemHolder& OtherChest) {
    if (this != &OtherChest) {
        Type     = OtherChest.Type;
        Contents = OtherChest.Contents;
        Gems     = OtherChest.Gems;
        EXP      = OtherChest.EXP;
    }
    return *this;
}

