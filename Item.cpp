#include "Item.h"

Item::Item() {
    Type     = TYPE_CHEST;
    Contents = NOTHING;
    Gems     = 0;
    Exp      = 0;
}

Item::~Item() {}


Item& Item::operator= (const Item& OtherItemHolder) {
    if (this != &OtherItemHolder) {
        Type     = OtherItemHolder.Type;
        Contents = OtherItemHolder.Contents;
        Gems     = OtherItemHolder.Gems;
        Exp      = OtherItemHolder.Exp;
    }
    return *this;
}

