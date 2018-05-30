#include "Randomizer.h"
#include "ROMData.h"
#include "Random.h"

#include <algorithm>
#include <iomanip>
#include <iostream>


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
//#define DONT_RANDOMIZE


using namespace std;
using namespace Random;

namespace Randomizer {

    static int UndergroundCastleEnemies[6] =
        {ACT1_GOBLIN, ACT1_IMP, ACT1_FLY, ACT1_PLANT, ACT1_SLIME, ACT1_TORCH};
    static int LeosPaintingsEnemies[4] =
        {ACT1_ARMOR, ACT1_BIRD, ACT1_TORCH2, ACT1_BLOCK};
    static int WaterShrineEnemies[5] =
        {ACT2_WATER_DRAGON, ACT2_MUDMAN, ACT2_BUSH, ACT2_STATUE, ACT2_FLOWER};
    static int FireLightShrineEnemies[5] =
        {ACT2_FIRE_SPIRIT, ACT2_GHOST, ACT2_LIZARDMAN, ACT2_TP_LIZARDMAN, ACT2_FIREMAN};
    static int FireLightShrineEnemiesNoFireSpirit[4] =
        {ACT2_GHOST, ACT2_LIZARDMAN, ACT2_TP_LIZARDMAN, ACT2_FIREMAN};
    static int SeabedEnemies[5] =
        {ACT3_URCHIN, ACT3_JELLYFISH, ACT3_CRAB, ACT3_RAY, ACT3_SEAHORSE};
    static int IslandsEnemies[5] =
        {ACT3_PALM_TREE, ACT3_ROCK, ACT3_FISH, ACT3_GORILLA, ACT3_EAGLE};
    static int IslandsEnemiesNoFish[4] =
        {ACT3_PALM_TREE, ACT3_ROCK, ACT3_GORILLA, ACT3_EAGLE};
    static int MountainEnemies[5] =
        {ACT4_RAT, ACT4_MOOSE, ACT4_YETI, ACT4_BAT, ACT4_SNOWBALL};
    static int MountainEnemiesNoSnowball[4] =
        {ACT4_RAT, ACT4_MOOSE, ACT4_YETI, ACT4_BAT};
    static int LaynoleLuneEnemies[6] =
        {ACT4_PURPLE_WIZARD, ACT4_RED_WIZARD, ACT4_ICE_HEAD, ACT4_ICE_BLOCK, ACT4_CIRCLING_BAT, ACT4_SLIME};
    static int LaynoleLuneEnemiesNoIceBlock[5] =
        {ACT4_PURPLE_WIZARD, ACT4_RED_WIZARD, ACT4_ICE_HEAD, ACT4_CIRCLING_BAT, ACT4_SLIME};
    static int LeosBasementEnemies[5] =
        {ACT5_METAL_MOUSE, ACT5_BULLDOZER, ACT5_HELICOPTER, ACT5_WORM, ACT5_ROBOT};
    static int LeosBasementEnemiesMetalOnly[3] =
        {ACT5_METAL_MOUSE, ACT5_BULLDOZER, ACT5_HELICOPTER};
    static int LeosBasementEnemiesNoMetal[2] =
        {ACT5_WORM, ACT5_ROBOT};
    static int ModelTownsEnemies[5] =
        {ACT5_MINI_KNIGHT, ACT5_MINI_ARCHER, ACT5_MINI_HORSEMAN, ACT5_CATAPULT, ACT5_TOWER};
    static int CastleBasementEnemies[4] =
        {ACT6_ORB, ACT6_GHOST, ACT6_SNAKE, ACT6_SKELETON};
    static int CastleBasementEnemiesNoGhost[3] =
        {ACT6_ORB, ACT6_SNAKE, ACT6_SKELETON};
    static int CastleBasementEnemiesFull[6] =
        {ACT6_SKULL, ACT6_ORB, ACT6_GHOST, ACT6_SNAKE, ACT6_FIRE, ACT6_SKELETON};
    static int CastleTowersEnemies[5] =
        {ACT6_PURPLE_KNIGHT, ACT6_RED_KNIGHT, ACT6_MIMIC, ACT6_DOLL, ACT6_CHESS_KNIGHT};
    static int CastleTowersEnemiesFull[7] =
        {ACT6_PURPLE_KNIGHT, ACT6_RED_KNIGHT, ACT6_FIRE2, ACT6_SKULL2, ACT6_MIMIC, ACT6_DOLL, ACT6_CHESS_KNIGHT};
    static int WorldOfEvilEnemies[3] =
        {ACT7_DEMON, ACT7_FLY, ACT7_BRICK};
    static int WorldOfEvilEnemiesNoBrick[2] =
        {ACT7_DEMON, ACT7_FLY};

