#ifndef __RANDOMIZER_H__
#define __RANDOMIZER_H__


#include "Chest.h"
#include "Lair.h"
#include "Map.h"

#include <fstream>
#include <vector>


using namespace std;

namespace Randomizer {
    void RandomizeLairEnemies(Lair &Lair);
    void RandomizeLairType(Lair &Lair);
    void RandomizeLairNbEnemies(Lair &Lair);
    void RandomizeLairSpawnRate(Lair &Lair);
    void RandomizeLairContents(vector<Lair> &LairList);

    void ExploreRegion(vector<Region>  &RegionList,
                       vector<Goal>    &GoalList,
                       vector<int>     &AvailableRevivingLairs,
                       vector<int>     &AvailableChests,
                       vector<int>     &AvailableGoals,
                       bool             RevivedNPCs[],
                       bool             CollectedKeyItems[],
                       int              RegionIndex);

    bool RandomizeProgression(vector<Lair>  &RandomizedLairList,
                              vector<Chest> &RandomizedChestList,
                              fstream       &ROMFile);
}

#endif // __RANDOMIZER_H__
