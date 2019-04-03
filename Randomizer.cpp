#include "Randomizer.h"

#include "Log.h"
#include "Random.h"
#include "ROMData.h"
#include "ROMUpdate.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <cstdio>
#include <boost/algorithm/string.hpp>
#include <boost/uuid/detail/sha1.hpp>

#define HEADER_OFFSET        0x200
#define SEED_SIZE            10
#define MAX_NUMBER_OF_TRIES  5

#define ONE_BY_ONE_WEIGHT       4
#define MULTISPAWN_WEIGHT      15
#define ONE_BY_ONE_PROX_WEIGHT  1

#define NB_ENEMIES_ONE_BY_ONE_MIN  2
#define NB_ENEMIES_ONE_BY_ONE_MAX  6
#define NB_ENEMIES_MULTISPAWN_MIN  4
#define NB_ENEMIES_MULTISPAWN_MAX 12
#define NB_ENEMIES_MULTISPAWN_REDUCED_MIN  4
#define NB_ENEMIES_MULTISPAWN_REDUCED_MAX  8

#define SPAWN_RATE_MIN 0x03
#define SPAWN_RATE_MAX 0x20

//#define DEBUG
//#define DEBUG_NO_ENEMIES
//#define DEBUG_TWO_ENEMIES
//#define DONT_RANDOMIZE

namespace Randomizer {

    static EnemyType UndergroundCastleEnemies[6] =
        {EnemyType::ACT1_GOBLIN, EnemyType::ACT1_IMP, EnemyType::ACT1_FLY, EnemyType::ACT1_PLANT, EnemyType::ACT1_SLIME, EnemyType::ACT1_TORCH};
    static EnemyType LeosPaintingsEnemies[4] =
        {EnemyType::ACT1_ARMOR, EnemyType::ACT1_BIRD, EnemyType::ACT1_TORCH2, EnemyType::ACT1_BLOCK};
    static EnemyType WaterShrineEnemies[5] =
        {EnemyType::ACT2_WATER_DRAGON, EnemyType::ACT2_MUDMAN, EnemyType::ACT2_BUSH, EnemyType::ACT2_STATUE, EnemyType::ACT2_FLOWER};
    static EnemyType FireLightShrineEnemies[5] =
        {EnemyType::ACT2_FIRE_SPIRIT, EnemyType::ACT2_GHOST, EnemyType::ACT2_LIZARDMAN, EnemyType::ACT2_TP_LIZARDMAN, EnemyType::ACT2_FIREMAN};
    static EnemyType FireLightShrineEnemiesNoFireSpirit[4] =
        {EnemyType::ACT2_GHOST, EnemyType::ACT2_LIZARDMAN, EnemyType::ACT2_TP_LIZARDMAN, EnemyType::ACT2_FIREMAN};
    static EnemyType SeabedEnemies[5] =
        {EnemyType::ACT3_URCHIN, EnemyType::ACT3_JELLYFISH, EnemyType::ACT3_CRAB, EnemyType::ACT3_RAY, EnemyType::ACT3_SEAHORSE};
    static EnemyType IslandsEnemies[5] =
        {EnemyType::ACT3_PALM_TREE, EnemyType::ACT3_ROCK, EnemyType::ACT3_FISH, EnemyType::ACT3_GORILLA, EnemyType::ACT3_EAGLE};
    static EnemyType IslandsEnemiesNoFish[4] =
        {EnemyType::ACT3_PALM_TREE, EnemyType::ACT3_ROCK, EnemyType::ACT3_GORILLA, EnemyType::ACT3_EAGLE};
    static EnemyType MountainEnemies[5] =
        {EnemyType::ACT4_RAT, EnemyType::ACT4_MOOSE, EnemyType::ACT4_YETI, EnemyType::ACT4_BAT, EnemyType::ACT4_SNOWBALL};
    static EnemyType MountainEnemiesNoSnowball[4] =
        {EnemyType::ACT4_RAT, EnemyType::ACT4_MOOSE, EnemyType::ACT4_YETI, EnemyType::ACT4_BAT};
    static EnemyType LaynoleLuneEnemies[6] =
        {EnemyType::ACT4_PURPLE_WIZARD, EnemyType::ACT4_RED_WIZARD, EnemyType::ACT4_ICE_HEAD, EnemyType::ACT4_ICE_BLOCK, EnemyType::ACT4_CIRCLING_BAT, EnemyType::ACT4_SLIME};
    static EnemyType LaynoleLuneEnemiesNoIceBlock[5] =
        {EnemyType::ACT4_PURPLE_WIZARD, EnemyType::ACT4_RED_WIZARD, EnemyType::ACT4_ICE_HEAD, EnemyType::ACT4_CIRCLING_BAT, EnemyType::ACT4_SLIME};
    static EnemyType LeosBasementEnemies[5] =
        {EnemyType::ACT5_METAL_MOUSE, EnemyType::ACT5_BULLDOZER, EnemyType::ACT5_HELICOPTER, EnemyType::ACT5_WORM, EnemyType::ACT5_ROBOT};
    static EnemyType LeosBasementEnemiesMetalOnly[3] =
        {EnemyType::ACT5_METAL_MOUSE, EnemyType::ACT5_BULLDOZER, EnemyType::ACT5_HELICOPTER};
    static EnemyType LeosBasementEnemiesNoMetal[2] =
        {EnemyType::ACT5_WORM, EnemyType::ACT5_ROBOT};
    static EnemyType ModelTownsEnemies[5] =
        {EnemyType::ACT5_MINI_KNIGHT, EnemyType::ACT5_MINI_ARCHER, EnemyType::ACT5_MINI_HORSEMAN, EnemyType::ACT5_CATAPULT, EnemyType::ACT5_TOWER};
    static EnemyType CastleBasementEnemies[4] =
        {EnemyType::ACT6_ORB, EnemyType::ACT6_GHOST, EnemyType::ACT6_SNAKE, EnemyType::ACT6_SKELETON};
    static EnemyType CastleBasementEnemiesNoGhost[3] =
        {EnemyType::ACT6_ORB, EnemyType::ACT6_SNAKE, EnemyType::ACT6_SKELETON};
    static EnemyType CastleBasementEnemiesFull[6] =
        {EnemyType::ACT6_SKULL, EnemyType::ACT6_ORB, EnemyType::ACT6_GHOST, EnemyType::ACT6_SNAKE, EnemyType::ACT6_FIRE, EnemyType::ACT6_SKELETON};
    static EnemyType CastleTowersEnemies[5] =
        {EnemyType::ACT6_PURPLE_KNIGHT, EnemyType::ACT6_RED_KNIGHT, EnemyType::ACT6_MIMIC, EnemyType::ACT6_DOLL, EnemyType::ACT6_CHESS_KNIGHT};
    static EnemyType CastleTowersEnemiesFull[7] =
        {EnemyType::ACT6_PURPLE_KNIGHT, EnemyType::ACT6_RED_KNIGHT, EnemyType::ACT6_FIRE2, EnemyType::ACT6_SKULL2, EnemyType::ACT6_MIMIC, EnemyType::ACT6_DOLL, EnemyType::ACT6_CHESS_KNIGHT};
    static EnemyType WorldOfEvilEnemies[3] =
        {EnemyType::ACT7_DEMON, EnemyType::ACT7_FLY, EnemyType::ACT7_BRICK};
    static EnemyType WorldOfEvilEnemiesNoBrick[2] =
        {EnemyType::ACT7_DEMON, EnemyType::ACT7_FLY};

