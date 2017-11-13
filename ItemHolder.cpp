#include "ItemHolder.h"

ItemHolder::ItemHolder() {
    Type     = TYPE_CHEST;
    Contents = NOTHING;
    Gems     = 0;
    Exp      = 0;
}

ItemHolder::~ItemHolder() {}


ItemHolder& ItemHolder::operator= (const ItemHolder& OtherItemHolder) {
    if (this != &OtherItemHolder) {
        Type     = OtherItemHolder.Type;
        Contents = OtherItemHolder.Contents;
        Gems     = OtherItemHolder.Gems;
        Exp      = OtherItemHolder.Exp;
    }
    return *this;
}

