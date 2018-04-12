#ifndef __TEXTUPDATE_H__
#define __TEXTUPDATE_H__

#include <fstream>
#include <vector>


using namespace std;

namespace TextUpdate {
    void GeneralTextUpdate(fstream &ROMFile);
    void NPCItemTextUpdate(int NPCItemIndex, int ItemID, fstream &ROMFile);
}

#endif // __TEXTUPDATE_H__