    static unsigned char OrientationList[4] =
        {0x00,  /* down */
         0x40,  /* left */
         0x80,  /* right */
         0xC0}; /* up */

    static bool CanRandomizeOrientation(ActID Act, EnemyType Enemy) {
        return ( (Act == ActID::ACT_2 && Enemy == EnemyType::ACT2_WATER_DRAGON) ||
                 (Act == ActID::ACT_4 && Enemy == EnemyType::ACT4_RAT) ||
                 (Act == ActID::ACT_4 && Enemy == EnemyType::ACT4_SNOWBALL) ||
                 (Act == ActID::ACT_5 && Enemy == EnemyType::ACT5_METAL_MOUSE) ||
                 (Act == ActID::ACT_5 && Enemy == EnemyType::ACT5_ROBOT) ||
                 (Act == ActID::ACT_5 && Enemy == EnemyType::ACT5_WORM) ||
                 (Act == ActID::ACT_5 && Enemy == EnemyType::ACT5_TOWER) ||
                 (Act == ActID::ACT_6 && Enemy == EnemyType::ACT6_SKULL) ||
                 (Act == ActID::ACT_6 && Enemy == EnemyType::ACT6_SNAKE) ||
                 (Act == ActID::ACT_6 && Enemy == EnemyType::ACT6_SKULL2) ||
                 (Act == ActID::ACT_7 && Enemy == EnemyType::ACT7_BRICK) );
    }

