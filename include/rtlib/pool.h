#pragma once

#include <assert.h>

#define __pool_methods_h(type_t, member_t)             \
    void type_t##_Construct(type_t * const self);      \
    void type_t##_Destroy(type_t * const self);        \
    member_t * type_t##_Allocate(type_t * const self); \
    void type_t##_Release(type_t * const self, member_t * object);

#define __static_pool_methods_c(type_t, member_t, pool_capacity)            \
    void type_t##_Construct(type_t * const self)                            \
    {                                                                       \
        assert(self);                                                       \
                                                                            \
        self->first_free_block = self->nodes;                               \
                                                                            \
        for(int block_no = 0; block_no < pool_capacity - 1; ++block_no)     \
        {                                                                   \
            self->nodes[block_no].next = &self->nodes[block_no + 1];        \
        }                                                                   \
        self->last_free_block       = &self->nodes[pool_capacity - 1];      \
        self->last_free_block->next = NULL;                                 \
    }                                                                       \
                                                                            \
    void type_t##_Destroy(type_t * const self)                              \
    {                                                                       \
        assert(self);                                                       \
    }                                                                       \
                                                                            \
    member_t * type_t##_Allocate(type_t * const self)                       \
    {                                                                       \
        assert(self);                                                       \
                                                                            \
        if(self->first_free_block)                                          \
        {                                                                   \
            type_t##_node * to_return_block = self->first_free_block;       \
            self->first_free_block          = self->first_free_block->next; \
            return &to_return_block->value;                                 \
        }                                                                   \
        else                                                                \
        {                                                                   \
            return NULL;                                                    \
        }                                                                   \
    }                                                                       \
                                                                            \
    void type_t##_Release(type_t * const self, member_t * object)           \
    {                                                                       \
        assert(self);                                                       \
        assert(object);                                                     \
                                                                            \
        type_t##_node * node        = (type_t##_node *)object;              \
        node->next                  = NULL;                                 \
        self->last_free_block->next = node;                                 \
        self->last_free_block       = node;                                 \
                                                                            \
        if(!self->first_free_block)                                         \
        {                                                                   \
            self->first_free_block = node;                                  \
        }                                                                   \
    }

#define __custom_allocator_pool_methods_c(pool_t, member_t, allocator_t)               \
    typedef struct pool_t                                                              \
    {                                                                                  \
        allocator_t allocator;                                                         \
    } pool_t;                                                                          \
                                                                                       \
    void pool_t##_Construct(pool_t * const self)                                       \
    {                                                                                  \
        assert(self);                                                                  \
                                                                                       \
        allocator_t##_Construct(&self->allocator);                                     \
    }                                                                                  \
                                                                                       \
    void pool_t##_Destroy(pool_t * const self)                                         \
    {                                                                                  \
        assert(self);                                                                  \
                                                                                       \
        allocator_t##_Destroy(&self->allocator);                                       \
    }                                                                                  \
                                                                                       \
    member_t * pool_t##_Allocate(pool_t * const self)                                  \
    {                                                                                  \
        assert(self);                                                                  \
                                                                                       \
        return (member_t *)allocator_t##_Allocate(&self->allocator, sizeof(member_t)); \
    }                                                                                  \
                                                                                       \
    void pool_t##_Release(pool_t * const self, member_t * object)                      \
    {                                                                                  \
        assert(self);                                                                  \
                                                                                       \
        allocator_t##_Deallocate(&self->allocator, object);                            \
    }

#define pool_t(type_t, member_t)  \
    typedef struct type_t type_t; \
    __pool_methods_h(type_t, member_t)

#define static_pool_t(type_t, member_t, pool_capacity) \
    typedef struct type_t##_node type_t##_node;        \
                                                       \
    struct type_t##_node                               \
    {                                                  \
        member_t value;                                \
        type_t##_node * next;                          \
    };                                                 \
                                                       \
    typedef struct type_t                              \
    {                                                  \
        type_t##_node nodes[pool_capacity];            \
        type_t##_node * first_free_block;              \
        type_t##_node * last_free_block;               \
    } type_t;                                          \
    __static_pool_methods_c(type_t, member_t, pool_capacity)

#define custom_allocator_pool_t(pool_t, member_t, allocator_t) \
    __custom_allocator_pool_methods_c(pool_t, member_t, allocator_t)

#define static_pool(type_t, member_t, pool_capacity) \
    typedef struct type_t type_t;                    \
    typedef struct type_t##_node type_t##_node;      \
                                                     \
    struct type_t##_node                             \
    {                                                \
        member_t value;                              \
        type_t##_node * next;                        \
    };                                               \
                                                     \
    typedef struct type_t                            \
    {                                                \
        type_t##_node nodes[pool_capacity];          \
        type_t##_node * first_free_block;            \
        type_t##_node * last_free_block;             \
    } type_t;                                        \
    __pool_methods_h(type_t, member_t)

#define custom_allocator_pool(type_t, member_t, allocator_t) \
    typedef struct type_t type_t;                            \
    typedef struct type_t                                    \
    {                                                        \
        allocator_t allocator;                               \
    } type_t;                                                \
    __pool_methods_h(type_t, member_t)

#define static_pool_impl(type_t, member_t, pool_capacity) __static_pool_methods_c(type_t, member_t, pool_capacity)

#define custom_allocator_pool_impl(type_t, member_t, allocator_t) \
    __custom_allocator_pool_methods_c(pool_t, member_t, allocator_t)
