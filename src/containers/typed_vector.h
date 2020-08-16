#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define typed_vector_t(vector_type, member_type) \
typedef struct vector_type##_Vector \
{ \
    size_t element_size; \
    size_t capacity; \
    size_t size; \
    member_type* end; \
} vector_type##_Vector; \
 \
vector_type##_Vector* vector_type##_Vector_Init(void * buffer, size_t size) \
{ \
    assert(buffer); \
    assert(size >= sizeof(vector_type##_Vector)); \
    \
    vector_type##_Vector* vector = (vector_type##_Vector*)buffer; \
    vector->element_size = sizeof(member_type); \
    vector->size = 0; \
    vector->capacity = (size - sizeof(vector_type##_Vector)) / vector->element_size; \
    vector->end = (member_type*)((uint8_t*)vector + sizeof(vector_type##_Vector)); \
    \
    return vector; \
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
    if(self->size < self->capacity) \
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

#ifdef __cplusplus
}
#endif