    void RandomizeLairEnemies(Lair &Lair) {

        /* Don't randomize enemies from 2-up-2-down lairs, because upside-down enemies can sometimes get away... */
        if (Lair.Type == LairType::LAIR_TWO_UP_TWO_DOWN) {
            return;
        }

        /* A few lairs should not be randomized (yet) */
        if (Lair.MustNotRandomizeLairPosition()) {
            return;
        }

        EnemyType Enemy = Lair.Enemy;
        //int OriginalEnemy = Enemy;

        switch (Lair.Act) {
        case ActID::ACT_1:
            if (Enemy != EnemyType::ACT1_SPIKEY &&
                Enemy != EnemyType::SOLID_ARM) {
                if (Enemy < EnemyType::ACT1_ARMOR) {
                    /* Lair is in Underground Castle */
                    Enemy = UndergroundCastleEnemies[Random::RandomInteger(6)];
                }
                else {
                    /* Lair is in Leo's Paintings */
                    Enemy = LeosPaintingsEnemies[Random::RandomInteger(4)];
                }
            }
            break;
        case ActID::ACT_2:
            if (Enemy != EnemyType::ACT2_SCORPION &&
                Enemy != EnemyType::ACT2_FIRE_SPIRIT &&
                Enemy != EnemyType::ELEMENTAL_STATUE) {
                if (Enemy < EnemyType::ACT2_FIRE_SPIRIT) {
                    /* Lair is in Lost Marsh or Water Shrine */
                    Enemy = WaterShrineEnemies[Random::RandomInteger(5)];
                }
                else {
                    /* Lair is in Fire Shrine or Light Shrine */
                    Enemy = FireLightShrineEnemiesNoFireSpirit[Random::RandomInteger(4)];
                }
            }
            break;
        case ActID::ACT_3:
            if (Enemy != EnemyType::ACT3_METAL_GORILLA &&
                Enemy != EnemyType::FLOATING_SKULL) {
                if (Enemy < EnemyType::ACT3_PALM_TREE) {
                    /* Lair is in Seabed */
                    Enemy = SeabedEnemies[Random::RandomInteger(5)];
                }
                else {
                    /* Lair is in Islands */
                    if (Lair.NoFishLairPosition()) {
                        Enemy = IslandsEnemiesNoFish[Random::RandomInteger(4)];
                    }
                    else {
                        Enemy = IslandsEnemies[Random::RandomInteger(5)];
                    }
                }
            }
            break;
        case ActID::ACT_4:
            if (Enemy != EnemyType::POSEIDON) {
                if (Enemy < EnemyType::ACT4_PURPLE_WIZARD) {
                    /* Lair is in Mountain of Souls */
                    Enemy = MountainEnemiesNoSnowball[Random::RandomInteger(4)];
                }
                else {
                    /* Lair is in Laynole or Lune */
                    Enemy = LaynoleLuneEnemiesNoIceBlock[Random::RandomInteger(5)];
                }
            }
            break;
        case ActID::ACT_5:
            if (Enemy != EnemyType::TIN_DOLL) {
                if (Enemy < EnemyType::ACT5_MINI_KNIGHT) {
                    /* Lair is in Leo's Lab Basement or Power Plant */
                    if (Lair.NoMetalLairPosition()) {
                        Enemy = LeosBasementEnemiesNoMetal[Random::RandomInteger(2)];
                    }
                    else if (Lair.MustBeMetalLairPosition()) {
                        Enemy = LeosBasementEnemiesMetalOnly[Random::RandomInteger(3)];
                    }
                    else {
                        Enemy = LeosBasementEnemies[Random::RandomInteger(5)];
                    }
                }
                else {
                    /* Lair is in one of the Model Towns */
                    Enemy = ModelTownsEnemies[Random::RandomInteger(5)];
                }
            }
            break;
        case ActID::ACT_6:
            if (Enemy != EnemyType::DEMON_BIRD) {
                if (Enemy < EnemyType::ACT6_PURPLE_KNIGHT) {
                    /* Lair is in Magridd Castle Basement */
                    if (Lair.NoGhostLairPosition()) {
                        Enemy = CastleBasementEnemiesNoGhost[Random::RandomInteger(3)];
                    }
                    else if (Lair.MustBeGhostLairPosition()) {
                        Enemy = EnemyType::ACT6_GHOST;
                    }
                    else {
                        Enemy = CastleBasementEnemies[Random::RandomInteger(4)];
                    }
                }
                else {
                    /* Lair is in one of the Magridd Castle Towers */
                    Enemy = CastleTowersEnemies[Random::RandomInteger(5)];
                }
            }
            break;
        case ActID::ACT_7:
            /* Lair is in World of Evil */
            Enemy = WorldOfEvilEnemiesNoBrick[Random::RandomInteger(2)];
            break;
        default:
            /* Should not happen! */
            break;
        }

        /* Update enemy */
        Lair.Enemy = Enemy;

        /* Randomize orientation if possible */
        if (CanRandomizeOrientation(Lair.Act, Enemy)) {
            Lair.Orientation = OrientationList[Random::RandomInteger(4)];
            if (Lair.MustNotBeUpwardsLairPosition() && Lair.Orientation == 0xC0) {
                /* Re-roll until it is not upwards */
                do {
                    Lair.Orientation = OrientationList[Random::RandomInteger(4)];
                } while (Lair.Orientation == 0xC0);
            }
        }
        else {
            Lair.Orientation = 0;
        }
    }


    void RandomizeLairType(Lair &Lair) {

        if (Lair.Type == LairType::LAIR_ONE_BY_ONE ||
            Lair.Type == LairType::LAIR_MULTISPAWN ||
            Lair.Type == LairType::LAIR_ONE_BY_ONE_PROX) {
            int RandomValue = Random::RandomInteger(ONE_BY_ONE_WEIGHT + MULTISPAWN_WEIGHT + ONE_BY_ONE_PROX_WEIGHT);
            if (RandomValue < ONE_BY_ONE_WEIGHT) {
                Lair.Type = LairType::LAIR_ONE_BY_ONE;
            }
            else if (RandomValue < ONE_BY_ONE_WEIGHT + MULTISPAWN_WEIGHT) {
                Lair.Type = LairType::LAIR_MULTISPAWN;
            }
            else {
                Lair.Type = LairType::LAIR_ONE_BY_ONE_PROX;
            }
        }
        else if (Lair.Type == LairType::LAIR_TWO_UP_TWO_DOWN) {
            int RandomValue = Random::RandomInteger(4);
            switch (RandomValue) {
            case 0:
                Lair.Type = LairType::LAIR_ONE_BY_ONE;
                break;
            case 1:
                Lair.Type = LairType::LAIR_MULTISPAWN;
                break;
            case 2:
                Lair.Type = LairType::LAIR_ONE_BY_ONE_PROX;
                break;
            case 3:
                /* Don't change */
                break;
            }
        }
    }


