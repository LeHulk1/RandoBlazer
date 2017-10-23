#include "Chest.h"

Chest::Chest() {
    Contents = NOTHING;
    Gems = 0;
}

Chest::~Chest() {}


Chest& Chest::operator= (const Chest& OtherChest) {
    if (this != &OtherChest) {
        Contents = OtherChest.Contents;
        Gems     = OtherChest.Gems;
    }
    return *this;
}

