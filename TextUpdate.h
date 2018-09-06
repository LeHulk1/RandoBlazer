#ifndef __TEXTUPDATE_H__
#define __TEXTUPDATE_H__

#include <fstream>
#include <vector>


using namespace std;

namespace ROMUpdate {
    int ConvertToHex(int Dec);
    void NPCTextUpdateMain(vector<Lair> RandomizedLairList,
                           vector<Item> RandomizedItemList,
                           fstream &ROMFile,
                           long Seed);
    void GeneralTextUpdate(vector<Lair> RandomizedLairList,
                           vector<Item> RandomizedItemList,
                           fstream &ROMFile,
                           long Seed);
    void NPCItemTextUpdate(int ItemIndex, int ItemID, fstream &ROMFile);
}

#endif // __TEXTUPDATE_H__
