#include "Random.h"

#include <random>
#include <boost/random/uniform_int_distribution.hpp>

using namespace std;

static std::mt19937 gen;

namespace Random {

    int RandomInit(unsigned int seed) {
        if (seed == 0) {
            std::random_device rd;
            seed = rd();
            gen.seed(seed); 
        }
        else {
            gen.seed(seed);
        }
        return seed;
    }

    int RandomInteger(int Range) {
        return RandomIntegerRange(0, Range - 1);
    }

    int RandomIntegerRange(int LowerBound, int UpperBound) {
        static boost::random::uniform_int_distribution<int> dis;

        return dis(gen, boost::random::uniform_int_distribution<int>::param_type{LowerBound, UpperBound});
    }
}
