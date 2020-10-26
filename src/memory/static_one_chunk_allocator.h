#pragma once

#include <stdlib.h>
#include <stdint.h>

#define declare_static_one_chunk_allocator_t(container_t, capacity) \
\
typedef struct container_t \
{ \
    size_t size; \
    uint8_t data[capacity]; \
} container_t; \
\
void container_t##_Construct(container_t* const self); \
void container_t##_Destroy(container_t* const self); \
void* container_t##_Allocate(container_t* const self, size_t size); \
void* container_t##_Reallocate(container_t* const self, void* object, size_t new_size); \
void container_t##_Deallocate(container_t* const self, void* object);


#define define_static_one_chunk_allocator_t(container_t, capacity) \
void container_t##_Construct(container_t* const self) \
{ \
    assert(self); \
    \
    self->size = 0; \
} \
\
void container_t##_Destroy(container_t* const self) \
{ \
    assert(self); \
} \
\
void* container_t##_Allocate(container_t* const self, size_t size) \
{ \
    assert(self); \
    \
    if(size > capacity) \
    { \
        return NULL; \
    } \
    if(self->size != 0) \
    { \
        return NULL; \
    } \
    self->size = size; \
    return self->data; \
} \
\
void* container_t##_Reallocate(container_t* const self, void* object, size_t new_size) \
{ \
    assert(self); \
    \
    if(new_size > capacity) \
    { \
        return NULL; \
    } \
    if(object != self->data) \
    { \
        return NULL; \
    } \
    self->size = new_size; \
    return self->data; \
} \
\
void container_t##_Deallocate(container_t* const self, void* object) \
{ \
    assert(self); \
    \
    self->size = 0; \
} \