    void RandomizeLairNbEnemies(Lair &Lair) {

#ifdef DEBUG_NO_ENEMIES
        /**** Debug: remove lair enemies ****/
        if (Lair.Enemy == DEMON_BIRD) {
            Lair.NbEnemies = 1;
            return;
        }
        else if (Lair.PositionData[0] == 0x2F ||
                 Lair.PositionData[0] == 0x2E ||
                 Lair.PositionData[0] == 0x30 ||
                 Lair.PositionData[0] == 0x31) {
            Lair.NbEnemies = 1;
            return;
        }
        else {
            Lair.NbEnemies = 0;
            return;
        }
#endif

        if (Lair.Type == LairType::LAIR_ONE_BY_ONE || Lair.Type == LairType::LAIR_ONE_BY_ONE_PROX) {
            Lair.NbEnemies = Random::RandomIntegerRange(NB_ENEMIES_ONE_BY_ONE_MIN, NB_ENEMIES_ONE_BY_ONE_MAX);
        }
        else if (Lair.Type == LairType::LAIR_MULTISPAWN || Lair.Type == LairType::LAIR_TWO_UP_TWO_DOWN) {
            if (Lair.Enemy == EnemyType::ACT6_MIMIC) {
                /* Don't spawn too many for those enemies */
                Lair.NbEnemies = Random::RandomIntegerRange(NB_ENEMIES_MULTISPAWN_REDUCED_MIN, NB_ENEMIES_MULTISPAWN_REDUCED_MAX);
            }
            else {
                Lair.NbEnemies = Random::RandomIntegerRange(NB_ENEMIES_MULTISPAWN_MIN, NB_ENEMIES_MULTISPAWN_MAX);
            }
        }

#ifdef DEBUG_TWO_ENEMIES
        if (Lair.NbEnemies > 2) Lair.NbEnemies = 2;
        return;
#endif
    }


    void RandomizeLairSpawnRate(Lair &Lair) {
        if (Lair.Type == LairType::LAIR_MULTISPAWN || Lair.Type == LairType::LAIR_TWO_UP_TWO_DOWN) {
            Lair.SpawnRate = Random::RandomIntegerRange(SPAWN_RATE_MIN, SPAWN_RATE_MAX);
        }

        /* Reset Spawn Rate for One-by-One Lairs */
        if (Lair.Type == LairType::LAIR_ONE_BY_ONE || Lair.Type == LairType::LAIR_ONE_BY_ONE_PROX) {
            Lair.SpawnRate = 0;
        }
    }


    void RandomizeLairContents(std::vector<Lair> &LairList) {

        for (int LairIndex = 0; LairIndex < NUMBER_OF_LAIRS; LairIndex++) {

            if (LairList[LairIndex].Enemy == EnemyType::NO_ENEMY ||
                LairList[LairIndex].Enemy == EnemyType::DREAM_NO_ENEMY) {
                /* Skip empty lairs / lairs in dreams */
                continue;
            }

            /* Change Enemy Type */
            RandomizeLairEnemies(LairList[LairIndex]);

            /* Change Lair Type */
            RandomizeLairType(LairList[LairIndex]);

            /* Change Number of Enemies */
            RandomizeLairNbEnemies(LairList[LairIndex]);

            /* Change Spawn Rate */
            RandomizeLairSpawnRate(LairList[LairIndex]);
        }
    }



    void RandomizeMapSprites(std::vector<Sprite> &RandomizedSpriteList, std::fstream &ROMFile) {

        /* Initialize the list */
        std::vector<Sprite> OriginalSpriteList(NUMBER_OF_SPRITES);
        ROMData::GetOriginalMapSpriteData(OriginalSpriteList, ROMFile);
        RandomizedSpriteList = OriginalSpriteList;

        EnemyType Enemy;
        for (int SpriteIndex = 0; SpriteIndex < NUMBER_OF_SPRITES; ++SpriteIndex) {

            Enemy = RandomizedSpriteList[SpriteIndex].Enemy;

            switch (RandomizedSpriteList[SpriteIndex].Act) {
            case ActID::ACT_1:
                if (Enemy < EnemyType::ACT1_ARMOR) {
                    /* Sprite is in Underground Castle */
                    Enemy = UndergroundCastleEnemies[Random::RandomInteger(6)];
                }
                else {
                    /* Sprite is in Leo's Paintings */
                    Enemy = LeosPaintingsEnemies[Random::RandomInteger(4)];
                }
                break;
            case ActID::ACT_2:
                if (Enemy < EnemyType::ACT2_FIRE_SPIRIT) {
                    /* Sprite is in Lost Marsh or Water Shrine */
                    Enemy = WaterShrineEnemies[Random::RandomInteger(5)];
                }
                else {
                    /* Sprite is in Fire Shrine or Light Shrine */
                    Enemy = FireLightShrineEnemies[Random::RandomInteger(5)];
                }
                break;
            case ActID::ACT_3:
                if (Enemy < EnemyType::ACT3_PALM_TREE) {
                    /* Sprite is in Seabed */
                    Enemy = SeabedEnemies[Random::RandomInteger(5)];
                }
                else {
                    /* Sprite is in Islands */
                    Enemy = IslandsEnemies[Random::RandomInteger(5)];
                }
                break;
            case ActID::ACT_4:
                if (Enemy < EnemyType::ACT4_PURPLE_WIZARD) {
                    /* Sprite is in Mountain of Souls */
                    Enemy = MountainEnemies[Random::RandomInteger(5)];
                }
                else {
                    /* Sprite is in Laynole or Lune */
                    Enemy = LaynoleLuneEnemies[Random::RandomInteger(5)];
                }
                break;
            case ActID::ACT_5:
                if (Enemy < EnemyType::ACT5_MINI_KNIGHT) {
                    /* Sprite is in Leo's Lab Basement or Power Plant */
                    Enemy = LeosBasementEnemies[Random::RandomInteger(5)];
                }
                else {
                    /* Sprite is in one of the Model Towns */
                    Enemy = ModelTownsEnemies[Random::RandomInteger(5)];
                }
                break;
            case ActID::ACT_6:
                if (Enemy < EnemyType::ACT6_PURPLE_KNIGHT) {
                    /* Sprite is in Magridd Castle Basement */
                    Enemy = CastleBasementEnemiesFull[Random::RandomInteger(6)];
                }
                else {
                    /* Sprite is in one of the Magridd Castle Towers */
                    Enemy = CastleTowersEnemiesFull[Random::RandomInteger(7)];
                }
                break;
            case ActID::ACT_7:
                /* Sprite is in World of Evil */
                Enemy = WorldOfEvilEnemies[Random::RandomInteger(3)];
                break;
            default:
                /* Should not happen! */
                break;
            }

            /* Randomize orientation if possible */
            if (CanRandomizeOrientation(RandomizedSpriteList[SpriteIndex].Act, Enemy)) {
                RandomizedSpriteList[SpriteIndex].Orientation = OrientationList[Random::RandomInteger(4)] + 1;
            }
            else {
                RandomizedSpriteList[SpriteIndex].Orientation = 0x01;
            }

            RandomizedSpriteList[SpriteIndex].Enemy = Enemy;
        }
    }



