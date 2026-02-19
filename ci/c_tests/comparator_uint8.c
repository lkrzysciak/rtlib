#include <stdint.h>

#include "rtlib/comparator.h"

comparator(uint8_t);

int main(void)
{
    const uint8_t lower  = 5;
    const uint8_t higher = 200;
    const uint8_t equal  = 5;

    if(uint8_t_Compare(&lower, &higher) >= 0)
    {
        return 1;
    }

    if(uint8_t_Compare(&higher, &lower) <= 0)
    {
        return 2;
    }

    if(uint8_t_Compare(&lower, &equal) != 0)
    {
        return 3;
    }

    return 0;
}
