#include "Item.h"
#include "Lair.h"
#include "Log.h"
#include "MapDataTools.h"
#include "Random.h"
#include "Randomizer.h"
#include "ROMCheck.h"
#include "ROMUpdate.h"
#include "Sprite.h"

#include <fstream>
#include <iostream>

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ROM_FILE_NAME      "Soul Blazer (U) [!].smc"
#define MOD_ROM_FILE_NAME  "Soul Blazer Randomized.smc"
#define SEED_FILE_NAME     "seed"

#define HEADER_OFFSET        0x200
#define SEED_SIZE            10
#define MAX_NUMBER_OF_TRIES  5

//#define DONT_CREATE_SEED_FILE



using namespace std;

int main ( int argc, char** argv ) {



    MapDataTools::Compress();
    MapDataTools::Decompress(0xF1694);




    return 0;










    /****************************\
    |*  Check the original ROM  *|
    \****************************/

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


    /*********************\
    |*  Check Seed file  *|
    \*********************/

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
    SeedFile.clear();


    /***************************************************\
    |*  Delete old modified ROM / backup original ROM  *|
    \***************************************************/

    /* If a modified ROM exists, delete it first */
    int RemoveResult = remove(MOD_ROM_FILE_NAME);
    if (RemoveResult == 0) {
        cout << "Previous randomized ROM detected and removed.\n";
    }

    /* Back up the original ROM */
    ifstream ROMFileOriginal(ROM_FILE_NAME, ios::binary);
    ofstream ROMFileCopy    (MOD_ROM_FILE_NAME,     ios::binary);

    ROMFileOriginal.seekg(0, ios::end);
    long ROMFileSize = ROMFileOriginal.tellg();
    if (OriginalROMStatus == HEADERED) {
        /* For headered ROM, ignore the first 512 bytes */
        ROMFileSize -= HEADER_OFFSET;
    }

    if(ROMFileOriginal.is_open() && ROMFileCopy.is_open()) {
		unsigned char *DataBuffer = new unsigned char[ROMFileSize];
		if (OriginalROMStatus == HEADERED) {
		    /* For headered ROM, ignore the first 512 bytes */
            ROMFileOriginal.seekg(HEADER_OFFSET, ios::beg);
        }
        else {
            ROMFileOriginal.seekg(0, ios::beg);
        }
		ROMFileOriginal.read((char*)DataBuffer, ROMFileSize);
		ROMFileCopy.write((char*)DataBuffer, ROMFileSize);
		delete[] DataBuffer;
	}
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
	ROMFileOriginal.clear();
	ROMFileCopy.clear();


	/****************\
    |*  Randomize!  *|
    \****************/

    cout << endl;
    cout << "Starting randomization.\n";

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
        cout << " . . . Randomization failed!\n";
        return 1;
    }
    cout << " . . . Randomization succeeded in " << RandomizationTry + 1
         << (RandomizationTry == 0 ? " try.\n" : " tries.\n");

    cout << "Starting ROM modification.\n";

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
                                     Seed);

    /* Close the ROM file */
    ROMFile.close();
    ROMFile.clear();

    cout << " . . . ROM modification complete.\n";

    cout << "Starting Spoiler Log creation.\n";

    /* Generate the Spoiler Log */
    Log::CreateSpoilerLog(RandomizedLairList, RandomizedItemList);

    cout << " . . . Spoiler Log created.\n";

    /* Ask user to press any key to close the program */
    cout << endl;
    cout << "RandoBlazer execution was successful!\n"
         << "Press any key to exit the program.\n";

    _getch();

    return 0;
}