    void ExploreRegion(std::vector<Region> &RegionList,
                       std::vector<Goal>   &GoalList,
                       std::vector<int>    &AvailableRevivingLairs,
                       std::vector<int>    &AvailableItems,
                       std::vector<int>    &AvailableGoals,
                       bool            RevivedNPCs[],
                       bool            CollectedKeyItems[],
                       int             RegionIndex) {
        int GoalIndex;
        bool KeepExploring, GoalFulfilled;

        do {

            KeepExploring = false;

            /* Explore the targeted region: store all the new available reviving lairs and items... */
            for (const auto& Content : RegionList[RegionIndex].Contents) {
                if (Content.Type == GoalType::LAIR) {
                    AvailableRevivingLairs.push_back(Content.Index);
                }
                else {
                    AvailableItems.push_back(Content.Index);
                }
            }

            /* ...and store all new available goals */
            AvailableGoals.insert(AvailableGoals.end(), RegionList[RegionIndex].NextGoals.begin(),
                                  RegionList[RegionIndex].NextGoals.end());

            /* Check the available goals, maybe some of them are already fulfilled */
            for (GoalIndex = 0; GoalIndex < (int)AvailableGoals.size(); GoalIndex++) {

                GoalFulfilled = true;

                /* Read the contents of this goal */
                for (const auto& Requirement : GoalList[AvailableGoals[GoalIndex]].Requirements) {
                    if ( (Requirement.Type == GoalType::LAIR && RevivedNPCs[Requirement.Index]       == false) ||
                         (Requirement.Type == GoalType::ITEM && CollectedKeyItems[Requirement.Index] == false) ) {
                        /* This goal is not met yet, check the next one */
                        GoalFulfilled = false;
                        break;
                    }
                }

                if (GoalFulfilled) {
                    /* This goal is met! We can remove it and explore its targeted region. */
                    RegionIndex = GoalList[AvailableGoals[GoalIndex]].Target;
                    AvailableGoals.erase(AvailableGoals.begin() + GoalIndex);
                    KeepExploring = true;
                    break;
                }
            }

        } while (KeepExploring);
    }


