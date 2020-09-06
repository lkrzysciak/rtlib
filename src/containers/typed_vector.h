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
typedef member_type vector_type##_iterator; \
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
\
vector_type##_iterator* vector_type##_Begin(vector_type * const self) \
{ \
    assert(self); \
    \
    return self->data; \
} \
\
vector_type##_iterator* vector_type##_End(vector_type * const self) \
{ \
    assert(self); \
    \
    return self->end; \
} \
\
member_type vector_type##_Iterator_GetValue(const vector_type##_iterator* const self) \
{ \
    assert(self); \
    \
    return *self; \
} \
\
void vector_type##_Iterator_SetValue(vector_type##_iterator* const self, member_type value) \
{ \
    assert(self); \
    \
    *self = value; \
} \
\
vector_type##_iterator* vector_type##_Iterator_Increment(const vector_type##_iterator* const self) \
{ \
    assert(self); \
    \
    vector_type##_iterator* next_it = (vector_type##_iterator*)self + 1; \
    return next_it; \
} \
\
vector_type##_iterator* vector_type##_Iterator_Decrement(const vector_type##_iterator* const self) \
{ \
    assert(self); \
    \
    vector_type##_iterator* prev_it = (vector_type##_iterator*)self - 1; \
    return prev_it; \
} \

