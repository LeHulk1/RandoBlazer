#include "TextUpdate.h"

#include <iostream>
#include <fstream>

#define NB_TEXT_TO_REMOVE_ADDRESSES 9



namespace TextUpdate {

    static int TextToRemoveAddressList[NB_TEXT_TO_REMOVE_ADDRESSES] = {
        0x4EF9,  /* Deathtoll first form text 1 */
        0x4FB7,  /* Deathtoll first form text 2 */
        0x53C7,  /* Deathtoll second form text */
        0x786B,  /* Master's text when hero dies */
        0x78BC,  /* Master's text after Brown Stone */
        0x7999,  /* Master's first text */
        0x18644, /* Bridge guard */
        0x1A914, /* Magician text 2 */
        0x1AA6B /* Tulip next to Underground Castle */
    };

    void GeneralTextUpdate(fstream &ROMFile) {

        /* Text to remove */
        for (int i=0; i<NB_TEXT_TO_REMOVE_ADDRESSES; ++i) {
            ROMFile.seekp(TextToRemoveAddressList[i], ios::beg);
            unsigned char Byte = 0x00;
            ROMFile.write((char*)(&Byte), 1);
            Byte = 0x13;
            ROMFile.write((char*)(&Byte), 1);
        }

        /* PUSH START */
        ROMFile.seekp(0x13B2B, ios::beg);
        ROMFile.write("RANDO HYPE", 10);
    }


    void NPCItemTextUpdate(int NPCItemIndex, int ItemID, fstream &ROMFile) {

    }

}
