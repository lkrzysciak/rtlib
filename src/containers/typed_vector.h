#pragma once

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define typed_vector_t(vector_type, member_type, container_capacity) \
typedef struct vector_type \
{ \
    size_t size; \
    member_type data[container_capacity]; \
    member_type* end; \
} vector_type; \
 \
void vector_type##_Init(vector_type* const self) \
{ \
    assert(self); \
    \
    self->size = 0; \
    self->end = self->data; \
} \
\
size_t vector_type##_Size(vector_type * const self) \
{ \
    assert(self); \
    \
    return self->size; \
} \
\
int vector_type##_PushBack(vector_type * const self, member_type data) \
{ \
    assert(self); \
    assert(data); \
    \
    if(self->size < container_capacity) \
    { \
        *self->end = data; \
        ++self->end; \
        ++self->size; \
        \
        return self->size; \
    } \
    else \
    { \
        return -1; \
    } \
} \
\
int vector_type##_PopBack(vector_type * const self) \
{ \
    assert(self); \
    if(self->size == 0) \
    { \
        return -1; \
    } \
    else \
    { \
        --self->size; \
        --self->end; \
        return self->size; \
    } \
} \
\
member_type vector_type##_Front(vector_type * const self) \
{ \
    assert(self); \
    assert(self->size > 0); \
    \
    return self->data[0]; \
} \
\
member_type vector_type##_Back(vector_type * const self) \
{ \
    assert(self); \
    assert(self->size > 0); \
    \
    return *(self->end - 1); \
} \
