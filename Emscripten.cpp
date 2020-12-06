#include "Randomizer.h"

#include <emscripten/bind.h>

namespace RandoblazerExport {
    int CheckFile(const std::string& in_file) { return Randomizer::CheckFile(in_file); }
    std::string Randomize(const std::string& in_file, const std::string& out_file, unsigned int seed, const std::string& options_string) {
        std::string seedName;
        bool result = Randomizer::Randomize(in_file, out_file, seed, Randomizer::Options(options_string), &seedName);

        return result ? seedName : "";
    }
}

using namespace emscripten;

EMSCRIPTEN_BINDINGS(randoblazer) {
    function("CheckFile", &RandoblazerExport::CheckFile);
    function("Randomize", &RandoblazerExport::Randomize);
}
