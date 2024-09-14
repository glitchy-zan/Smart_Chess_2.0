#include <util/util.h>

/* helper function turning snesor outputs to array */
void unsignedCharToIntArray(unsigned char value, int *array)
{
    for (int i = 0; i < 8; i++)
    {
        array[i] = (value & (1 << (7 - i))) ? 1 : 0;
    }
}