    void RandomizeLairEnemies(Lair &Lair) {

        /* Don't randomize enemies from 2-up-2-down lairs, because upside-down enemies can sometimes get away... */
        if (Lair.Type[0] == LAIR_TWO_UP_TWO_DOWN) {
            return;
        }

        /* A few lairs should not be randomized (yet) */
        if (Lair.MustNotRandomizeLairPosition()) {
            return;
        }

        int Enemy = Lair.Enemy;
        int OriginalEnemy = Enemy;

        switch (Lair.Act) {
        case ACT_1:
            if (Enemy != ACT1_SPIKEY &&
                Enemy != SOLID_ARM) {
                if (Enemy < ACT1_ARMOR) {
                    /* Lair is in Underground Castle */
                    Enemy = UndergroundCastleEnemies[RandomInteger(6)];
                }
                else {
                    /* Lair is in Leo's Paintings */
                    Enemy = LeosPaintingsEnemies[RandomInteger(4)];
                }
            }
            break;
        case ACT_2:
            if (Enemy != ACT2_SCORPION &&
                Enemy != ACT2_FIRE_SPIRIT &&
                Enemy != ELEMENTAL_STATUE) {
                if (Enemy < ACT2_FIRE_SPIRIT) {
                    /* Lair is in Lost Marsh or Water Shrine */
                    Enemy = WaterShrineEnemies[RandomInteger(5)];
                }
                else {
                    /* Lair is in Fire Shrine or Light Shrine */
                    Enemy = FireLightShrineEnemiesNoFireSpirit[RandomInteger(4)];
                }
            }
            break;
        case ACT_3:
            if (Enemy != ACT3_METAL_GORILLA &&
                Enemy != FLOATING_SKULL) {
                if (Enemy < ACT3_PALM_TREE) {
                    /* Lair is in Seabed */
                    Enemy = SeabedEnemies[RandomInteger(5)];
                }
                else {
                    /* Lair is in Islands */
                    if (Lair.NoFishLairPosition()) {
                        Enemy = IslandsEnemiesNoFish[RandomInteger(4)];
                    }
                    else {
                        Enemy = IslandsEnemies[RandomInteger(5)];
                    }
                }
            }
            break;
        case ACT_4:
            if (Enemy != POSEIDON) {
                if (Enemy < ACT4_PURPLE_WIZARD) {
                    /* Lair is in Mountain of Souls */
                    Enemy = MountainEnemiesNoSnowball[RandomInteger(4)];
                }
                else {
                    /* Lair is in Laynole or Lune */
                    Enemy = LaynoleLuneEnemiesNoIceBlock[RandomInteger(5)];
                }
            }
            break;
        case ACT_5:
            if (Enemy != TIN_DOLL) {
                if (Enemy < ACT5_MINI_KNIGHT) {
                    /* Lair is in Leo's Lab Basement or Power Plant */
                    if (Lair.NoMetalLairPosition()) {
                        Enemy = LeosBasementEnemiesNoMetal[RandomInteger(2)];
                    }
                    else if (Lair.MustBeMetalLairPosition()) {
                        Enemy = LeosBasementEnemiesMetalOnly[RandomInteger(3)];
                    }
                    else {
                        Enemy = LeosBasementEnemies[RandomInteger(5)];
                    }
                }
                else {
                    /* Lair is in one of the Model Towns */
                    Enemy = ModelTownsEnemies[RandomInteger(5)];
                }
            }
            break;
        case ACT_6:
            if (Enemy != DEMON_BIRD) {
                if (Enemy < ACT6_PURPLE_KNIGHT) {
                    /* Lair is in Magridd Castle Basement */
                    if (Lair.NoGhostLairPosition()) {
                        Enemy = CastleBasementEnemiesNoGhost[RandomInteger(3)];
                    }
                    else if (Lair.MustBeGhostLairPosition()) {
                        Enemy = ACT6_GHOST;
                    }
                    else {
                        Enemy = CastleBasementEnemies[RandomInteger(4)];
                    }
                }
                else {
                    /* Lair is in one of the Magridd Castle Towers */
                    Enemy = CastleTowersEnemies[RandomInteger(5)];
                }
            }
            break;
        case ACT_7:
            /* Lair is in World of Evil */
            Enemy = WorldOfEvilEnemiesNoBrick[RandomInteger(2)];
            break;
        default:
            /* Should not happen! */
            break;
        }

        /* Update enemy */
        Lair.Enemy = Enemy;

        /* If the enemy has changed, reset the upside-down flag */
        if (Enemy != OriginalEnemy) {
            Lair.UpsideDownFlag = 0;
        }
    }


