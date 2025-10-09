#pragma once

#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define __comparator(type, prefix) prefix int type##_Compare(const type * v1, const type * v2);

#define __comparator_impl(type, prefix)                         \
    prefix int type##_Compare(const type * v1, const type * v2) \
    {                                                           \
        return memcmp(v1, v2, sizeof(type));                    \
    }

#define comparator(type) __comparator(type, )
#define comparator_impl(type) __comparator_impl(type, )

#define private_comparator(type) __comparator(type, static)
#define private_comparator_impl(type) __comparator_impl(type, static)

#ifdef __cplusplus
}
#endif