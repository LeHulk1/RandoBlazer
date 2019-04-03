#ifndef __RANDOMIZER_H__
#define __RANDOMIZER_H__


#include "Item.h"
#include "Lair.h"
#include "Map.h"
#include "Sprite.h"
#include "ROMCheck.h"

#include <fstream>
#include <string>
#include <vector>

namespace Randomizer {
    struct Options {
	Options() {}
	Options(const std::string& options_string);
        bool race = false;
    };

    ROMStatus CheckFile(const std::string& Filename);
    bool Randomize(const std::string& InFile, const std::string& OutFile, unsigned int seed, const Options& options);

    void RandomizeLairEnemies(Lair &Lair);
    void RandomizeLairType(Lair &Lair);
    void RandomizeLairNbEnemies(Lair &Lair);
    void RandomizeLairSpawnRate(Lair &Lair);
    void RandomizeLairContents(std::vector<Lair> &LairList);
    void RandomizeMapSprites(std::vector<Sprite> &RandomizedSpriteList, std::fstream &ROMFile);

    void ExploreRegion(std::vector<Region>  &RegionList,
                       std::vector<Goal>    &GoalList,
                       std::vector<int>     &AvailableRevivingLairs,
                       std::vector<int>     &AvailableItems,
                       std::vector<int>     &AvailableGoals,
                       bool             RevivedNPCs[],
                       bool             CollectedKeyItems[],
                       int              RegionIndex);

    bool RandomizeProgression(std::vector<Lair>  &RandomizedLairList,
                              std::vector<Item>  &RandomizedItemList,
                              std::fstream       &ROMFile);

    int WeightedGoalChoice(std::vector<Goal>  &GoalList,
                           std::vector<int>   &AvailableGoals);
}

#endif // __RANDOMIZER_H__
