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


typedef unsigned char   byte;
using namespace std;

int main ( int argc, char** argv ) {






    /*
Soul Blazer map decompressor.
This version outputs:
  - A detailed breakdown of the decomp process
  - An ASCII map of the decompressed data
*/









  // Variables
  int offset;
  int width;
  int height;
  int length;
  int bytesout;
  int pos;
  int i, j, k, m;



  // Copy arguments to the variables
  offset = 0x4DEE3;

  // Open the file.
  FILE *rom;
  rom = fopen(ROM_FILE_NAME, "rb");
  if (!rom) {
    printf("Error: File %s not found\n", ROM_FILE_NAME);
    exit(1);
  }

  // Get to the proper location, and read the width, height and length.
  fseek(rom, offset, SEEK_SET);
  width = fgetc(rom);
  height = fgetc(rom);
  length = fgetc(rom);
  length += (0x100 * fgetc(rom));
  if (length <= 0) {
    printf("Error: According to that data, the length is %d.\n", length);
    exit(1);
  }

  ofstream TestFile("TestFile.txt");

  // Print header data.
  TestFile << offset << endl;
  TestFile << "Width  = " << width << endl;
  TestFile << "Height = " << height << endl;
  TestFile << "Length = " << length << endl;

  // Read the bytes.
  bool todecomp[8 * length];
  for (i = 0; i < length; i++) {
    byte input = fgetc(rom);
    todecomp[0 + 8*i] = input & 0x80;
    todecomp[1 + 8*i] = input & 0x40;
    todecomp[2 + 8*i] = input & 0x20;
    todecomp[3 + 8*i] = input & 0x10;
    todecomp[4 + 8*i] = input & 0x08;
    todecomp[5 + 8*i] = input & 0x04;
    todecomp[6 + 8*i] = input & 0x02;
    todecomp[7 + 8*i] = input & 0x01;
  }
  fclose(rom);
  bytesout = 0;
  pos = 0;

  // A place to store the output...
  byte* wram = (byte*)malloc(length * sizeof(byte));
  if (wram == NULL) {
    printf("Error: Could not allocate memory.\n");
    exit(1);
  }



  // Parse!
  TestFile << endl;
  TestFile << endl;
  TestFile << endl;
  TestFile << "*****************\n";
  TestFile << "* Decompression *\n";
  TestFile << "*****************\n";
  TestFile << "\n";
  while (bytesout < length) {
    if (todecomp[pos] == 1) {
      // 1: Literal case.
      byte curbyte = 0;
      curbyte += (todecomp[pos + 1] * 0x80);
      curbyte += (todecomp[pos + 2] * 0x40);
      curbyte += (todecomp[pos + 3] * 0x20);
      curbyte += (todecomp[pos + 4] * 0x10);
      curbyte += (todecomp[pos + 5] * 0x08);
      curbyte += (todecomp[pos + 6] * 0x04);
      curbyte += (todecomp[pos + 7] * 0x02);
      curbyte += (todecomp[pos + 8] * 0x01);
      wram[bytesout] = curbyte;

      // Position and offset
      TestFile << offset+(pos/8)+4 << "." << 7 - pos%8 << "   ";
      // Type
      TestFile << "1 ";
      // Bits
      TestFile << todecomp[pos + 1];
      TestFile << todecomp[pos + 2];
      TestFile << todecomp[pos + 3];
      TestFile << todecomp[pos + 4];
      TestFile << todecomp[pos + 5];
      TestFile << todecomp[pos + 6];
      TestFile << todecomp[pos + 7];
      TestFile << todecomp[pos + 8];
      // Byte
      TestFile << "     ";
      TestFile << "   lit " << curbyte << "       = " << 0x7E8000 + bytesout << ": ";
      TestFile << curbyte;

      // Update
      bytesout += 1;
      pos += 9;
    } else {
      // 0: Repeat case.
      int bytestowrite = 0;
      bytestowrite += (todecomp[pos +  9] * 0x8);
      bytestowrite += (todecomp[pos + 10] * 0x4);
      bytestowrite += (todecomp[pos + 11] * 0x2);
      bytestowrite += (todecomp[pos + 12] * 0x1);
      bytestowrite += 2;

      // Counting starts from 0xEF. Why? Who knows?
      int fromoffset = 0;
      fromoffset += (todecomp[pos + 1] * 0x80);
      fromoffset += (todecomp[pos + 2] * 0x40);
      fromoffset += (todecomp[pos + 3] * 0x20);
      fromoffset += (todecomp[pos + 4] * 0x10);
      fromoffset += (todecomp[pos + 5] * 0x08);
      fromoffset += (todecomp[pos + 6] * 0x04);
      fromoffset += (todecomp[pos + 7] * 0x02);
      fromoffset += (todecomp[pos + 8] * 0x01);
      fromoffset = (fromoffset + 0x11) % 0x100;

      // Position and offset
      TestFile << offset+(pos/8)+4 << "." << 7 - pos%8 << "   ";
      // Type
      TestFile << "0 ";
      // Bits
      TestFile << todecomp[pos + 1];
      TestFile << todecomp[pos + 2];
      TestFile << todecomp[pos + 3];
      TestFile << todecomp[pos + 4];
      TestFile << todecomp[pos + 5];
      TestFile << todecomp[pos + 6];
      TestFile << todecomp[pos + 7];
      TestFile << todecomp[pos + 8];
      TestFile << " ";
      TestFile << todecomp[pos +  9];
      TestFile << todecomp[pos + 10];
      TestFile << todecomp[pos + 11];
      TestFile << todecomp[pos + 12];
      // Byte and repetitions
      TestFile << "  (src " << fromoffset << ", " << bytestowrite
               << ") = " << 0x7E8000 + bytesout << ": ";

      // Copy the bytes
      fromoffset += ((bytesout >> 8) << 8);
      if (fromoffset >= bytesout) {
        fromoffset -= 0x100;
      }
      for (i = 0; i < bytestowrite; i++) {
        wram[bytesout + i] = wram[fromoffset + i];
        TestFile << wram[bytesout + i] << " ";
      }

      // Update
      bytesout += bytestowrite;
      pos += 13;
    }
    TestFile << endl;
  }



  // ASCII map output.
  TestFile << endl;
  TestFile << endl;
  TestFile << endl;
  TestFile << "*************\n";
  TestFile << "* ASCII map *\n";
  TestFile << "*************\n";
  TestFile << endl;
  byte control_char_fix;
  while (bytesout > 0) {
    for (i = 0; i < height; i++) {
      for (j = 0; j < 16; j++) {

        // One line of the map: START
        for (k = 0; k < width; k++) {
          for (m = 0; m < 16; m++) {
            control_char_fix = wram[(0x100 * width * i) + (0x10 * j) + (0x100 * k) + m];
            control_char_fix &= 0x7F;
            if (control_char_fix < 0x20) {
              control_char_fix += 0x20;
            }
            TestFile << control_char_fix;
            bytesout--;
          }
        }
        TestFile << endl;
        // One line of the map: END

      }
    }
  }

  // Cleanup.
  // pos is where you would be to read the next bit, so subtract one to get the actual last bit used.
  // The +4 is for the header (width, height, length).
  free(wram);
  TestFile << endl;
  TestFile << "Last byte (inclusive): " << offset + ((pos - 1)/8) + 4;
  TestFile << endl;
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
