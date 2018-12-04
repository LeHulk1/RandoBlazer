#ifndef __SPRITE_H__
#define __SPRITE_H__


class Sprite {
public:
    Sprite();
    ~Sprite();

    int Address;
    ActID Act;
    unsigned char x;
    unsigned char y;
    unsigned char Orientation;
    EnemyType Enemy;
};

#endif // __SPRITE_H__
