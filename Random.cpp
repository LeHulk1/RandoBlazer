#include "Random.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

namespace Random {

    long RandomInit(long Seed) {
        if (Seed == 0) {
            /* No seed, then use timestamp as seed */
            Seed = time(NULL);
            srand(Seed);
        }
        else {
            /* Seed provided: use it */
            srand(Seed);
        }
        return Seed;
    }

    int RandomInteger(int Range) {
        /* Return an integer between 0 and Range-1 */
        return rand() % Range;
    }

    int RandomIntegerRange(int LowerBound, int UpperBound) {
        /* Return an integer between LowerBound and UpperBound */
        return (rand() % (UpperBound - LowerBound + 1)) + LowerBound;
    }
}
