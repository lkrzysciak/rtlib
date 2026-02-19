#include <stdint.h>

#include "rtlib/hash.h"

hash(uint8_t);

int main(void)
{
    const uint8_t a = 5;
    const uint8_t b = 200;
    const uint8_t c = 5;

    const uint32_t ha = uint8_t_Hash(&a);
    const uint32_t hb = uint8_t_Hash(&b);
    const uint32_t hc = uint8_t_Hash(&c);

    if(ha != hc)
    {
        return 1;
    }

    if(ha == hb)
    {
        return 2;
    }

    return 0;
}
