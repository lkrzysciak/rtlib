#pragma once

#include <stdlib.h>

#define memory_t(container_t)                                                                  \
                                                                                               \
    typedef struct container_t                                                                 \
    {                                                                                          \
        char stub; /* Empty structs are not allowed in C */                                    \
    } container_t;                                                                             \
                                                                                               \
    void container_t##_Construct(container_t * const self);                                    \
    void container_t##_Destroy(container_t * const self);                                      \
    void * container_t##_Allocate(container_t * const self, size_t size);                      \
    void * container_t##_Reallocate(container_t * const self, void * object, size_t new_size); \
    void container_t##_Deallocate(container_t * const self, void * object);

#define dynamic_memory_t(container_t)                                                         \
    void container_t##_Construct(container_t * const self)                                    \
    {                                                                                         \
        assert(self);                                                                         \
    }                                                                                         \
                                                                                              \
    void container_t##_Destroy(container_t * const self)                                      \
    {                                                                                         \
        assert(self);                                                                         \
    }                                                                                         \
                                                                                              \
    void * container_t##_Allocate(container_t * const self, size_t size)                      \
    {                                                                                         \
        assert(self);                                                                         \
                                                                                              \
        return malloc(size);                                                                  \
    }                                                                                         \
                                                                                              \
    void * container_t##_Reallocate(container_t * const self, void * object, size_t new_size) \
    {                                                                                         \
        assert(self);                                                                         \
                                                                                              \
        return realloc(object, new_size);                                                     \
    }                                                                                         \
                                                                                              \
    void container_t##_Deallocate(container_t * const self, void * object)                    \
    {                                                                                         \
        assert(self);                                                                         \
                                                                                              \
        free(object);                                                                         \
    }

#define dynamic_memory memory_t
#define dynamic_memory_impl dynamic_memory_t
