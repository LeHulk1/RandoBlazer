#include "TextUpdate.h"

#include <iostream>
#include <fstream>

#define NB_TEXT_TO_REMOVE_ADDRESSES 6



namespace TextUpdate {

    static int TextToRemoveAddressList[NB_TEXT_TO_REMOVE_ADDRESSES] = {
        0x4EF9,  /* Deathtoll first form text 1 */
        0x4FB7,  /* Deathtoll first form text 2 */
        0x53C7,  /* Deathtoll second form text */
        0x786B,  /* Master's text when hero dies */
        0x78BC,  /* Master's text after Brown Stone */
        0x18644 /* Bridge guard */
    };

    void GeneralTextUpdate(fstream &ROMFile) {

        unsigned char Byte;
            
        /* Text to remove */
//         for (int i=0; i<NB_TEXT_TO_REMOVE_ADDRESSES; ++i) {
//             ROMFile.seekp(TextToRemoveAddressList[i], ios::beg);
//             Byte = 0x00;
//             ROMFile.write((char*)(&Byte), 1);
//             Byte = 0x13;
//             ROMFile.write((char*)(&Byte), 1);
//         }

        /* PUSH START */
        ROMFile.seekp(0x13B2B, ios::beg);
        ROMFile.write("RANDO HYPE", 10);
        
        /* Master's first text */
        ROMFile.seekp(0x7999, ios::beg);
        ROMFile.write("420 Soul blaze it!", 18);
        Byte = 0x13;
        ROMFile.write((char*)(&Byte), 1);
        Byte = 0x52;
        ROMFile.write((char*)(&Byte), 1);
        Byte = 0xFA;
        ROMFile.write((char*)(&Byte), 1);
        ROMFile.seekp(0x7A07, ios::beg);
        Byte = 0x13;
        ROMFile.write((char*)(&Byte), 1);
        Byte = 0x52;
        ROMFile.write((char*)(&Byte), 1);
        Byte = 0xFA;
        ROMFile.write((char*)(&Byte), 1);
        
        /* Old Woman revival text */
        ROMFile.seekp(0x1815A, ios::beg);
        Byte = 0x00;
        ROMFile.write((char*)(&Byte), 1);
        
        /* Magician text 1 */
        ROMFile.seekp(0x1A7FA, ios::beg);
        ROMFile.write("Have this nice item!", 20);
        Byte = 0x13;
        ROMFile.write((char*)(&Byte), 1);
        Byte = 0x88;
        ROMFile.write((char*)(&Byte), 1);
        Byte = 0xB9;
        ROMFile.write((char*)(&Byte), 1);
        
        /* Magician text 2 */
        ROMFile.seekp(0x1A914, ios::beg);
        ROMFile.write("Good luck and/or", 16);
        Byte = 0x0D;
        ROMFile.write((char*)(&Byte), 1);
        ROMFile.write("blame Everhate.", 15);
        Byte = 0x13;
        ROMFile.write((char*)(&Byte), 1);
        Byte = 0x88;
        ROMFile.write((char*)(&Byte), 1);
        Byte = 0xB9;
        ROMFile.write((char*)(&Byte), 1);
        
        /* Tulip next to Underground Castle */
        ROMFile.seekp(0x1AA56, ios::beg);
        Byte = 0x00;
        ROMFile.write((char*)(&Byte), 1);
        
    }


    void NPCItemTextUpdate(int NPCItemIndex, int ItemID, fstream &ROMFile) {

    }

}
