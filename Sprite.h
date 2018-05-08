#ifndef __SPRITE_H__
#define __SPRITE_H__


class Sprite {
public:
    Sprite();
    ~Sprite();

    int Address;
    int Act;
    unsigned char x;
    unsigned char y;
    unsigned char Orientation;
    unsigned char Enemy; /* Enemy type */
};

#endif // __SPRITE_H__