    void RandomizeLairType(Lair &Lair) {

        if (Lair.Type[0] == LAIR_ONE_BY_ONE ||
            Lair.Type[0] == LAIR_MULTISPAWN ||
            Lair.Type[0] == LAIR_ONE_BY_ONE_PROX) {
            int RandomValue = RandomInteger(ONE_BY_ONE_WEIGHT + MULTISPAWN_WEIGHT + ONE_BY_ONE_PROX_WEIGHT);
            if (RandomValue < ONE_BY_ONE_WEIGHT) {
                Lair.Type[0] = LAIR_ONE_BY_ONE;
                Lair.Type[1] = LAIR_ONE_BY_ONE_SECOND_BYTE;
            }
            else if (RandomValue < ONE_BY_ONE_WEIGHT + MULTISPAWN_WEIGHT) {
                Lair.Type[0] = LAIR_MULTISPAWN;
                Lair.Type[1] = LAIR_MULTISPAWN_SECOND_BYTE;
            }
            else {
                Lair.Type[0] = LAIR_ONE_BY_ONE_PROX;
                Lair.Type[1] = LAIR_ONE_BY_ONE_PROX_SECOND_BYTE;
            }
        }
        else if (Lair.Type[0] == LAIR_TWO_UP_TWO_DOWN) {
            int RandomValue = RandomInteger(4);
            switch (RandomValue) {
            case 0:
                Lair.Type[0] = LAIR_ONE_BY_ONE;
                Lair.Type[1] = LAIR_ONE_BY_ONE_SECOND_BYTE;
                break;
            case 1:
                Lair.Type[0] = LAIR_MULTISPAWN;
                Lair.Type[1] = LAIR_MULTISPAWN_SECOND_BYTE;
                break;
            case 2:
                Lair.Type[0] = LAIR_ONE_BY_ONE_PROX;
                Lair.Type[1] = LAIR_ONE_BY_ONE_PROX_SECOND_BYTE;
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

        if (Lair.Type[0] == LAIR_ONE_BY_ONE || Lair.Type[0] == LAIR_ONE_BY_ONE_PROX) {
            Lair.NbEnemies = RandomIntegerRange(NB_ENEMIES_ONE_BY_ONE_MIN, NB_ENEMIES_ONE_BY_ONE_MAX);
        }
        else if (Lair.Type[0] == LAIR_MULTISPAWN || Lair.Type[0] == LAIR_TWO_UP_TWO_DOWN) {
            if (Lair.Enemy == ACT6_MIMIC) {
                /* Don't spawn too many for those enemies */
                Lair.NbEnemies = RandomIntegerRange(NB_ENEMIES_MULTISPAWN_REDUCED_MIN, NB_ENEMIES_MULTISPAWN_REDUCED_MAX);
            }
            else {
                Lair.NbEnemies = RandomIntegerRange(NB_ENEMIES_MULTISPAWN_MIN, NB_ENEMIES_MULTISPAWN_MAX);
            }
        }
    }


    void RandomizeLairSpawnRate(Lair &Lair) {
        if (Lair.Type[0] == LAIR_MULTISPAWN || Lair.Type[0] == LAIR_TWO_UP_TWO_DOWN) {
            Lair.SpawnRate = RandomIntegerRange(SPAWN_RATE_MIN, SPAWN_RATE_MAX);
        }

        /* Reset Spawn Rate for One-by-One Lairs */
        if (Lair.Type[0] == LAIR_ONE_BY_ONE || Lair.Type[0] == LAIR_ONE_BY_ONE_PROX) {
            Lair.SpawnRate = 0;
        }
    }


    void RandomizeLairContents(vector<Lair> &LairList) {

        for (int LairIndex = 0; LairIndex < NUMBER_OF_LAIRS; LairIndex++) {

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



    void RandomizeMapSprites(vector<Sprite> &RandomizedSpriteList, fstream &ROMFile) {

        /* Initialize the list */
        vector<Sprite> OriginalSpriteList(NUMBER_OF_SPRITES);
        ROMData::GetOriginalMapSpriteData(OriginalSpriteList, ROMFile);
        RandomizedSpriteList = OriginalSpriteList;

        unsigned char Enemy;
        for (int SpriteIndex = 0; SpriteIndex < NUMBER_OF_SPRITES; ++SpriteIndex) {

            Enemy = RandomizedSpriteList[SpriteIndex].Enemy;

            switch (RandomizedSpriteList[SpriteIndex].Act) {
            case ACT_1:
                if (Enemy < ACT1_ARMOR) {
                    /* Sprite is in Underground Castle */
                    Enemy = UndergroundCastleEnemies[RandomInteger(6)];
                }
                else {
                    /* Sprite is in Leo's Paintings */
                    Enemy = LeosPaintingsEnemies[RandomInteger(4)];
                }
                break;
            case ACT_2:
                if (Enemy < ACT2_FIRE_SPIRIT) {
                    /* Sprite is in Lost Marsh or Water Shrine */
                    Enemy = WaterShrineEnemies[RandomInteger(5)];
                }
                else {
                    /* Sprite is in Fire Shrine or Light Shrine */
                    Enemy = FireLightShrineEnemies[RandomInteger(5)];
                }
                break;
            case ACT_3:
                if (Enemy < ACT3_PALM_TREE) {
                    /* Sprite is in Seabed */
                    Enemy = SeabedEnemies[RandomInteger(5)];
                }
                else {
                    /* Sprite is in Islands */
                    Enemy = IslandsEnemies[RandomInteger(5)];
                }
                break;
            case ACT_4:
                if (Enemy < ACT4_PURPLE_WIZARD) {
                    /* Sprite is in Mountain of Souls */
                    Enemy = MountainEnemies[RandomInteger(5)];
                }
                else {
                    /* Sprite is in Laynole or Lune */
                    Enemy = LaynoleLuneEnemies[RandomInteger(5)];
                }
                break;
            case ACT_5:
                if (Enemy < ACT5_MINI_KNIGHT) {
                    /* Sprite is in Leo's Lab Basement or Power Plant */
                    Enemy = LeosBasementEnemies[RandomInteger(5)];
                }
                else {
                    /* Sprite is in one of the Model Towns */
                    Enemy = ModelTownsEnemies[RandomInteger(5)];
                }
                break;
            case ACT_6:
                if (Enemy < ACT6_PURPLE_KNIGHT) {
                    /* Sprite is in Magridd Castle Basement */
                    Enemy = CastleBasementEnemiesFull[RandomInteger(6)];
                }
                else {
                    /* Sprite is in one of the Magridd Castle Towers */
                    Enemy = CastleTowersEnemiesFull[RandomInteger(7)];
                }
                break;
            case ACT_7:
                /* Sprite is in World of Evil */
                Enemy = WorldOfEvilEnemies[RandomInteger(3)];
                break;
            default:
                /* Should not happen! */
                break;
            }

            if (Enemy != RandomizedSpriteList[SpriteIndex].Enemy) {
                /* Reset orientation if the enemy type has changed */
                RandomizedSpriteList[SpriteIndex].Orientation = 0x01;
            }

            RandomizedSpriteList[SpriteIndex].Enemy = Enemy;
        }
    }



    void ExploreRegion(vector<Region> &RegionList,
                       vector<Goal>   &GoalList,
                       vector<int>    &AvailableRevivingLairs,
                       vector<int>    &AvailableItems,
                       vector<int>    &AvailableGoals,
                       bool            RevivedNPCs[],
                       bool            CollectedKeyItems[],
                       int             RegionIndex) {

        list<Element>::const_iterator ElementIterator;
        list<int>::const_iterator GoalIterator;
        int GoalIndex;
        bool KeepExploring, GoalFulfilled;

        do {

            KeepExploring = false;

            /* Explore the targeted region: store all the new available reviving lairs and items... */
            for (ElementIterator = RegionList[RegionIndex].Contents.begin();
                 ElementIterator != RegionList[RegionIndex].Contents.end();
                 ++ElementIterator) {
                if ((*ElementIterator).Type == LAIR) {
                    AvailableRevivingLairs.push_back((*ElementIterator).Index);
                }
                else {
                    AvailableItems.push_back((*ElementIterator).Index);
                }
            }

            /* ...and store all new available goals */
            for (GoalIterator = RegionList[RegionIndex].NextGoals.begin();
                 GoalIterator != RegionList[RegionIndex].NextGoals.end();
                 ++GoalIterator) {
                AvailableGoals.push_back(*GoalIterator);
            }

            /* Check the available goals, maybe some of them are already fulfilled */
            for (GoalIndex = 0; GoalIndex < (int)AvailableGoals.size(); GoalIndex++) {

                GoalFulfilled = true;

                /* Read the contents of this goal */
                for (ElementIterator = GoalList[AvailableGoals[GoalIndex]].Requirements.begin();
                     ElementIterator != GoalList[AvailableGoals[GoalIndex]].Requirements.end();
                     ++ElementIterator) {

                    if ( ((*ElementIterator).Type == LAIR && RevivedNPCs[(*ElementIterator).Index]       == false) ||
                         ((*ElementIterator).Type == ITEM && CollectedKeyItems[(*ElementIterator).Index] == false) ) {
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


    bool RandomizeProgression(vector<Lair>  &RandomizedLairList,
                              vector<Item>  &RandomizedItemList,
                              fstream       &ROMFile) {

        /* Get the original Lair and Item data */
        vector<Lair> OriginalLairList(NUMBER_OF_LAIRS);
        vector<Item> OriginalItemList(NUMBER_OF_ITEMS);
        ROMData::GetOriginalLairData(OriginalLairList, ROMFile);
        ROMData::GetOriginalItemData(OriginalItemList);

        vector<Region> RegionList(NUMBER_OF_REGIONS);
        vector<Goal>   GoalList(NUMBER_OF_GOALS);

        /* Get the map and calculate the goal weights */
        //Map::InitMap(RegionList, GoalList);
        Map::InitMap_v2(RegionList, GoalList);
        Map::CalculateWeights(RegionList, GoalList, GOAL_TO_FIRST_REGION);

#ifdef DEBUG
        cout << endl;
        for (int GoalIdx = 0; GoalIdx < NUMBER_OF_GOALS; ++GoalIdx) {
            cout << "Weight for Goal #" << GoalIdx << " -> " << GoalList[GoalIdx].Weight << endl;
        }
        cout << endl;
#endif

        vector<int> AvailableRevivingLairs;
        vector<int> AvailableItems;
        vector<int> AvailableGoals;
        vector<int> VillageChiefPossibleLairs;
        bool RevivedNPCs      [NUMBER_OF_LAIRS] = {false};
        bool CollectedKeyItems[NUMBER_OF_ITEMS] = {false};
        list<Element>::const_iterator ElementIterator;
        list<int>::const_iterator GoalIterator;

        int GoalIndex, RevivingLairIndex, ItemIndex, NewRegionIndex;
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
            //GoalIndex = RandomInteger(AvailableGoals.size());
            GoalIndex = WeightedGoalChoice(GoalList, AvailableGoals);

#ifdef DEBUG
            cout << "Available goals: ";
            for (unsigned int i=0; i<AvailableGoals.size(); ++i) {
                cout << AvailableGoals[i] << " ";
            }
            cout << endl;
            cout << endl;
            cout << "Available Lairs: ";
            for (unsigned int i=0; i<AvailableRevivingLairs.size(); ++i) {
                cout << AvailableRevivingLairs[i] << " ";
            }
            cout << endl;
            cout << "Available Items: ";
            for (unsigned int i=0; i<AvailableItems.size(); ++i) {
                cout << AvailableItems[i] << " ";
            }
            cout << endl;
            cout << endl;
            cout << "Picked goal " << AvailableGoals[GoalIndex] << ".\n";
#endif

            /* Read the contents of the goal */
            for (ElementIterator = GoalList[AvailableGoals[GoalIndex]].Requirements.begin();
                 ElementIterator != GoalList[AvailableGoals[GoalIndex]].Requirements.end();
                 ++ElementIterator) {

                if ((*ElementIterator).Type == LAIR) {

                    /* If this NPC is already revived, skip this requirement */
                    if (RevivedNPCs[(*ElementIterator).Index] == false) {

                        if (AvailableRevivingLairs.size() > 0) {

                            /* Choose one of the available reviving Monster Lairs and assign this NPC to it */

                            RevivingLairIndex = RandomInteger(AvailableRevivingLairs.size());

//                            if ( (*ElementIterator).Index == NPC_MERMAID_STATUE_ROCKBIRD   ||
//                                 (*ElementIterator).Index == NPC_MERMAID_STATUE_DUREAN     ||
//                                 (*ElementIterator).Index == NPC_MERMAID_STATUE_BLESTER    ||
//                                 (*ElementIterator).Index == NPC_MERMAID_STATUE_GHOST_SHIP ||
//                                 (*ElementIterator).Index == NPC_MERMAID_QUEEN ) {
//                                /* First Mermaid Statue problem fix: make sure these NPCs can only be found in Lairs from Act 3 or later */
//                                while ( (AvailableRevivingLairs[RevivingLairIndex] < NPC_DOLPHIN) ||
//                                        (AvailableRevivingLairs[RevivingLairIndex] == NPC_MERMAID_STATUE_ROCKBIRD) ) {
//                                    RevivingLairIndex = RandomInteger(AvailableRevivingLairs.size());
//                                }
//#ifdef DEBUG
//                                cout << "Mermaid special Lair : " << (*ElementIterator).Index << " <-- "
//                                    << AvailableRevivingLairs[RevivingLairIndex] << endl;
//#endif
//                            }

                            RandomizedLairList[(*ElementIterator).Index] = OriginalLairList[AvailableRevivingLairs[RevivingLairIndex]];

#ifdef DEBUG
                            cout << "Lair : " << (*ElementIterator).Index << " <-- " << AvailableRevivingLairs[RevivingLairIndex] << endl;
#endif

                            /* This NPC has been revived */
                            RevivedNPCs[(*ElementIterator).Index] = true;

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
                    if (CollectedKeyItems[(*ElementIterator).Index] == false) {

                        if (AvailableItems.size() > 0) {

                            /* Choose one of the available item locations and assign this item to it */
                            ItemIndex = RandomInteger(AvailableItems.size());
                            RandomizedItemList[AvailableItems[ItemIndex]] = OriginalItemList[(*ElementIterator).Index];

#ifdef DEBUG
                            cout << "Item: " << AvailableItems[ItemIndex] << " <-- " << (*ElementIterator).Index << endl;
#endif

                            /* This Key Item has been collected */
                            CollectedKeyItems[(*ElementIterator).Index] = true;

                            /* If this was Mountain King's item, remember it */
                            if (AvailableItems[ItemIndex] == ITEM_MOUNTAIN_KING) {
                                MountainKingItemRandomized = true;
                            }

                            /* This Item is no longer available */
                            AvailableItems.erase(AvailableItems.begin() + ItemIndex);
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
        vector<int> NonKeyNPCList;
        vector<int> NonKeyItemList;
        //Map::GetNonKeyNPCList (NonKeyNPCList);
        Map::GetNonKeyNPCList_v2 (NonKeyNPCList);
        //Map::GetNonKeyItemList(NonKeyItemList);
        Map::GetNonKeyItemList_v2(NonKeyItemList);

        /* Add the items/NPC locations which may not always be accessible,
           to make sure they still receive a non-key item/NPC. */
        AvailableItems.push_back(CHEST_LEOS_BRUSH);
        AvailableItems.push_back(17);
        AvailableItems.push_back(10);
        //AvailableItems.push_back(22);
        AvailableItems.push_back(ITEM_QUEEN_MAGRIDD);
        AvailableItems.push_back(ITEM_SOLDIER_PLATINUM_CARD);
        AvailableRevivingLairs.push_back(NPC_OLD_MAN);

#ifdef DEBUG
        cout << AvailableRevivingLairs.size() << " available Reviving Lairs, " << NonKeyNPCList.size() << " in NPC list.\n";
        cout << AvailableItems.size() << " available Items, " << NonKeyItemList.size() << " in Item list.\n";
#endif

        /* Make sure Mountain King receives an item different from a Medical Herb, a Strange Bottle, Gems/EXP or nothing */
        if (MountainKingItemRandomized == false) {
            ItemIndex = RandomInteger(NonKeyItemList.size());
            while (OriginalItemList[NonKeyItemList[ItemIndex]].Contents == MEDICAL_HERB ||
                   OriginalItemList[NonKeyItemList[ItemIndex]].Contents == STRANGE_BOTTLE ||
                   OriginalItemList[NonKeyItemList[ItemIndex]].Contents == GEMS_EXP ||
                   OriginalItemList[NonKeyItemList[ItemIndex]].Contents == NOTHING) {
                ItemIndex = RandomInteger(NonKeyItemList.size());
            }
            RandomizedItemList[ITEM_MOUNTAIN_KING] = OriginalItemList[NonKeyItemList[ItemIndex]];
            NonKeyItemList.erase(NonKeyItemList.begin() + ItemIndex);
        }

        /* Shuffle these lists */
        random_shuffle(NonKeyNPCList.begin(),  NonKeyNPCList.end(),  RandomInteger);
        random_shuffle(NonKeyItemList.begin(), NonKeyItemList.end(), RandomInteger);

        for (RevivingLairIndex = 0; RevivingLairIndex < (int)AvailableRevivingLairs.size(); RevivingLairIndex++) {
            /* Fill this lair and remove it from the list */
            RandomizedLairList[NonKeyNPCList[RevivingLairIndex]] = OriginalLairList[AvailableRevivingLairs[RevivingLairIndex]];
        }

        int Offset = 0;
        for (ItemIndex = 0; ItemIndex < (int)AvailableItems.size(); ItemIndex++) {

            /* Mountain King's Item has already been assigned */
            if (AvailableItems[ItemIndex] == ITEM_MOUNTAIN_KING) {
                Offset = 1;
                continue;
            }

            /* Fill this item and remove it from the list */
            RandomizedItemList[AvailableItems[ItemIndex]] = OriginalItemList[NonKeyItemList[ItemIndex-Offset]];
        }

        /* Randomization successful! */
        return true;
    }


    int WeightedGoalChoice(vector<Goal>  &GoalList,
                           vector<int>   &AvailableGoals) {

        int SumOfWeights = 0;
        unsigned int GoalIndex;

        /* Add up all the weights */
        for (GoalIndex = 0; GoalIndex < AvailableGoals.size(); ++GoalIndex) {
            SumOfWeights += GoalList[AvailableGoals[GoalIndex]].Weight;
        }

        /* Pick a number and determine which Goal it corresponds to */
        int RandomNumber = RandomInteger(SumOfWeights);
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
}
