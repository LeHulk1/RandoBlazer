#include "ItemHolder.h"
#include "Lair.h"
#include "Log.h"
#include "Random.h"
#include "Randomizer.h"
#include "ROMUpdate.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>


#define ROM_FILE_NAME      "Soul Blazer (U) [!].smc"
#define MOD_ROM_FILE_NAME  "Soul Blazer Randomized.smc"
#define SEED_FILE_NAME     "seed"

#define SEED_SIZE  10


using namespace std;

int main ( int argc, char** argv ) {

    /* Check that the ROM file is there, make the fstream instance */
    fstream ROMFile(ROM_FILE_NAME, ios::in | ios::out | ios::binary | ios::ate);
    if (!ROMFile.is_open()) {
        cout << "ROM file \"" << ROM_FILE_NAME << "\" is not found!\n";
        return 1;
    }
    ROMFile.close();
    ROMFile.clear();

    /* Initialize randomness with provided seed, if any */
    ifstream SeedFile(SEED_FILE_NAME, ios::in | ios::binary);
    if (!SeedFile.is_open()) {
        long Seed = Random::RandomInit(0);
        ofstream NewSeedFile(SEED_FILE_NAME);
        NewSeedFile << Seed;
        NewSeedFile.close();
    }
    else {
        char SeedChar[SEED_SIZE+1];
        long SeedInt;
        SeedFile.seekg (0, ios::beg);
        SeedFile.readsome(SeedChar, SEED_SIZE);
        SeedChar[SEED_SIZE] = '\0';
        SeedInt = atoi(SeedChar);
        Random::RandomInit(SeedInt);
    }
    SeedFile.close();

    /* Rename the ROM to be modified */
    int RenameResult = rename(ROM_FILE_NAME, MOD_ROM_FILE_NAME);
    if (RenameResult != 0) {
        cout << "Failure renaming ROM file!\n";
    }

    /* Back up the original ROM */
    ifstream ROMFileOriginal(MOD_ROM_FILE_NAME, ios::binary);
    ofstream ROMFileCopy    (ROM_FILE_NAME,     ios::binary);
    ROMFileCopy << ROMFileOriginal.rdbuf();

    /* Initialize the final lists of randomized lairs and chests */
    vector<Lair>  RandomizedLairList;
    vector<ItemHolder> RandomizedItemList;

    /* Re-open ROM to be modified */
    ROMFile.open(MOD_ROM_FILE_NAME, ios::in | ios::out | ios::binary | ios::ate);

    /* Call the main algorithm to randomize the progression through the game:
       ==> randomize chests contents and revived NPCs */
    bool RandomizationStatus = Randomizer::RandomizeProgression(RandomizedLairList,
                                                                RandomizedItemList,
                                                                ROMFile);
    if (!RandomizationStatus) {
        cout << "Randomization failed!\n";
        return 1;
    }

    /* Randomize monster lair contents: enemy types, lair types, number of enemies and spawn rates */
    Randomizer::RandomizeLairContents(RandomizedLairList);

    /* Modify the ROM with the randomized lists */
    ROMUpdate::ROMUpdateLairs(RandomizedLairList, ROMFile);
    ROMUpdate::ROMUpdateItems(RandomizedItemList, ROMFile);

    /* Generate the Spoiler Log */
    Log::CreateSpoilerLog(RandomizedLairList, RandomizedItemList);

    /* Close the ROM file */
    ROMFile.close();

    return 0;
}
