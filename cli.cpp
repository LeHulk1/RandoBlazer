#include "Randomizer.h"
#include "Random.h"

#include <stdlib.h>

#define ROM_FILE_NAME      "Soul Blazer (U) [!].smc"
#define MOD_ROM_FILE_NAME  "Soul Blazer Randomized.smc"

int main (int argc, char** argv ) {
    const char *in_file = ROM_FILE_NAME;
    unsigned int seed;
    if(argc > 1) {
        in_file = argv[1];
    }
    if(argc > 2) {
        seed = atoi(argv[2]);
    } else {
        seed = Random::RandomInit(0);
    }
	return Randomizer::Randomize(in_file, MOD_ROM_FILE_NAME, seed) ? 0 : 1;
}
