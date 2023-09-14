#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "hash_table.h"
#include "rtlib/memory.h"

#define static_unordered_set(container_t, key_t, container_capacity)         \
    typedef struct container_t container_t;                                  \
    typedef struct container_t##_Iterator container_t##_Iterator;            \
    typedef struct container_t##_node container_t##_node;                    \
                                                                             \
    typedef int (*container_t##_compare_t)(const key_t *, const key_t *);    \
    typedef unsigned int (*container_t##_hash_t)(const key_t *);             \
    struct container_t##_node                                                \
    {                                                                        \
        key_t key;                                                           \
        container_t##_node * prev;                                           \
        container_t##_node * next;                                           \
    };                                                                       \
                                                                             \
    static_pool(container_t##_pool, container_t##_node, container_capacity); \
                                                                             \
    struct container_t##_Iterator                                            \
    {                                                                        \
        container_t##_node * node;                                           \
        container_t * container;                                             \
    };                                                                       \
    struct container_t                                                       \
    {                                                                        \
        container_t##_node * nodes_table[container_capacity + 1];            \
        container_t##_compare_t compare_function;                            \
        container_t##_hash_t hash_function;                                  \
        size_t size;                                                         \
        container_t##_pool pool;                                             \
    };                                                                       \
    __hash_table_methods_h(container_t, key_t);

#define static_unordered_set_impl(container_t, key_t, container_capacity)         \
    static_pool_impl(container_t##_pool, container_t##_node, container_capacity); \
    __static_hash_table_methods_c(container_t, key_t, container_capacity);

#define custom_allocator_unordered_set(container_t, key_t, allocator_t)   \
    typedef struct container_t container_t;                               \
    typedef struct container_t##_Iterator container_t##_Iterator;         \
    typedef struct container_t##_node container_t##_node;                 \
                                                                          \
    typedef int (*container_t##_compare_t)(const key_t *, const key_t *); \
    typedef unsigned int (*container_t##_hash_t)(const key_t *);          \
                                                                          \
    struct container_t##_node                                             \
    {                                                                     \
        key_t key;                                                        \
        container_t##_node * prev;                                        \
        container_t##_node * next;                                        \
    };                                                                    \
                                                                          \
    struct container_t##_Iterator                                         \
    {                                                                     \
        container_t##_node * node;                                        \
        container_t * container;                                          \
    };                                                                    \
                                                                          \
    struct container_t                                                    \
    {                                                                     \
        container_t##_node ** nodes_table;                                \
        size_t nodes_table_size;                                          \
        container_t##_compare_t compare_function;                         \
        container_t##_hash_t hash_function;                               \
        size_t size;                                                      \
        allocator_t allocator;                                            \
    };                                                                    \
    __hash_table_methods_h(container_t, key_t);

#define custom_allocator_unordered_set_impl(container_t, key_t, container_capacity) \
    __custom_allocator_hash_table_methods_c(container_t, key_t, container_capacity);

#define dynamic_unordered_set(container_t, key_t)   \
    dynamic_memory(container_t##_DynamicAllocator); \
    custom_allocator_unordered_set(container_t, key_t, container_t##_DynamicAllocator);

#define dynamic__unordered_set_impl(container_t, key_t)  \
    dynamic_memory_impl(container_t##_DynamicAllocator); \
    custom_allocator_unordered_set_impl(container_t, key_t, container_t##_DynamicAllocator);

#ifdef __cplusplus
}
#endif