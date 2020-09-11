#pragma once

#include <assert.h>
#include "../memory/typed_pool.h"

#define typed_list_t(type_name, member_type, list_capacity) \
\
typedef struct type_name type_name; \
typedef struct type_name##_iterator type_name##_iterator; \
\
typedef struct type_name##_iterator \
{ \
    type_name##_iterator * prev; \
    type_name##_iterator * next; \
    member_type value; \
} type_name##_iterator; \
\
typedef type_name##_iterator type_name##_node; \
\
typed_pool_t(type_name##_pool, type_name##_node, list_capacity + 1); \
\
typedef struct type_name \
{ \
    type_name##_node* begin; \
    type_name##_node* end; \
    type_name##_pool pool; \
    size_t size; \
} type_name; \
\
void type_name##_Init(type_name* const self) \
{ \
    assert(self); \
    \
    type_name##_pool_Init(&self->pool); \
    self->end = type_name##_pool_Alloc(&self->pool); \
    assert(self->end); \
    self->begin = self->end; \
    self->begin->prev = NULL; \
    self->begin->next = NULL; \
    self->size = 0; \
} \
\
int type_name##_PushBack(type_name* const self, member_type value) \
{ \
    assert(self); \
    \
    type_name##_node* node = type_name##_pool_Alloc(&self->pool); \
    if(node) \
    { \
        type_name##_node* old_end_node = self->end->prev; \
        if(old_end_node) \
        { \
            old_end_node->next = node; \
        } \
        else \
        { \
            self->begin = node; \
        } \
        node->prev = old_end_node; \
        node->next = self->end; \
        self->end->prev = node; \
        node->value = value; \
        \
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
int type_name##_PopBack(type_name* const self) \
{ \
    assert(self); \
    \
    type_name##_node* old_end_node = self->end->prev; \
    type_name##_node* new_end_node = old_end_node->prev; \
    \
    type_name##_pool_Free(&self->pool, old_end_node); \
    \
    if(new_end_node == NULL) \
    { \
        self->begin = self->end; \
        self->begin->next = NULL; \
        self->begin->prev = NULL; \
    } \
    else \
    { \
        new_end_node->next = self->end; \
        self->end->prev = new_end_node; \
    } \
    \
    --self->size; \
    \
    return self->size; \
} \
\
int type_name##_PushFront(type_name* const self, member_type value) \
{ \
    assert(self); \
    \
    type_name##_node* node = type_name##_pool_Alloc(&self->pool); \
    if(node) \
    { \
        type_name##_node* old_begin_node = self->begin; \
        old_begin_node->prev = node; \
        node->next = old_begin_node; \
        node->prev = NULL; \
        node->value = value; \
        self->begin = node; \
        \
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
int type_name##_PopFront(type_name* const self) \
{ \
    assert(self); \
    \
    type_name##_node* old_begin_node = self->begin; \
    type_name##_node* new_begin_node = old_begin_node->next; \
    \
    type_name##_pool_Free(&self->pool, old_begin_node); \
    \
    new_begin_node->prev = NULL; \
    self->begin = new_begin_node; \
    \
    --self->size; \
    \
    return self->size; \
} \
\
int type_name##_Insert(type_name* const self, member_type value, size_t index) \
{ \
    assert(self); \
    \
    type_name##_node* node = type_name##_pool_Alloc(&self->pool); \
    if(node) \
    { \
        type_name##_node* old_begin_node = self->begin; \
        old_begin_node->prev = node; \
        node->next = old_begin_node; \
        node->prev = NULL; \
        node->value = value; \
        self->begin = node; \
        \
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
int type_name##_Erase(type_name* const self, size_t index) \
{ \
    assert(self); \
    \
    type_name##_node* old_begin_node = self->begin; \
    type_name##_node* new_begin_node = old_begin_node->next; \
    \
    type_name##_pool_Free(&self->pool, old_begin_node); \
    \
    new_begin_node->prev = NULL; \
    self->begin = new_begin_node; \
    \
    --self->size; \
    \
    return self->size; \
} \
\
member_type type_name##_Front(type_name* const self) \
{ \
    assert(self); \
    assert(self->size > 0); \
    \
    return self->begin->value; \
} \
\
member_type type_name##_Back(type_name* const self) \
{ \
    assert(self); \
    assert(self->size > 0); \
    \
    return self->end->prev->value; \
} \
\
type_name##_iterator*  type_name##_Begin(type_name* const self) \
{ \
    assert(self); \
    \
    return self->begin; \
} \
\
type_name##_iterator* type_name##_End(type_name* const self) \
{ \
    assert(self); \
    \
    return self->end; \
} \
\
member_type type_name##_Iterator_GetValue(const type_name##_iterator* const self) \
{ \
    assert(self); \
    \
    return self->value; \
} \
\
void type_name##_Iterator_SetValue(type_name##_iterator* const self, member_type value) \
{ \
    assert(self); \
    \
    self->value = value; \
} \
\
type_name##_iterator* type_name##_Iterator_Increment(const type_name##_iterator* const self) \
{ \
    assert(self); \
    \
    return self->next; \
} \
\
type_name##_iterator* type_name##_Iterator_Decrement(const type_name##_iterator* const self) \
{ \
    assert(self); \
    \
    return self->prev; \
} \

