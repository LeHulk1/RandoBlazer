#ifndef __TEXTUPDATE_H__
#define __TEXTUPDATE_H__

#include <fstream>
#include <vector>

namespace ROMUpdate {
    int ConvertToHex(int Dec);
    void NPCTextUpdateMain(const std::vector<Lair>& RandomizedLairList,
                           const std::vector<Item>& RandomizedItemList,
                           std::fstream &ROMFile,
                           long Seed);
    void GeneralTextUpdate(const std::vector<Lair>& RandomizedLairList,
                           const std::vector<Item>& RandomizedItemList,
                           std::fstream &ROMFile,
                           long Seed);
    void NPCItemTextUpdate(int ItemIndex, int ItemID, std::fstream &ROMFile);
}

#endif // __TEXTUPDATE_H__