    bool RandomizeProgression(std::vector<Lair>  &RandomizedLairList,
                              std::vector<Item>  &RandomizedItemList,
                              std::fstream       &ROMFile) {

        /* Get the original Lair and Item data */
        std::vector<Lair> OriginalLairList(NUMBER_OF_LAIRS);
        std::vector<Item> OriginalItemList(NUMBER_OF_ITEMS);
        ROMData::GetOriginalLairData(OriginalLairList, ROMFile);
        ROMData::GetOriginalItemData(OriginalItemList);

        std::vector<Region> RegionList(NUMBER_OF_REGIONS);
        std::vector<Goal>   GoalList(NUMBER_OF_GOALS);

        /* Get the map and calculate the goal weights */
        Map::InitMap(RegionList, GoalList);
        Map::CalculateWeights(RegionList, GoalList, GOAL_TO_FIRST_REGION);

#ifdef DEBUG
        std::cout << endl;
        for (int GoalIdx = 0; GoalIdx < NUMBER_OF_GOALS; ++GoalIdx) {
            std::cout << "Weight for Goal #" << GoalIdx << " -> " << GoalList[GoalIdx].Weight << endl;
        }
        std::cout << endl;
#endif

        std::vector<int> AvailableRevivingLairs;
        std::vector<int> AvailableItems;
        std::vector<int> AvailableGoals;
        std::vector<int> VillageChiefPossibleLairs;
        bool RevivedNPCs      [NUMBER_OF_LAIRS] = {false};
        bool CollectedKeyItems[NUMBER_OF_ITEMS] = {false};

        int GoalIndex, RevivingLairIndex, ItemID, NewRegionIndex;
        bool MountainKingItemRandomized = false;

        /* Initialize the randomized lists to the original lair and item lists */
        RandomizedLairList = OriginalLairList;
        RandomizedItemList = OriginalItemList;

#ifdef DONT_RANDOMIZE
        return true;
#endif

        /* Initialization: Region 0 is available at the start, so let's explore it */
        ExploreRegion(RegionList,
                      GoalList,
                      AvailableRevivingLairs,
                      AvailableItems,
                      AvailableGoals,
                      RevivedNPCs,
                      CollectedKeyItems,
                      0);


        /*=========================*\
        |   Main Exploration Loop   |
        \*=========================*/

        while (AvailableGoals.size() > 0) {

            /* Pick a goal */
            //GoalIndex = Random::RandomInteger(AvailableGoals.size());
            GoalIndex = WeightedGoalChoice(GoalList, AvailableGoals);

#ifdef DEBUG
            std::cout << "Available goals: ";
            for (unsigned int i=0; i<AvailableGoals.size(); ++i) {
                std::cout << AvailableGoals[i] << " ";
            }
            std::cout << endl;
            std::cout << endl;
            std::cout << "Available Lairs: ";
            for (unsigned int i=0; i<AvailableRevivingLairs.size(); ++i) {
                std::cout << AvailableRevivingLairs[i] << " ";
            }
            std::cout << endl;
            std::cout << "Available Items: ";
            for (unsigned int i=0; i<AvailableItems.size(); ++i) {
                std::cout << AvailableItems[i] << " ";
            }
            std::cout << endl;
            std::cout << endl;
            std::cout << "Picked goal " << AvailableGoals[GoalIndex] << ".\n";
#endif

            /* Read the contents of the goal */
            for (const auto &Requirement : GoalList[AvailableGoals[GoalIndex]].Requirements) {
                if (Requirement.Type == GoalType::LAIR) {

                    /* If this NPC is already revived, skip this requirement */
                    if (RevivedNPCs[Requirement.Index] == false) {

                        if (AvailableRevivingLairs.size() > 0) {

                            /* Choose one of the available reviving Monster Lairs and assign this NPC to it */
                            RevivingLairIndex = Random::RandomInteger(AvailableRevivingLairs.size());
                            RandomizedLairList[Requirement.Index] = OriginalLairList[AvailableRevivingLairs[RevivingLairIndex]];

#ifdef DEBUG
                            std::cout << "Lair : " << Requirement.Index << " <-- " << AvailableRevivingLairs[RevivingLairIndex] << endl;
#endif

                            /* This NPC has been revived */
                            RevivedNPCs[Requirement.Index] = true;

                            /* This Lair is no longer available */
                            AvailableRevivingLairs.erase(AvailableRevivingLairs.begin() + RevivingLairIndex);

                        }
                        else {
                            /* FAILURE! */
                            return false;
                        }
                    }
                }
                else {

                    /* If this Key Item is already collected, skip this requirement */
                    if (CollectedKeyItems[Requirement.Index] == false) {

                        if (AvailableItems.size() > 0) {

                            /* Choose one of the available item locations and assign this item to it */
                            ItemID = Random::RandomInteger(AvailableItems.size());
                            RandomizedItemList[AvailableItems[ItemID]] = OriginalItemList[Requirement.Index];

#ifdef DEBUG
                            std::cout << "Item: " << AvailableItems[ItemID] << " <-- " << Requirement.Index << endl;
#endif

                            /* This Key Item has been collected */
                            CollectedKeyItems[Requirement.Index] = true;

                            /* If this was Mountain King's item, remember it */
                            if (AvailableItems[ItemID] == ITEM_MOUNTAIN_KING) {
                                MountainKingItemRandomized = true;
                            }

                            /* This Item is no longer available */
                            AvailableItems.erase(AvailableItems.begin() + ItemID);
                        }
                        else {
                            /* FAILURE! */
                            return false;
                        }
                    }
                }
            }

            /* Index of the region targeted by this goal */
            NewRegionIndex = GoalList[AvailableGoals[GoalIndex]].Target;

            /* This goal has been met! Remove it. */
            AvailableGoals.erase(AvailableGoals.begin() + GoalIndex);

            /* The target region of this goal is now available: explore it and store its contents */
            ExploreRegion(RegionList,
                          GoalList,
                          AvailableRevivingLairs,
                          AvailableItems,
                          AvailableGoals,
                          RevivedNPCs,
                          CollectedKeyItems,
                          NewRegionIndex);

        } /* End of the Main Exploration Loop */


        /*===================*\
        |   Post-Processing   |
        \*===================*/

        /* At this stage, all key chest items and NPC have been placed.
           We now need to place all the other elements (non-key items and NPCs). */
        std::vector<int> NonKeyNPCList;
        std::vector<int> NonKeyItemList;
        Map::GetNonKeyNPCList (NonKeyNPCList);
        Map::GetNonKeyItemList(NonKeyItemList);

        /* Add the items/NPC locations which may not always be accessible,
           to make sure they still receive a non-key item/NPC. */
        //AvailableItems.push_back(CHEST_LEOS_BRUSH);
        AvailableItems.push_back(17);
        //AvailableItems.push_back(10);
        //AvailableItems.push_back(22);
        AvailableItems.push_back(ITEM_QUEEN_MAGRIDD);
        //AvailableItems.push_back(ITEM_SOLDIER_PLATINUM_CARD);
        //AvailableRevivingLairs.push_back(NPC_OLD_MAN);

#ifdef DEBUG
        std::cout << AvailableRevivingLairs.size() << " available Reviving Lairs, " << NonKeyNPCList.size() << " in NPC list.\n";
        std::cout << AvailableItems.size() << " available Items, " << NonKeyItemList.size() << " in Item list.\n";
#endif

        /* Make sure Mountain King receives an item different from a Medical Herb, a Strange Bottle, Gems/EXP or nothing */
        if (MountainKingItemRandomized == false) {
            ItemID = Random::RandomInteger(NonKeyItemList.size());
            while (OriginalItemList[NonKeyItemList[ItemID]].Contents == ItemID::MEDICAL_HERB ||
                   OriginalItemList[NonKeyItemList[ItemID]].Contents == ItemID::STRANGE_BOTTLE ||
                   OriginalItemList[NonKeyItemList[ItemID]].Contents == ItemID::GEMS_EXP ||
                   OriginalItemList[NonKeyItemList[ItemID]].Contents == ItemID::NOTHING) {
                ItemID = Random::RandomInteger(NonKeyItemList.size());
            }
            RandomizedItemList[ITEM_MOUNTAIN_KING] = OriginalItemList[NonKeyItemList[ItemID]];
            NonKeyItemList.erase(NonKeyItemList.begin() + ItemID);
        }

        /* Shuffle these lists */
        Random::Shuffle(NonKeyNPCList);
        Random::Shuffle(NonKeyItemList);

        for (RevivingLairIndex = 0; RevivingLairIndex < (int)AvailableRevivingLairs.size(); RevivingLairIndex++) {
            /* Fill this lair and remove it from the list */
            RandomizedLairList[NonKeyNPCList[RevivingLairIndex]] = OriginalLairList[AvailableRevivingLairs[RevivingLairIndex]];
        }

        int Offset = 0;
        for (ItemID = 0; ItemID < (int)AvailableItems.size(); ItemID++) {

            /* Mountain King's Item has already been assigned */
            if (AvailableItems[ItemID] == ITEM_MOUNTAIN_KING) {
                Offset = 1;
                continue;
            }

            /* Fill this item and remove it from the list */
            RandomizedItemList[AvailableItems[ItemID]] = OriginalItemList[NonKeyItemList[ItemID-Offset]];
        }

        for (int i=NUMBER_OF_CHESTS; i<NUMBER_OF_ITEMS; i++) {
            /* If the NPC doesn't normally give EXP, let us turn the
               EXP/Nothing prize into a Medical Herb for now. */
            auto ItemID = RandomizedItemList[i].Contents;
            if (!ROMData::NPCOriginallyGivesEXP(i) &&
                (ItemID == ItemID::GEMS_EXP || ItemID == ItemID::NOTHING)) {
                RandomizedItemList[i].Contents = ItemID::MEDICAL_HERB;
            }
        }

        /* Randomization successful! */
        return true;
    }


