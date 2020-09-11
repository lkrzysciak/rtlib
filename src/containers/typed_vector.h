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
int vector_type##_PushFront(vector_type * const self, member_type data) \
{ \
    assert(self); \
    \
    if(self->size < container_capacity) \
    { \
        memmove(&self->data[1], &self->data[0], self->size * sizeof(member_type)); \
        self->data[0] = data; \
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
int vector_type##_PopFront(vector_type * const self) \
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
        memmove(&self->data[0], &self->data[1], self->size * sizeof(member_type)); \
        return self->size; \
    } \
} \
\
int vector_type##_Insert(vector_type * const self, member_type data, size_t index) \
{ \
    assert(self); \
    \
    if(self->size < container_capacity) \
    { \
        if(index <= self->size) \
        { \
            const size_t to_move_elements = self->size - index; \
            memmove(&self->data[index + 1], &self->data[index], to_move_elements * sizeof(member_type)); \
            self->data[index] = data; \
            ++self->end; \
            ++self->size; \
            \
            return self->size; \
        } \
        else \
        { \
            /* Out of range */ \
            return -2; \
        } \
    } \
    else \
    { \
        /* Allocation error */ \
        return -1; \
    } \
} \
\
int vector_type##_Erase(vector_type * const self, size_t index) \
{ \
    assert(self); \
    if(self->size == 0 || index >= self->size) \
    { \
        /* Out of range */ \
        return -2; \
    } \
    \
    --self->size; \
    --self->end; \
    const size_t to_move_elements = self->size - index; \
    memmove(&self->data[index], &self->data[index + 1], to_move_elements * sizeof(member_type)); \
    return self->size; \
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

