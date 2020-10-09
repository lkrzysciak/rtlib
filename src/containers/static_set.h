#pragma once

#include <assert.h>
#include <stdbool.h>
#include "../memory/typed_pool.h"

#define declare_static_set_t(container_t, member_t, container_capacity) \
typedef struct container_t container_t; \
typedef struct container_t##_iterator container_t##_iterator; \
typedef struct container_t##_node container_t##_node; \
\
typedef struct container_t##_node \
{ \
    container_t##_node * right; \
    container_t##_node * left; \
    container_t##_node * parent; \
    member_t value; \
} container_t##_node; \
\
typedef struct container_t##_iterator \
{ \
    container_t##_node * node; \
    container_t##_node * next; \
} container_t##_iterator; \
\
typed_pool_t(container_t##_pool, container_t##_node, container_capacity); \
\
typedef int(*compare_t)(const member_t*, const member_t*); \
\
typedef struct container_t \
{ \
    container_t##_node* root; \
    container_t##_pool pool; \
    compare_t compare_function; \
    size_t size; \
} container_t; \
\
void container_t##_Init(container_t* const self, compare_t compare); \
size_t container_t##_Size(const container_t * const self); \
bool container_t##_Empty(const container_t * const self); \
int container_t##_Insert(container_t * const self, container_t##_iterator* const iterator, member_t data); \
int container_t##_Erase(container_t * const self, container_t##_iterator* const iterator); \
container_t##_iterator container_t##_Begin(const container_t * const self); \
container_t##_iterator container_t##_End(const container_t * const self); \
member_t container_t##_Iterator_GetValue(const container_t##_iterator* const self); \
void container_t##_Iterator_SetValue(container_t##_iterator* const self, member_t value); \
bool container_t##_Iterator_Equal(const container_t##_iterator* const first, const container_t##_iterator* const second); \
bool container_t##_Iterator_NotEqual(const container_t##_iterator* const first, const container_t##_iterator* const second); \
void container_t##_Iterator_Increment(container_t##_iterator* const self); \
void container_t##_Iterator_Decrement(container_t##_iterator* const self); \
container_t##_iterator container_t##_Find(container_t * const self, member_t data);


#define define_static_set_t(container_t, member_t, container_capacity) \
void container_t##_Init(container_t* const self, compare_t compare) \
{ \
    assert(self); \
    \
    self->size = 0; \
    self->root = NULL; \
    self->compare_function = compare; \
    container_t##_pool_Init(&self->pool); \
    assert(container_t##_pool_Capacity(&self->pool) == container_capacity); \
    void* end = container_t##_pool_Alloc(&self->pool); \
    assert(end); \
\
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
int container_t##_Insert(container_t * const self, member_t data) \
{ \
    assert(self); \
    \
    container_t##_node* node = container_t##_pool_Alloc(&self->pool); \
    if(node) \
    { \
        node->value = data; \
        \
        if(self->root) \
        { \
            container_t##_node* parent_node = NULL; \
            container_t##_node* child_node = self->root; \
            \
            while(1) \
            { \
                parent_node = child_node; \
                \
                int compare_value = self->compare_function(&child_node->value, &data); \
                if(compare_value < 0) \
                { \
                    child_node = child_node->left; \
                    if(!child_node) \
                    { \
                        parent_node->left = node; \
                        break; \
                    } \
                } \
                else if(compare_value > 0) \
                { \
                    child_node = child_node->right; \
                    if(!child_node) \
                    { \
                        parent_node->right = node; \
                        break; \
                    } \
                } \
                else \
                { \
                    /* Element exists */ \
                    return -2; \
                } \
            } \
            node->right = NULL; \
            node->left = NULL; \
            node->parent = parent_node; \
        } \
        else \
        { \
            node->right = NULL; \
            node->left = NULL; \
            node->parent = NULL; \
            self->root = node; \
        } \
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
    return 0; \
} \
\
container_t##_iterator container_t##_Begin(const container_t * const self) \
{ \
    assert(self); \
    \
    container_t##_iterator it = {0}; \
    \
    /* Go to the last left node */ \
    container_t##_node* parent_node = NULL; \
    container_t##_node* child_node = self->root; \
    while(child_node) \
    { \
        parent_node = child_node; \
        child_node = child_node->left; \
    } \
    it.node = parent_node; \
    it.next = parent_node->parent; \
    return it; \
} \
\
container_t##_iterator container_t##_End(const container_t * const self) \
{ \
    container_t##_iterator it = {0}; \
    return it; \
} \
\
member_t container_t##_Iterator_GetValue(const container_t##_iterator* const self) \
{ \
    member_t member = self->node->value; \
    return member; \
} \
\
void container_t##_Iterator_SetValue(container_t##_iterator* const self, member_t value) \
{ \
\
} \
\
bool container_t##_Iterator_Equal(const container_t##_iterator* const first, const container_t##_iterator* const second) \
{ \
    return first->node == second->node; \
} \
\
bool container_t##_Iterator_NotEqual(const container_t##_iterator* const first, const container_t##_iterator* const second) \
{ \
    return false; \
} \
\
void container_t##_Iterator_Increment(container_t##_iterator* const self) \
{ \
    assert(self); \
    \
    self->node = self->next; \
    if(!self->next) \
    { \
        return; \
    } \
    else \
    { \
        if(self->next->right != NULL) \
        { \
            self->next = self->next->right; \
            while (self->next->left != NULL) \
            { \
                self->next = self->next->left; \
            } \
            return; \
        } \
        \
        while(1) \
        { \
            if(self->next->parent == NULL) \
            { \
                self->next = NULL; \
                return; \
            } \
            if(self->next->parent->left == self->next) \
            { \
                self->next = self->next->parent; \
                return; \
            } \
            self->next = self->next->parent; \
        } \
    } \
} \
\
void container_t##_Iterator_Decrement(container_t##_iterator* const self) \
{ \
\
} \
\
container_t##_iterator container_t##_Find(container_t * const self, member_t data) \
{ \
    container_t##_iterator it = {0}; \
    return it; \
} \