    int WeightedGoalChoice(std::vector<Goal>  &GoalList,
                           std::vector<int>   &AvailableGoals) {

        int SumOfWeights = 0;
        unsigned int GoalIndex;

        /* Add up all the weights */
        for (GoalIndex = 0; GoalIndex < AvailableGoals.size(); ++GoalIndex) {
            SumOfWeights += GoalList[AvailableGoals[GoalIndex]].Weight;
        }

        /* Pick a number and determine which Goal it corresponds to */
        int RandomNumber = Random::RandomInteger(SumOfWeights);
        for (GoalIndex = 0; GoalIndex < AvailableGoals.size(); ++GoalIndex) {
            if (RandomNumber < GoalList[AvailableGoals[GoalIndex]].Weight) {
                return GoalIndex;
            }
            else {
                RandomNumber -= GoalList[AvailableGoals[GoalIndex]].Weight;
            }
        }

        /* We shouldn't end up here */
        return 0;
    }

    ROMStatus CheckFile(const std::string& Filename) {
        /* Check that the ROM file is there, make the fstream instance */
        std::fstream ROMFile(Filename, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
        if (!ROMFile.is_open()) {
            std::cout << "ROM file \"" << Filename << "\" is not found!\n";
            return UNKNOWN;
        }

        /* Check if this is the headered or the un-headered ROM */
        ROMStatus OriginalROMStatus = CheckOriginalROM(ROMFile);
        if (OriginalROMStatus == UNKNOWN) {
            std::cout << "File \"" << Filename << "\" is not a known original Soul Blazer (U) ROM!\n";
            return UNKNOWN;
        }

        std::cout << "ROM is valid and " << (OriginalROMStatus == UNHEADERED ? "un-" : "") << "headered.\n";

        return OriginalROMStatus;
    }

    bool Randomize(const std::string& InFile, const std::string& OutFile, unsigned int seed, const Options& options, std::string* seedName) {
        /****************************\
        |*  Check the original ROM  *|
        \****************************/
        ROMStatus OriginalROMStatus = Randomizer::CheckFile(InFile);
        if (OriginalROMStatus == UNKNOWN) {
            return false;
        }

        if (options.race) {
            std::cout << "Race mode enabled, randomizing seed number.\n";
            seed = 0;
        }

        seed = Random::RandomInit(seed);
        std::string seedText = std::to_string(seed);

        if (options.race) {
            seedText += std::to_string(Random::RandomInteger(INT32_MAX));
            // Get top 10 characters of SHA1 hash
            boost::uuids::detail::sha1 sha1;
            sha1.process_bytes(seedText.data(), seedText.size());
            unsigned hash[5] = {0};
            sha1.get_digest(hash);
            char buffer[41] = {0};
            for (int i = 0; i < 5; i++) {
                std::sprintf(buffer + (i << 3), "%08x", hash[i]);
            }
            seedText = "Race ";
            seedText += std::string(buffer, 5);
        }

        if (seedName) {
            *seedName = seedText;
        }

        std::cout << "Seed " << seedText << "\n";

        /***************************************************\
        |*  Delete old modified ROM / backup original ROM  *|
        \***************************************************/

        /* If a modified ROM exists, delete it first */
        int RemoveResult = ::remove(OutFile.c_str());
        if (RemoveResult == 0) {
            std::cout << "Previous randomized ROM detected and removed.\n";
        }

        /* Back up the original ROM */
        std::ifstream ROMFileOriginal(InFile, std::ios::binary);
        std::ofstream ROMFileCopy    (OutFile,     std::ios::binary);

        ROMFileOriginal.seekg(0, std::ios::end);
        long ROMFileSize = ROMFileOriginal.tellg();
        if (OriginalROMStatus == HEADERED) {
            /* For headered ROM, ignore the first 512 bytes */
            ROMFileSize -= HEADER_OFFSET;
        }

        if(ROMFileOriginal.is_open() && ROMFileCopy.is_open()) {
            unsigned char *DataBuffer = new unsigned char[ROMFileSize];
            if (OriginalROMStatus == HEADERED) {
                /* For headered ROM, ignore the first 512 bytes */
                ROMFileOriginal.seekg(HEADER_OFFSET, std::ios::beg);
            }
            else {
                ROMFileOriginal.seekg(0, std::ios::beg);
            }
            ROMFileOriginal.read((char*)DataBuffer, ROMFileSize);
            ROMFileCopy.write((char*)DataBuffer, ROMFileSize);
            delete[] DataBuffer;
        }
        else if(!ROMFileCopy.is_open())        {
            std::cout << "Failure backing up the ROM!\n";
            return false;
        }
        else if(!ROMFileOriginal.is_open())        {
            std::cout << "Failure opening the original ROM for copying!\n";
            return false;
        }

        ROMFileOriginal.close();
        ROMFileCopy.close();
        ROMFileOriginal.clear();
        ROMFileCopy.clear();


        /****************\
        |*  Randomize!  *|
        \****************/

        std::cout << std::endl;
        std::cout << "Starting randomization.\n";

        /* Initialize the final lists of randomized lairs, chests and sprites */
        std::vector<Lair> RandomizedLairList;
        std::vector<Item> RandomizedItemList;
        std::vector<Sprite> RandomizedSpriteList;

        /* Open ROM to be modified */
        std::fstream ROMFile(OutFile, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);

        /* Call the main algorithm to randomize the progression through the game:
        ==> randomize item locations and revived NPCs */
        int RandomizationTry;
        bool RandomizationStatus = false;
        for (RandomizationTry = 0; RandomizationTry < MAX_NUMBER_OF_TRIES; RandomizationTry++) {
            RandomizationStatus = Randomizer::RandomizeProgression(RandomizedLairList,
                                                                RandomizedItemList,
                                                                ROMFile);
            if (RandomizationStatus) {
                break;
            }
        }
        if (!RandomizationStatus) {
            std::cout << " . . . Randomization failed!\n";
            return false;
        }
        std::cout << " . . . Randomization succeeded in " << RandomizationTry + 1
            << (RandomizationTry == 0 ? " try.\n" : " tries.\n");

        std::cout << "Starting ROM modification.\n";

        /* Randomize monster lair contents: enemy types, lair types, number of enemies and spawn rates */
        Randomizer::RandomizeLairContents(RandomizedLairList);

        /* Randomize static enemies in maps */
        Randomizer::RandomizeMapSprites(RandomizedSpriteList, ROMFile);

        /* Modify the ROM with the randomized lists */
        ROMUpdate::ROMUpdateLairs(RandomizedLairList, ROMFile);
        ROMUpdate::ROMUpdateMapSprites(RandomizedSpriteList, ROMFile);
        ROMUpdate::ROMUpdateTextAndItems(RandomizedLairList,
                                        RandomizedItemList,
                                        ROMFile,
                                        seedText);

        /* Close the ROM file */
        ROMFile.close();
        ROMFile.clear();

        std::cout << " . . . ROM modification complete.\n";

        if (!options.race) {
            std::cout << "Starting Spoiler Log creation.\n";

            /* Generate the Spoiler Log */
            Log::CreateSpoilerLog(RandomizedLairList, RandomizedItemList);

            std::cout << " . . . Spoiler Log created.\n";
        }

        return true;
    }

    Options::Options(const std::string& options_string) {
        std::vector<std::string> options_list;
        boost::algorithm::split(options_list, options_string, boost::is_any_of(","));

        for (const auto &option : options_list) {
            if (option == "race") {
                race = true;
            } else {
                std::cout << "Unknown option: " << option << "\n";
            }
        }
    }
}
