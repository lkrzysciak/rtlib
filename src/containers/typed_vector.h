#pragma once

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define typed_vector_t(container_t, member_t, container_capacity) \
typedef struct container_t \
{ \
    size_t size; \
    member_t data[container_capacity]; \
    member_t* end; \
} container_t; \
\
typedef struct container_t##_iterator \
{ \
    member_t* value; \
} container_t##_iterator; \
\
void container_t##_Init(container_t* const self) \
{ \
    assert(self); \
    \
    self->size = 0; \
    self->end = self->data; \
} \
\
size_t container_t##_Size(container_t * const self) \
{ \
    assert(self); \
    \
    return self->size; \
} \
\
int container_t##_PushBack(container_t * const self, member_t data) \
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
int container_t##_PopBack(container_t * const self) \
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
int container_t##_PushFront(container_t * const self, member_t data) \
{ \
    assert(self); \
    \
    if(self->size < container_capacity) \
    { \
        memmove(&self->data[1], &self->data[0], self->size * sizeof(member_t)); \
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
int container_t##_PopFront(container_t * const self) \
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
        memmove(&self->data[0], &self->data[1], self->size * sizeof(member_t)); \
        return self->size; \
    } \
} \
\
int container_t##_Insert(container_t * const self, container_t##_iterator* const iterator, member_t data) \
{ \
    assert(self); \
    assert(iterator); \
    \
    if(self->size < container_capacity) \
    { \
        const size_t to_move_bytes = (uint8_t*)self->end - (uint8_t*)iterator->value; \
        memmove(iterator->value + 1, iterator->value, to_move_bytes); \
        *iterator->value = data; \
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
int container_t##_Erase(container_t * const self, container_t##_iterator* const iterator) \
{ \
    assert(self); \
    assert(iterator); \
    \
    if(self->size == 0) \
    { \
        return -1; \
    } \
    else \
    { \
        --self->size; \
        --self->end; \
        const size_t to_move_bytes = (uint8_t*)self->end - (uint8_t*)iterator->value; \
        memmove(iterator->value, iterator->value + 1, to_move_bytes); \
        return self->size; \
    } \
} \
\
member_t container_t##_Front(container_t * const self) \
{ \
    assert(self); \
    assert(self->size > 0); \
    \
    return self->data[0]; \
} \
\
member_t container_t##_Back(container_t * const self) \
{ \
    assert(self); \
    assert(self->size > 0); \
    \
    return *(self->end - 1); \
} \
\
container_t##_iterator container_t##_Begin(container_t * const self) \
{ \
    assert(self); \
    \
    container_t##_iterator it; \
    it.value = self->data; \
    \
    return it; \
} \
\
container_t##_iterator container_t##_End(container_t * const self) \
{ \
    assert(self); \
    \
    container_t##_iterator it; \
    it.value = self->end; \
    \
    return it; \
} \
\
member_t container_t##_Iterator_GetValue(const container_t##_iterator* const self) \
{ \
    assert(self); \
    \
    return *self->value; \
} \
\
void container_t##_Iterator_SetValue(container_t##_iterator* const self, member_t value) \
{ \
    assert(self); \
    \
    *self->value = value; \
} \
\
bool container_t##_Iterator_Equal(const container_t##_iterator* const first, const container_t##_iterator* const second) \
{ \
    const bool is_equal = first->value == second->value ; \
    return is_equal; \
} \
\
void container_t##_Iterator_Increment(container_t##_iterator* const self) \
{ \
    assert(self); \
    \
    self->value = self->value + 1; \
} \
\
void container_t##_Iterator_Decrement(container_t##_iterator* const self) \
{ \
    assert(self); \
    \
    self->value = self->value - 1; \
} \
