#include "Item.h"

Item::Item() {
    Type     = TYPE_CHEST;
    Contents = NOTHING;
    GemsExp  = 0;
}

Item::~Item() {}


Item& Item::operator= (const Item& OtherItem) {
    if (this != &OtherItem) {
        Type     = OtherItem.Type;
        Contents = OtherItem.Contents;
        GemsExp  = OtherItem.GemsExp;
    }
    return *this;
}

