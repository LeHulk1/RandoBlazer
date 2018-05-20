#include "ROMCheck.h"


ROMStatus CheckOriginalROM(fstream &ROMFile) {

    unsigned char FileStartSample[5];
    ROMFile.seekg(0, ios::beg);
    ROMFile.read((char*)(&(FileStartSample[0])), 1);
    ROMFile.read((char*)(&(FileStartSample[1])), 1);
    ROMFile.read((char*)(&(FileStartSample[2])), 1);
    ROMFile.read((char*)(&(FileStartSample[3])), 1);
    FileStartSample[4] = '\0';

    /* Read first bytes to identify if this ROM is the headered or the un-headered one.
       I should do this with an actual hash or checksum later. */
    if (FileStartSample[0] == 0x78 &&
        FileStartSample[1] == 0xD8 &&
        FileStartSample[2] == 0x18 &&
        FileStartSample[3] == 0xFB) {
        return UNHEADERED;
    }
    else if (FileStartSample[0] == 0x80 &&
             FileStartSample[1] == 0x00 &&
             FileStartSample[2] == 0x00 &&
             FileStartSample[3] == 0x00) {
        return HEADERED;
    }

    return UNKNOWN;
}
