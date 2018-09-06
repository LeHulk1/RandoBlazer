#include "MapDataTools.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fstream>
#include <iostream>


#define MAP_FILE_NAME  "MapToCompress.txt"
#define ROM_FILE_NAME  "Soul Blazer (U) [!].smc"

#define SEARCH_LOG2  8
#define SEARCH_SIZE  256
#define LOOKAHEAD_LOG2  4
#define LOOKAHEAD_SIZE  16

#define MAX_BYTES  10000

#define MIN(x,y) (x<y?x:y)

#define WRITE_BYTE_IN_BINARY(x) { \
    bool a = x & 0x80;  \
    bool b = x & 0x40;  \
    bool c = x & 0x20;  \
    bool d = x & 0x10;  \
    bool e = x & 0x08;  \
    bool f = x & 0x04;  \
    bool g = x & 0x02;  \
    bool h = x & 0x01;  \
    OutputFile << a << b << c << d << e << f << g << h; \
}

#define WRITE_BYTE_IN_4_BITS(x) { \
    bool e = x & 0x08;  \
    bool f = x & 0x04;  \
    bool g = x & 0x02;  \
    bool h = x & 0x01;  \
    OutputFile << e << f << g << h; \
}

#define COPY_BIT(bit) { \
    if (bit!=0) {OutputBytes[CurrentOutputByte] += PositionMask[CurrentOutputBit];} \
    if (CurrentOutputBit == 7) {CurrentOutputBit = 0; CurrentOutputByte++;} else {CurrentOutputBit++;} \
}

#define COPY_4_BITS(x) { \
    bool e = x & 0x08; COPY_BIT(e); \
    bool f = x & 0x04; COPY_BIT(f); \
    bool g = x & 0x02; COPY_BIT(g); \
    bool h = x & 0x01; COPY_BIT(h); \
}

#define COPY_BYTE(x) { \
    bool a = x & 0x80; COPY_BIT(a); \
    bool b = x & 0x40; COPY_BIT(b); \
    bool c = x & 0x20; COPY_BIT(c); \
    bool d = x & 0x10; COPY_BIT(d); \
    bool e = x & 0x08; COPY_BIT(e); \
    bool f = x & 0x04; COPY_BIT(f); \
    bool g = x & 0x02; COPY_BIT(g); \
    bool h = x & 0x01; COPY_BIT(h); \
}


typedef unsigned char   byte;
using namespace std;

static int PositionMask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

namespace MapDataTools {

