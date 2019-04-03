#include "Randomizer.h"

#include <emscripten.h>

extern "C" {

int EMSCRIPTEN_KEEPALIVE check_file(const char* in_file) {
    return Randomizer::CheckFile(in_file);
}


int EMSCRIPTEN_KEEPALIVE randomize(const char* in_file, const char* out_file, unsigned int seed, const char* options_string) {
    return Randomizer::Randomize(in_file, out_file, seed, Randomizer::Options(options_string));
}

}
