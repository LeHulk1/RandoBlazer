#ifndef __RANDOM_H__
#define __RANDOM_H__


namespace Random {
    int RandomInit(unsigned int Seed);
    // Return [0, Range)
    int RandomInteger(int Range);
    // Returns [LB, UB]
    int RandomIntegerRange(int LowerBound, int UpperBound);
}

#endif // __RANDOM_H__
