#ifndef __ROMCHECK_H__
#define __ROMCHECK_H__

#include <fstream>


typedef enum {
    UNKNOWN = 0,
    UNHEADERED,
    HEADERED
} ROMStatus;


using namespace std;

ROMStatus CheckOriginalROM(fstream &ROMFile);


#endif // __ROMCHECK_H__