    void Compress(void) {


        // Open the file.
        FILE *MapFile;
        MapFile = fopen(MAP_FILE_NAME, "rb");
        if (!MapFile) {
            printf("Error: File %s not found\n", MAP_FILE_NAME);
            exit(1);
        }

        ofstream OutputFile("MapComp.txt");

        OutputFile << "***************\n";
        OutputFile << "* Binary dump *\n";
        OutputFile << "***************\n";
        OutputFile << endl;


        // Read the bytes.
        byte tocomp[MAX_BYTES];
        int BufferSize = 0;
        while (BufferSize < MAX_BYTES) {
            byte input = fgetc(MapFile);
            if (feof(MapFile)) {break;}
            tocomp[BufferSize + SEARCH_SIZE] = input;
            WRITE_BYTE_IN_BINARY(input);
            BufferSize++;
        }
        fclose(MapFile);

        OutputFile << endl;
        OutputFile << endl;
        OutputFile << "Number of bytes: " << BufferSize << ".";


        OutputFile << endl;
        OutputFile << endl;
        OutputFile << endl;
        OutputFile << "*********************\n";
        OutputFile << "* Compressed output *\n";
        OutputFile << "*********************\n";
        OutputFile << endl;


        int currentIndex = SEARCH_SIZE;
        int i, j, bestIndex, bestLength, compareLimit, currentLength;

        // Prepare memory buffer: left-pad with with 0x20 values
        for (i = 0; i < SEARCH_SIZE; i++) {
            tocomp[i] = 0x20;
        }
        BufferSize += SEARCH_SIZE;
        int CurrentOutputBit = 0;
        int CurrentOutputByte = 0;

        // Compressed map in byte format
        byte OutputBytes[MAX_BYTES];

        // Main compression loop.
        while (currentIndex < BufferSize) {
            bestIndex = 0;
            bestLength = 0;

            // Look for a match in the search buffer. (Brute force)
            for (i = 0; i < SEARCH_SIZE; i++) {
                // Don't compare past the end of the lookahead buffer.
                // Don't compare past the end of the memory buffer.
                compareLimit = MIN(
                  LOOKAHEAD_SIZE + 1,
                  BufferSize - currentIndex
                );

                // Compare the search buffer to the lookahead buffer.
                // Count how many sequential bytes match (possibly zero).
                currentLength = 0;
                for (j = 0; j < compareLimit; j++) {
                    if (tocomp[currentIndex - SEARCH_SIZE + i + j] == tocomp[currentIndex + j]) {
                        currentLength += 1;
                    }
                    else {
                        break;
                    }
                }

                // Keep track of the largest match we've seen.
                if (currentLength > bestLength) {
                    bestIndex = currentIndex - SEARCH_SIZE + i;
                    bestLength = currentLength;
                }
            }

            // Write the next block of compressed output.
            if (bestLength >= 2) {
                // For some reason, the decompressor expects the pastcopy
                // source values to be offset by 0xEF. I have no idea why.

//                OutputFile << endl;
//                OutputFile << "Pastcopy -> Index " << bestIndex % 256 << " (";
//                WRITE_BYTE_IN_BINARY((byte)((bestIndex + 0xEF) & 0xFF));
//                OutputFile << ") / Length " << bestLength << " (";
//                WRITE_BYTE_IN_BINARY((byte)(bestLength - 2));
//                OutputFile << ")\n";
//                OutputFile << endl;


                bestIndex = (bestIndex + 0xEF) & 0xFF;
                OutputFile << 0;
                WRITE_BYTE_IN_BINARY((byte)(bestIndex));
                WRITE_BYTE_IN_4_BITS((byte)(bestLength - 2));

                COPY_BIT(0);
                COPY_BYTE((byte)(bestIndex));
                COPY_4_BITS((byte)(bestLength - 2));

//                OutputFile << endl;

                currentIndex += bestLength;
            }
            else {

//                OutputFile << endl;
//                OutputFile << "Literal -> Byte ";
//                WRITE_BYTE_IN_BINARY(tocomp[currentIndex]);
//                OutputFile << endl;
//                OutputFile << endl;

                OutputFile << 1;
                WRITE_BYTE_IN_BINARY(tocomp[currentIndex]);

                COPY_BIT(1);
                COPY_BYTE(tocomp[currentIndex]);

//                OutputFile << endl;

                currentIndex += 1;
            }
        }

        OutputFile << endl;
        OutputFile << endl;
        for (i = 0; i < CurrentOutputByte + (CurrentOutputBit == 0 ? 0 : 1); i++) {
            OutputFile << OutputBytes[i];
        }

        OutputFile << endl;
        OutputFile << endl;
        OutputFile << "Number of bytes : " << CurrentOutputByte + (CurrentOutputBit == 0 ? 0 : 1) << endl;


        OutputFile.close();
        OutputFile.clear();

    }






