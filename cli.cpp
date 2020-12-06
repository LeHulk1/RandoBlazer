#include "Randomizer.h"
#include "Random.h"

#include <optional>
#include <stdlib.h>

#define ROM_FILE_NAME      "Soul Blazer (U) [!].smc"
#define MOD_ROM_FILE_NAME  "Soul Blazer Randomized.smc"

int main (int argc, char** argv ) {
    std::string in_file;
    std::string out_file;
    std::optional<unsigned int> seed;
    Randomizer::Options options;

    for (int i = 1; i < argc; i++) {
       if (in_file.empty()) {
           in_file = argv[i];
       } else if (out_file.empty()) {
           out_file = argv[i];
       } else if (!seed.has_value()) {
           seed = atoi(argv[i]);
       } else {
           options = Randomizer::Options(argv[i]);
       }
    }

    if (in_file.empty()) in_file = ROM_FILE_NAME;
    if (out_file.empty()) out_file = MOD_ROM_FILE_NAME;
    if (!seed.has_value()) seed = Random::RandomInit(0);

    return Randomizer::Randomize(in_file, out_file, seed.value(), options) ? 0 : 1;
}
