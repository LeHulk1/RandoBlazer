#ifndef __TEXTUPDATE_H__
#define __TEXTUPDATE_H__

#include <fstream>
#include <vector>


using namespace std;

namespace TextUpdate {
    void GeneralTextUpdate(fstream &ROMFile, long Seed);
    void NPCItemTextUpdate(int ItemIndex, int ItemID, fstream &ROMFile);
}

#endif // __TEXTUPDATE_H__
