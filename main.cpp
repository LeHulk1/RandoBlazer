#include "Item.h"
#include "Lair.h"
#include "Log.h"
#include "Random.h"
#include "Randomizer.h"
#include "ROMCheck.h"
#include "ROMUpdate.h"
#include "Sprite.h"
#include "TextUpdate.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>


#define ROM_FILE_NAME      "Soul Blazer (U) [!].smc"
#define MOD_ROM_FILE_NAME  "Soul Blazer Randomized.smc"
#define SEED_FILE_NAME     "seed"

#define SEED_SIZE            10
#define MAX_NUMBER_OF_TRIES  5

//#define DONT_CREATE_SEED_FILE



using namespace std;

int main ( int argc, char** argv ) {

    ROMStatus OriginalROMStatus;

    /* Check that the ROM file is there, make the fstream instance */
    fstream ROMFile(ROM_FILE_NAME, ios::in | ios::out | ios::binary | ios::ate);
    if (!ROMFile.is_open()) {
        cout << "ROM file \"" << ROM_FILE_NAME << "\" is not found!\n";
        return 1;
    }
    else {
        /* Check if this is the headered or the un-headered ROM */
        OriginalROMStatus = CheckOriginalROM(ROMFile);
        if (OriginalROMStatus == UNKNOWN) {
            cout << "File \"" << ROM_FILE_NAME << "\" is not a known original Soul Blazer (U) ROM!\n";
            return 1;
        }
        else {
            cout << "This ROM is " << (OriginalROMStatus == UNHEADERED ? "un-" : "") << "headered.\n";
        }
    }
    ROMFile.close();
    ROMFile.clear();

    long Seed = 0;

    /* Initialize randomness with provided seed, if any */
    ifstream SeedFile(SEED_FILE_NAME, ios::in | ios::binary);
    if (!SeedFile.is_open()) {
        Seed = Random::RandomInit(0);
#ifndef DONT_CREATE_SEED_FILE
        ofstream NewSeedFile(SEED_FILE_NAME);
        NewSeedFile << Seed;
        NewSeedFile.close();
#endif // DONT_CREATE_SEED_FILE
    }
    else {
        char SeedChar[SEED_SIZE+1];
        SeedFile.seekg(0, ios::beg);
        SeedFile.readsome(SeedChar, SEED_SIZE);
        SeedChar[SEED_SIZE] = '\0';
        Seed = atoi(SeedChar);
        Random::RandomInit(Seed);
    }
    SeedFile.close();

    /* If a modified ROM exists, delete it first */
    int RemoveResult = remove(MOD_ROM_FILE_NAME);
    if (RemoveResult == 0) {
        cout << "Previous randomized ROM detected and removed.\n";
    }

    /* Rename the ROM to be modified */
//    int RenameResult = rename(ROM_FILE_NAME, MOD_ROM_FILE_NAME);
//    if (RenameResult != 0) {
//        cout << "Failure renaming ROM file!\n";
//    }

    /* Back up the original ROM */
    ifstream ROMFileOriginal(ROM_FILE_NAME, ios::binary);
    ofstream ROMFileCopy    (MOD_ROM_FILE_NAME,     ios::binary);



    ROMFileOriginal.seekg(0, ios::end);
    long fileSize = ROMFileOriginal.tellg();
    if (OriginalROMStatus == HEADERED) {
        fileSize -= 0x200;
        }
    cout << "Buffer size = " << fileSize;
    if(ROMFileOriginal.is_open() && ROMFileCopy.is_open()) {
		unsigned char * buffer = new unsigned char[fileSize];
		//Determine the file's size
		//Then starts from the beginning
		if (OriginalROMStatus == HEADERED) {
        ROMFileOriginal.seekg(0x200, ios::beg);
        }
        else {
            ROMFileOriginal.seekg(0, ios::beg);
        }
		//Then read enough of the file to fill the buffer
		ROMFileOriginal.read((char*)buffer, fileSize);
		//And then write out all that was read
		ROMFileCopy.write((char*)buffer, fileSize);
		delete[] buffer;
	}
	//If there were any problems with the copying process, let the user know
	else if(!ROMFileCopy.is_open())	{
		cout << "Failure backing up the ROM!\n";
		return 1;
	}
	else if(!ROMFileOriginal.is_open())	{
		cout << "Failure opening the original ROM for copying!\n";
		return 1;
	}

	ROMFileOriginal.close();
	ROMFileCopy.close();

    //ROMFileCopy << ROMFileOriginal.rdbuf();

    /* Initialize the final lists of randomized lairs, chests and sprites */
    vector<Lair> RandomizedLairList;
    vector<Item> RandomizedItemList;
    vector<Sprite> RandomizedSpriteList;

    /* Re-open ROM to be modified */
    ROMFile.open(MOD_ROM_FILE_NAME, ios::in | ios::out | ios::binary | ios::ate);

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
        cout << "Randomization failed!\n";
        return 1;
    }

    /* Randomize monster lair contents: enemy types, lair types, number of enemies and spawn rates */
    Randomizer::RandomizeLairContents(RandomizedLairList);

    /* Randomize static enemies in maps */
    Randomizer::RandomizeMapSprites(RandomizedSpriteList, ROMFile);

    /* Modify the ROM with the randomized lists */
    ROMUpdate::ROMUpdateLairs(RandomizedLairList, ROMFile);
    ROMUpdate::ROMUpdateItems(RandomizedItemList, ROMFile);
    ROMUpdate::ROMUpdateMapSprites(RandomizedSpriteList, ROMFile);

    /* General text modification */
    TextUpdate::GeneralTextUpdate(ROMFile, Seed);

    /* Generate the Spoiler Log */
    Log::CreateSpoilerLog(RandomizedLairList, RandomizedItemList);

    /* Close the ROM file */
    ROMFile.close();

    return 0;
}
