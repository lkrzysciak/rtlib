#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#define pair(container_t, first_t, second_t) \
    typedef struct container_t container_t;  \
    struct container_t                       \
    {                                        \
        first_t first;                       \
        second_t second;                     \
    };

#ifdef __cplusplus
}
#endif
