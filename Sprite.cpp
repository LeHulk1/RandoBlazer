#include "Lair.h"
#include "Sprite.h"


Sprite::Sprite() {
    Address     = 0x00;
    Act         = ACT_1;
    x           = 0;
    y           = 0;
    Orientation = 0x01;
    Enemy       = NO_ENEMY;
}

Sprite::~Sprite() {}
