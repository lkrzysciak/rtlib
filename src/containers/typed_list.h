#pragma once

#include <assert.h>
#include "../memory/typed_pool.h"

#define typed_list_t(container_t, member_t, container_capacity) \
\
typedef struct container_t container_t; \
typedef struct container_t##_iterator container_t##_iterator; \
typedef struct container_t##_node container_t##_node; \
\
typedef struct container_t##_node \
{ \
    container_t##_node * prev; \
    container_t##_node * next; \
    member_t value; \
} container_t##_node; \
\
typedef struct container_t##_iterator \
{ \
    container_t##_node * node; \
} container_t##_iterator; \
\
typed_pool_t(container_t##_pool, container_t##_node, container_capacity + 1); \
\
typedef struct container_t \
{ \
    container_t##_node* begin; \
    container_t##_node* end; \
    container_t##_pool pool; \
    size_t size; \
} container_t; \
\
void container_t##_Init(container_t* const self) \
{ \
    assert(self); \
    \
    container_t##_pool_Init(&self->pool); \
    self->end = container_t##_pool_Alloc(&self->pool); \
    assert(self->end); \
    self->begin = self->end; \
    self->begin->prev = NULL; \
    self->begin->next = NULL; \
    self->size = 0; \
} \
\
int container_t##_PushBack(container_t* const self, member_t value) \
{ \
    assert(self); \
    \
    container_t##_node* node = container_t##_pool_Alloc(&self->pool); \
    if(node) \
    { \
        container_t##_node* old_end_node = self->end->prev; \
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
int container_t##_PopBack(container_t* const self) \
{ \
    assert(self); \
    \
    container_t##_node* old_end_node = self->end->prev; \
    container_t##_node* new_end_node = old_end_node->prev; \
    \
    container_t##_pool_Free(&self->pool, old_end_node); \
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
int container_t##_PushFront(container_t* const self, member_t value) \
{ \
    assert(self); \
    \
    container_t##_node* node = container_t##_pool_Alloc(&self->pool); \
    if(node) \
    { \
        container_t##_node* old_begin_node = self->begin; \
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
int container_t##_PopFront(container_t* const self) \
{ \
    assert(self); \
    \
    container_t##_node* old_begin_node = self->begin; \
    container_t##_node* new_begin_node = old_begin_node->next; \
    \
    container_t##_pool_Free(&self->pool, old_begin_node); \
    \
    new_begin_node->prev = NULL; \
    self->begin = new_begin_node; \
    \
    --self->size; \
    \
    return self->size; \
} \
\
int container_t##_Insert(container_t* const self, member_t value, container_t##_iterator* const iterator) \
{ \
    assert(self); \
    assert(iterator); \
   \
   if(iterator->node == self->begin) \
   { \
        return container_t##_PushFront(self, value); \
   } \
   \
   else if(iterator->node == self->end) \
   { \
       return container_t##_PushBack(self, value); \
   } \
    container_t##_node* node = container_t##_pool_Alloc(&self->pool); \
    if(node) \
    { \
        container_t##_node* next_iterator = iterator->node; \
        assert(next_iterator); \
        container_t##_node* prev_iterator = next_iterator->prev; \
        assert(prev_iterator); \
        next_iterator->prev = node; \
        prev_iterator->next = node; \
        node->next = next_iterator; \
        node->prev = prev_iterator; \
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
int container_t##_Erase(container_t* const self, container_t##_iterator* const iterator) \
{ \
    assert(self); \
    \
    container_t##_node* to_delete_node = iterator->node; \
    container_t##_node* next_node = to_delete_node->next; \
    container_t##_node* prev_node = to_delete_node->prev; \
    \
    container_t##_pool_Free(&self->pool, to_delete_node); \
    \
    if(!prev_node) \
    { \
        return container_t##_PopFront(self); \
    } \
    prev_node->next = next_node; \
    next_node->prev = prev_node; \
    \
    --self->size; \
    \
    return self->size; \
} \
\
member_t container_t##_Front(container_t* const self) \
{ \
    assert(self); \
    assert(self->size > 0); \
    \
    return self->begin->value; \
} \
\
member_t container_t##_Back(container_t* const self) \
{ \
    assert(self); \
    assert(self->size > 0); \
    \
    return self->end->prev->value; \
} \
\
container_t##_iterator container_t##_Begin(container_t* const self) \
{ \
    assert(self); \
    \
    container_t##_iterator it; \
    it.node = self->begin; \
    return it; \
} \
\
container_t##_iterator container_t##_End(container_t* const self) \
{ \
    assert(self); \
    \
    container_t##_iterator it; \
    it.node = self->end; \
    return it; \
} \
\
member_t container_t##_Iterator_GetValue(const container_t##_iterator* const self) \
{ \
    assert(self); \
    \
    return self->node->value; \
} \
\
void container_t##_Iterator_SetValue(container_t##_iterator* const self, member_t value) \
{ \
    assert(self); \
    \
    self->node->value = value; \
} \
\
container_t##_iterator container_t##_Iterator_Next(const container_t##_iterator* const self) \
{ \
    assert(self); \
    \
    container_t##_iterator it; \
    it.node = self->node->next; \
    return it; \
} \
\
container_t##_iterator container_t##_Iterator_Prev(const container_t##_iterator* const self) \
{ \
    assert(self); \
    \
    container_t##_iterator it; \
    it.node = self->node->prev; \
    return it; \
} \
\
bool container_t##_Iterator_Equal(const container_t##_iterator* const first, const container_t##_iterator* const second) \
{ \
    const bool is_equal = first->node == second->node ; \
    return is_equal; \
} \