    void Decompress(int MapDataAddress) {

                /*
        Soul Blazer map decompressor.
        This version outputs:
          - A detailed breakdown of the decomp process
          - An ASCII map of the decompressed data
        */


          // Variables
          int width;
          int height;
          int length;
          int bytesout;
          int pos;
          int i, j, k, m;

          // Open the file.
          FILE *ROMFile;
          ROMFile = fopen(ROM_FILE_NAME, "rb");
          if (!ROMFile) {
            printf("Error: File %s not found\n", ROM_FILE_NAME);
            exit(1);
          }

          // Get to the proper location, and read the width, height and length.
          fseek(ROMFile, MapDataAddress, SEEK_SET);
          width = fgetc(ROMFile);
          height = fgetc(ROMFile);
          length = fgetc(ROMFile);
          length += (0x100 * fgetc(ROMFile));
          if (length <= 0) {
            printf("Error: According to that data, the length is %d.\n", length);
            exit(1);
          }

          ofstream OutputFile("MapDecomp.txt");

          // Print header data.
          OutputFile << MapDataAddress << endl;
          OutputFile << "Width  = " << width << endl;
          OutputFile << "Height = " << height << endl;
          OutputFile << "Length = " << length << endl;

          OutputFile << endl;
          OutputFile << endl;
          OutputFile << endl;
          OutputFile << "***************\n";
          OutputFile << "* Binary dump *\n";
          OutputFile << "***************\n";
          OutputFile << endl;

          // Read the bytes.
          bool todecomp[8 * length];
          for (i = 0; i < length; i++) {
            byte input = fgetc(ROMFile);
            todecomp[0 + 8*i] = input & 0x80;
            todecomp[1 + 8*i] = input & 0x40;
            todecomp[2 + 8*i] = input & 0x20;
            todecomp[3 + 8*i] = input & 0x10;
            todecomp[4 + 8*i] = input & 0x08;
            todecomp[5 + 8*i] = input & 0x04;
            todecomp[6 + 8*i] = input & 0x02;
            todecomp[7 + 8*i] = input & 0x01;
            OutputFile << todecomp[0 + 8*i] << todecomp[1 + 8*i] << todecomp[2 + 8*i] << todecomp[3 + 8*i]
                       << todecomp[4 + 8*i] << todecomp[5 + 8*i] << todecomp[6 + 8*i] << todecomp[7 + 8*i];
          }

          OutputFile << endl;
          OutputFile << endl;

          fseek(ROMFile, MapDataAddress, SEEK_SET);
          fgetc(ROMFile); fgetc(ROMFile); fgetc(ROMFile);
          for (i = 0; i < length; i++) {
            byte input = fgetc(ROMFile);
            OutputFile << input;
          }


          fclose(ROMFile);

          bytesout = 0;
          pos = 0;

          // A place to store the output...
          byte* wram = (byte*)malloc(length * sizeof(byte));
          if (wram == NULL) {
            printf("Error: Could not allocate memory.\n");
            exit(1);
          }



          // Parse!
          OutputFile << endl;
          OutputFile << endl;
          OutputFile << endl;
          OutputFile << "*****************\n";
          OutputFile << "* Decompression *\n";
          OutputFile << "*****************\n";
          OutputFile << endl;
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
              OutputFile << MapDataAddress+(pos/8)+4 << "." << 7 - pos%8 << "   ";
              // Type
              OutputFile << "1 ";
              // Bits
              OutputFile << todecomp[pos + 1];
              OutputFile << todecomp[pos + 2];
              OutputFile << todecomp[pos + 3];
              OutputFile << todecomp[pos + 4];
              OutputFile << todecomp[pos + 5];
              OutputFile << todecomp[pos + 6];
              OutputFile << todecomp[pos + 7];
              OutputFile << todecomp[pos + 8];
              // Byte
              OutputFile << "     ";
              OutputFile << "   lit " << curbyte << "       = " << 0x7E8000 + bytesout << ": ";
              OutputFile << curbyte;

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
              OutputFile << MapDataAddress+(pos/8)+4 << "." << 7 - pos%8 << "   ";
              // Type
              OutputFile << "0 ";
              // Bits
              OutputFile << todecomp[pos + 1];
              OutputFile << todecomp[pos + 2];
              OutputFile << todecomp[pos + 3];
              OutputFile << todecomp[pos + 4];
              OutputFile << todecomp[pos + 5];
              OutputFile << todecomp[pos + 6];
              OutputFile << todecomp[pos + 7];
              OutputFile << todecomp[pos + 8];
              OutputFile << " ";
              OutputFile << todecomp[pos +  9];
              OutputFile << todecomp[pos + 10];
              OutputFile << todecomp[pos + 11];
              OutputFile << todecomp[pos + 12];
              // Byte and repetitions
              OutputFile << "  (src " << fromoffset << ", " << bytestowrite
                       << ") = " << 0x7E8000 + bytesout << ": ";

              // Copy the bytes
              fromoffset += ((bytesout >> 8) << 8);
              if (fromoffset >= bytesout) {
                fromoffset -= 0x100;
              }
              for (i = 0; i < bytestowrite; i++) {
                wram[bytesout + i] = wram[fromoffset + i];
                OutputFile << wram[bytesout + i] << " ";
              }

              // Update
              bytesout += bytestowrite;
              pos += 13;
            }
            OutputFile << endl;
          }



          // ASCII map output.
          OutputFile << endl;
          OutputFile << endl;
          OutputFile << endl;
          OutputFile << "*************\n";
          OutputFile << "* ASCII map *\n";
          OutputFile << "*************\n";
          OutputFile << endl;
          byte control_char_fix;
          while (bytesout > 0) {
            for (i = 0; i < height; i++) {
              for (j = 0; j < 16; j++) {

                // One line of the map: START
                for (k = 0; k < width; k++) {
                  for (m = 0; m < 16; m++) {
                    control_char_fix = wram[(0x100 * width * i) + (0x10 * j) + (0x100 * k) + m];
        //            control_char_fix &= 0x7F;
        //            if (control_char_fix < 0x20) {
        //              control_char_fix += 0x20;
        //            }
                    OutputFile << control_char_fix;
                    bytesout--;
                  }
                }
                OutputFile << endl;
                // One line of the map: END

              }
            }
          }

          OutputFile << endl;
          OutputFile << endl;
          OutputFile << endl;
          OutputFile << "****************************\n";
          OutputFile << "* Decompressed binary dump *\n";
          OutputFile << "****************************\n";
          OutputFile << endl;
          for (i = 0; i < length; i++) {
            WRITE_BYTE_IN_BINARY(wram[i]);
          }
          OutputFile << endl;
          OutputFile << endl;


          // Cleanup.
          // pos is where you would be to read the next bit, so subtract one to get the actual last bit used.
          // The +4 is for the header (width, height, length).
          free(wram);
          OutputFile << endl;
          OutputFile << "Last byte (inclusive): " << MapDataAddress + ((pos - 1)/8) + 4;
          OutputFile << endl;

          OutputFile.close();
          OutputFile.clear();
    }

}
