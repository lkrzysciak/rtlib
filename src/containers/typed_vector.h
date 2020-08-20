#pragma once

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define typed_vector_t(vector_type, member_type, container_capacity) \
typedef struct vector_type##_Vector \
{ \
    size_t size; \
    member_type data[container_capacity]; \
    member_type* end; \
} vector_type##_Vector; \
 \
void vector_type##_Vector_Init(vector_type##_Vector* const self) \
{ \
    assert(self); \
    \
    self->size = 0; \
    self->end = self->data; \
} \
\
size_t vector_type##_Vector_Size(vector_type##_Vector * const self) \
{ \
    assert(self); \
    \
    return self->size; \
} \
\
int vector_type##_Vector_PushBack(vector_type##_Vector * const self, member_type data) \
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
        return 0; \
    } \
    else \
    { \
        return -1; \
    } \
} \
\
int vector_type##_Vector_PopBack(vector_type##_Vector * const self) \
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
        return 0; \
    } \
}
