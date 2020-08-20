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
        return 0; \
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
    if(new_end_node == self->begin) \
    { \
        self->begin = self->end; \
        self->begin->next = NULL; \
        self->begin->prev = NULL; \
    } \
    else \
    { \
        new_end_node->next = self->end; \
    } \
} \