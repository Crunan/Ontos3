#include "Utilities.h"


bool isBitSet(int test_int, int bit_pos)
{
    int bitmask{};

    bitmask = 1<<bit_pos;
    if (test_int & bitmask) {
        return true;
    }
    else {
        return false;
    }
}
