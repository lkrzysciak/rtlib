#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "binary_tree.h"
#include "rtlib/memory.h"

#define static_set(container_t, key_t, container_capacity)                   \
    typedef struct container_t container_t;                                  \
    typedef struct container_t##_Iterator container_t##_Iterator;            \
    typedef struct container_t##_node container_t##_node;                    \
    typedef int (*container_t##_compare_t)(const key_t *, const key_t *);    \
                                                                             \
    struct container_t##_node                                                \
    {                                                                        \
        container_t##_node * right;                                          \
        container_t##_node * left;                                           \
        container_t##_node * parent;                                         \
        key_t key;                                                           \
    };                                                                       \
                                                                             \
    struct container_t##_Iterator                                            \
    {                                                                        \
        container_t##_node * node;                                           \
        container_t##_node * next;                                           \
        container_t##_node * prev;                                           \
    };                                                                       \
    static_pool(container_t##_pool, container_t##_node, container_capacity); \
                                                                             \
    struct container_t                                                       \
    {                                                                        \
        container_t##_node * root;                                           \
        container_t##_pool pool;                                             \
        container_t##_compare_t compare_function;                            \
        size_t size;                                                         \
    };                                                                       \
    __binary_tree_methods_h(container_t, key_t)

#define static_set_impl(container_t, key_t, container_capacity)                   \
    static_pool_impl(container_t##_pool, container_t##_node, container_capacity); \
    __static_binary_tree_methods_c(container_t, key_t, container_capacity)

#define custom_allocator_set(container_t, key_t, allocator_t)             \
    typedef struct container_t container_t;                               \
    typedef struct container_t##_Iterator container_t##_Iterator;         \
    typedef struct container_t##_node container_t##_node;                 \
    typedef int (*container_t##_compare_t)(const key_t *, const key_t *); \
                                                                          \
    struct container_t##_node                                             \
    {                                                                     \
        container_t##_node * right;                                       \
        container_t##_node * left;                                        \
        container_t##_node * parent;                                      \
        key_t key;                                                        \
    };                                                                    \
                                                                          \
    struct container_t##_Iterator                                         \
    {                                                                     \
        container_t##_node * node;                                        \
        container_t##_node * next;                                        \
        container_t##_node * prev;                                        \
    };                                                                    \
                                                                          \
    struct container_t                                                    \
    {                                                                     \
        container_t##_node * root;                                        \
        allocator_t allocator;                                            \
        container_t##_compare_t compare_function;                         \
        size_t size;                                                      \
    };                                                                    \
    __binary_tree_methods_h(container_t, key_t);

#define custom_allocator_set_impl(container_t, key_t, allocator_t) \
    __custom_allocator_binary_tree_methods_c(container_t, key_t, allocator_t)

#define dynamic_set(container_t, key_t)             \
    dynamic_memory(container_t##_DynamicAllocator); \
    custom_allocator_set(container_t, key_t, container_t##_DynamicAllocator);

#define dynamic_set_impl(container_t, key_t)             \
    dynamic_memory_impl(container_t##_DynamicAllocator); \
    custom_allocator_set_impl(container_t, key_t, container_t##_DynamicAllocator);

#ifdef __cplusplus
}
#endif