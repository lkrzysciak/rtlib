#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>

    typedef struct Pool Pool;

    Pool * const Pool_Init(void * memory_block, size_t memory_size, size_t object_size);
    void * Pool_Alloc(Pool * const self);
    void Pool_Free(Pool * const self, void * object);
    size_t Pool_Capacity(Pool * const self);
    size_t Pool_Size(Pool * const self);

#ifdef __cplusplus
}
#endif