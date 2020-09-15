#pragma once

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "error_codes.h"

#define declare_static_vector_t(container_t, member_t, container_capacity) \
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
void container_t##_Init(container_t* const self); \
size_t container_t##_Size(const container_t * const self); \
bool container_t##_Empty(const container_t * const self); \
int container_t##_PushBack(container_t * const self, member_t data); \
int container_t##_PopBack(container_t * const self); \
int container_t##_PushFront(container_t * const self, member_t data); \
int container_t##_PopFront(container_t * const self); \
int container_t##_Insert(container_t * const self, container_t##_iterator* const iterator, member_t data); \
int container_t##_Erase(container_t * const self, container_t##_iterator* const iterator); \
member_t container_t##_Front(container_t * const self); \
member_t container_t##_Back(container_t * const self); \
container_t##_iterator container_t##_Begin(container_t * const self); \
container_t##_iterator container_t##_End(container_t * const self); \
member_t container_t##_Iterator_GetValue(const container_t##_iterator* const self); \
void container_t##_Iterator_SetValue(container_t##_iterator* const self, member_t value); \
bool container_t##_Iterator_Equal(const container_t##_iterator* const first, const container_t##_iterator* const second); \
bool container_t##_Iterator_NotEqual(const container_t##_iterator* const first, const container_t##_iterator* const second); \
void container_t##_Iterator_Increment(container_t##_iterator* const self); \
void container_t##_Iterator_Decrement(container_t##_iterator* const self); \
container_t##_iterator container_t##_Find(container_t * const self, member_t data, bool(*fun)(const member_t*, const member_t*));

#define define_static_vector_t(container_t, member_t, container_capacity) \
void container_t##_Init(container_t* const self) \
{ \
    assert(self); \
    assert(sizeof(self->data) / sizeof(member_t) == container_capacity); \
    \
    self->size = 0; \
    self->end = self->data; \
} \
\
size_t container_t##_Size(const container_t * const self) \
{ \
    assert(self); \
    \
    return self->size; \
} \
\
bool container_t##_Empty(const container_t * const self) \
{ \
    assert(self); \
    \
    return self->size == 0; \
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
        return ALLOCATION_ERROR; \
    } \
} \
\
int container_t##_PopBack(container_t * const self) \
{ \
    assert(self); \
    \
    --self->size; \
    --self->end; \
    return self->size; \
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
        return ALLOCATION_ERROR; \
    } \
} \
\
int container_t##_PopFront(container_t * const self) \
{ \
    assert(self); \
    \
    --self->size; \
    --self->end; \
    memmove(&self->data[0], &self->data[1], self->size * sizeof(member_t)); \
    return self->size; \
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
        return ALLOCATION_ERROR; \
    } \
} \
\
int container_t##_Erase(container_t * const self, container_t##_iterator* const iterator) \
{ \
    assert(self); \
    assert(iterator); \
    \
    --self->size; \
    --self->end; \
    const size_t to_move_bytes = (uint8_t*)self->end - (uint8_t*)iterator->value; \
    memmove(iterator->value, iterator->value + 1, to_move_bytes); \
    return self->size; \
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
bool container_t##_Iterator_NotEqual(const container_t##_iterator* const first, const container_t##_iterator* const second) \
{ \
    const bool is_equal = first->value == second->value ; \
    return !is_equal; \
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
\
container_t##_iterator container_t##_Find(container_t * const self, member_t data, bool(*fun)(const member_t*, const member_t*)) \
{ \
    assert(self); \
    \
    container_t##_iterator end = container_t##_End(self); \
    container_t##_iterator it=container_t##_Begin(self); \
    \
    for(; container_t##_Iterator_NotEqual(&it, &end); container_t##_Iterator_Increment(&it)) \
    { \
        const member_t it_value = container_t##_Iterator_GetValue(&it); \
        if(fun(&data, &it_value)) \
        { \
            break; \
        } \
    } \
    return it; \
} \

