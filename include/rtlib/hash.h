#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define __hash(type, prefix) prefix uint32_t type##_Hash(const type * v1);

#define __hash_impl(type, prefix)                \
    prefix uint32_t type##_Hash(const type * v1) \
    {                                            \
        const uint8_t * s = (const uint8_t *)v1; \
        size_t len        = sizeof(type);        \
        uint32_t h        = 5381;                \
        while(len--)                             \
        {                                        \
            h += (h << 5);                       \
            h ^= *s++;                           \
        }                                        \
        return h;                                \
    }

#define hash(type) __hash(type, )
#define hash_impl(type) __hash_impl(type, )

#define private_hash(type) __hash(type, static)
#define private_hash_impl(type) __hash_impl(type, static)

#ifdef __cplusplus
}
#endif