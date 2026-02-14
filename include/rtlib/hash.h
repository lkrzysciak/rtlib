#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define hash(type)                                      \
    static inline uint32_t type##_Hash(const type * v1) \
    {                                                   \
        const uint8_t * s = (const uint8_t *)v1;        \
        size_t len        = sizeof(type);               \
        uint32_t h        = 5381;                       \
        while(len--)                                    \
        {                                               \
            h += (h << 5);                              \
            h ^= *s++;                                  \
        }                                               \
        return h;                                       \
    }

#ifdef __cplusplus
}
#endif