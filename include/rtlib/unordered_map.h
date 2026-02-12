#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <rtlib/common.h>
#include <rtlib/error_codes.h>
#include <rtlib/memory.h>
#include <rtlib/pair.h>

#ifndef RTLIB_UNORDERED_MAP_KEY_EQUAL
#define RTLIB_UNORDERED_MAP_KEY_EQUAL(key_t, a, b) (key_t##_Compare((const key_t *)(a), (const key_t *)(b)) == 0)
#endif

#define __unordered_map_methods_h(container_t, key_t, value_t)                                  \
    void container_t##_Construct(container_t * const self);                                     \
    void container_t##_Destruct(container_t * const self);                                      \
    size_t container_t##_Size(const container_t * const self);                                  \
    bool container_t##_Empty(const container_t * const self);                                   \
    int container_t##_Insert(container_t * const self, key_t key, value_t value);               \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator); \
    void container_t##_Clear(container_t * const self);                                         \
    value_t * container_t##_Ref(container_t * const self, key_t key);                           \
    const value_t * container_t##_CRef(const container_t * const self, key_t key);              \
    container_t##_Iterator container_t##_Find(container_t * const self, key_t key);             \
                                                                                                \
    container_t##_Iterator container_t##_Begin(const container_t * const self);                 \
    container_t##_Iterator container_t##_End(const container_t * const self);                   \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,               \
                                      const container_t##_Iterator * const second);             \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self);                 \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self);                 \
    container_t##_Pair container_t##_Iterator_Ref(const container_t##_Iterator * const self);   \
    container_t##_CPair container_t##_Iterator_CRef(const container_t##_Iterator * const self);

#define __static_unordered_map_methods_c(container_t, key_t, value_t, container_capacity)                                \
    enum                                                                                                                 \
    {                                                                                                                    \
        __##container_t##_TABLE_SIZE      = (int)RTLIB_NEXT_POW2_SIZE_T_CONST(((size_t)container_capacity) * (size_t)2), \
        __##container_t##_STATE_EMPTY     = 0u,                                                                          \
        __##container_t##_STATE_OCCUPIED  = 1u,                                                                          \
        __##container_t##_STATE_TOMBSTONE = 2u                                                                           \
    };                                                                                                                   \
                                                                                                                         \
    void container_t##_Construct(container_t * const self)                                                               \
    {                                                                                                                    \
        assert(self);                                                                                                    \
        memset(self, 0, sizeof(*self));                                                                                  \
        self->size       = 0;                                                                                            \
        self->tombstones = 0;                                                                                            \
    }                                                                                                                    \
                                                                                                                         \
    void container_t##_Destruct(container_t * const self)                                                                \
    {                                                                                                                    \
        assert(self);                                                                                                    \
    }                                                                                                                    \
                                                                                                                         \
    size_t container_t##_Size(const container_t * const self)                                                            \
    {                                                                                                                    \
        assert(self);                                                                                                    \
        return self->size;                                                                                               \
    }                                                                                                                    \
                                                                                                                         \
    bool container_t##_Empty(const container_t * const self)                                                             \
    {                                                                                                                    \
        assert(self);                                                                                                    \
        return self->size == 0;                                                                                          \
    }                                                                                                                    \
                                                                                                                         \
    int container_t##_Insert(container_t * const self, key_t key, value_t value)                                         \
    {                                                                                                                    \
        assert(self);                                                                                                    \
                                                                                                                         \
        if(self->size == (size_t)container_capacity)                                                                     \
        {                                                                                                                \
            return ALLOCATION_ERROR;                                                                                     \
        }                                                                                                                \
                                                                                                                         \
        const unsigned int hash_value        = key_t##_Hash((const key_t *)&key);                                        \
        const unsigned int mask              = (unsigned int)__##container_t##_TABLE_SIZE - 1u;                          \
        unsigned int index                   = hash_value & mask;                                                        \
        container_t##_node * first_tombstone = NULL;                                                                     \
                                                                                                                         \
        {                                                                                                                \
            container_t##_node * slot0 = &self->nodes_table[index];                                                      \
            if(slot0->state == __##container_t##_STATE_EMPTY)                                                            \
            {                                                                                                            \
                slot0->key   = key;                                                                                      \
                slot0->val   = value;                                                                                    \
                slot0->state = __##container_t##_STATE_OCCUPIED;                                                         \
                ++self->size;                                                                                            \
                return (int)self->size;                                                                                  \
            }                                                                                                            \
            if(slot0->state == __##container_t##_STATE_OCCUPIED)                                                         \
            {                                                                                                            \
                if(RTLIB_UNORDERED_MAP_KEY_EQUAL(key_t, &key, &slot0->key))                                              \
                {                                                                                                        \
                    return ELEMENT_EXISTS;                                                                               \
                }                                                                                                        \
            }                                                                                                            \
            else if(slot0->state == __##container_t##_STATE_TOMBSTONE)                                                   \
            {                                                                                                            \
                first_tombstone = slot0;                                                                                 \
            }                                                                                                            \
        }                                                                                                                \
                                                                                                                         \
        index = (index + 1u) & mask;                                                                                     \
        for(unsigned int probe = 1; probe < (unsigned int)__##container_t##_TABLE_SIZE; ++probe)                         \
        {                                                                                                                \
            container_t##_node * slot = &self->nodes_table[index];                                                       \
            if(slot->state == __##container_t##_STATE_EMPTY)                                                             \
            {                                                                                                            \
                if(first_tombstone)                                                                                      \
                {                                                                                                        \
                    slot = first_tombstone;                                                                              \
                    --self->tombstones;                                                                                  \
                }                                                                                                        \
                slot->key   = key;                                                                                       \
                slot->val   = value;                                                                                     \
                slot->state = __##container_t##_STATE_OCCUPIED;                                                          \
                ++self->size;                                                                                            \
                return (int)self->size;                                                                                  \
            }                                                                                                            \
            if(slot->state == __##container_t##_STATE_OCCUPIED)                                                          \
            {                                                                                                            \
                if(RTLIB_UNORDERED_MAP_KEY_EQUAL(key_t, &key, &slot->key))                                               \
                {                                                                                                        \
                    return ELEMENT_EXISTS;                                                                               \
                }                                                                                                        \
            }                                                                                                            \
            else if(slot->state == __##container_t##_STATE_TOMBSTONE && !first_tombstone)                                \
            {                                                                                                            \
                first_tombstone = slot;                                                                                  \
            }                                                                                                            \
            index = (index + 1u) & mask;                                                                                 \
        }                                                                                                                \
                                                                                                                         \
        if(first_tombstone)                                                                                              \
        {                                                                                                                \
            first_tombstone->key   = key;                                                                                \
            first_tombstone->val   = value;                                                                              \
            first_tombstone->state = __##container_t##_STATE_OCCUPIED;                                                   \
            --self->tombstones;                                                                                          \
            ++self->size;                                                                                                \
            return (int)self->size;                                                                                      \
        }                                                                                                                \
        return ALLOCATION_ERROR;                                                                                         \
    }                                                                                                                    \
                                                                                                                         \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator)                           \
    {                                                                                                                    \
        assert(self);                                                                                                    \
        assert(iterator);                                                                                                \
        assert(iterator->node);                                                                                          \
        assert(iterator->node->state == __##container_t##_STATE_OCCUPIED);                                               \
                                                                                                                         \
        iterator->node->state = __##container_t##_STATE_TOMBSTONE;                                                       \
        --self->size;                                                                                                    \
        ++self->tombstones;                                                                                              \
        return (int)self->size;                                                                                          \
    }                                                                                                                    \
                                                                                                                         \
    container_t##_Iterator container_t##_Begin(const container_t * const self)                                           \
    {                                                                                                                    \
        assert(self);                                                                                                    \
        container_t##_Iterator it       = { 0 };                                                                         \
        container_t##_node * begin_node = (container_t##_node *)&self->nodes_table[__##container_t##_TABLE_SIZE];        \
        size_t begin_index              = (size_t)__##container_t##_TABLE_SIZE;                                          \
        for(unsigned int i = 0; i < (unsigned int)__##container_t##_TABLE_SIZE; ++i)                                     \
        {                                                                                                                \
            if(self->nodes_table[i].state == __##container_t##_STATE_OCCUPIED)                                           \
            {                                                                                                            \
                begin_node  = (container_t##_node *)&self->nodes_table[i];                                               \
                begin_index = (size_t)i;                                                                                 \
                break;                                                                                                   \
            }                                                                                                            \
        }                                                                                                                \
        it.node      = begin_node;                                                                                       \
        it.container = (container_t *)self;                                                                              \
        it.index     = begin_index;                                                                                      \
        return it;                                                                                                       \
    }                                                                                                                    \
                                                                                                                         \
    container_t##_Iterator container_t##_End(const container_t * const self)                                             \
    {                                                                                                                    \
        assert(self);                                                                                                    \
        container_t##_Iterator it     = { 0 };                                                                           \
        container_t##_node * end_node = (container_t##_node *)&self->nodes_table[__##container_t##_TABLE_SIZE];          \
        it.node                       = end_node;                                                                        \
        it.container                  = (container_t *)self;                                                             \
        it.index                      = (size_t)__##container_t##_TABLE_SIZE;                                            \
        return it;                                                                                                       \
    }                                                                                                                    \
                                                                                                                         \
    value_t * container_t##_Ref(container_t * const self, key_t key)                                                     \
    {                                                                                                                    \
        container_t##_Iterator it  = container_t##_Find(self, key);                                                      \
        container_t##_Iterator end = container_t##_End(self);                                                            \
        if(container_t##_Iterator_Equal(&it, &end))                                                                      \
        {                                                                                                                \
            return NULL;                                                                                                 \
        }                                                                                                                \
        return &it.node->val;                                                                                            \
    }                                                                                                                    \
                                                                                                                         \
    const value_t * container_t##_CRef(const container_t * const self, key_t key)                                        \
    {                                                                                                                    \
        container_t##_Iterator it  = container_t##_Find((container_t *)self, key);                                       \
        container_t##_Iterator end = container_t##_End(self);                                                            \
        if(container_t##_Iterator_Equal(&it, &end))                                                                      \
        {                                                                                                                \
            return NULL;                                                                                                 \
        }                                                                                                                \
        return &it.node->val;                                                                                            \
    }                                                                                                                    \
                                                                                                                         \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                                        \
                                      const container_t##_Iterator * const second)                                       \
    {                                                                                                                    \
        return first->node == second->node;                                                                              \
    }                                                                                                                    \
                                                                                                                         \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self)                                           \
    {                                                                                                                    \
        assert(self);                                                                                                    \
        if(self->index >= (size_t)__##container_t##_TABLE_SIZE)                                                          \
        {                                                                                                                \
            return;                                                                                                      \
        }                                                                                                                \
                                                                                                                         \
        size_t index = self->index + 1;                                                                                  \
        while(index < (size_t)__##container_t##_TABLE_SIZE &&                                                            \
              self->container->nodes_table[index].state != __##container_t##_STATE_OCCUPIED)                             \
        {                                                                                                                \
            ++index;                                                                                                     \
        }                                                                                                                \
        self->index = index;                                                                                             \
        self->node  = (index < (size_t)__##container_t##_TABLE_SIZE)                                                     \
                          ? &self->container->nodes_table[index]                                                         \
                          : &self->container->nodes_table[__##container_t##_TABLE_SIZE];                                 \
    }                                                                                                                    \
                                                                                                                         \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self)                                           \
    {                                                                                                                    \
        assert(self);                                                                                                    \
                                                                                                                         \
        size_t current_index = self->index;                                                                              \
        size_t index = (current_index > (size_t)__##container_t##_TABLE_SIZE) ? (size_t)__##container_t##_TABLE_SIZE     \
                                                                              : current_index;                           \
        while(index > 0)                                                                                                 \
        {                                                                                                                \
            --index;                                                                                                     \
            if(self->container->nodes_table[index].state == __##container_t##_STATE_OCCUPIED)                            \
            {                                                                                                            \
                self->index = index;                                                                                     \
                self->node  = &self->container->nodes_table[index];                                                      \
                return;                                                                                                  \
            }                                                                                                            \
        }                                                                                                                \
        if(current_index == (size_t)__##container_t##_TABLE_SIZE &&                                                      \
           self->container->nodes_table[0].state == __##container_t##_STATE_OCCUPIED)                                    \
        {                                                                                                                \
            self->index = 0;                                                                                             \
            self->node  = &self->container->nodes_table[0];                                                              \
        }                                                                                                                \
    }                                                                                                                    \
                                                                                                                         \
    container_t##_Pair container_t##_Iterator_Ref(const container_t##_Iterator * const self)                             \
    {                                                                                                                    \
        container_t##_Pair ret = { &self->node->key, &self->node->val };                                                 \
        return ret;                                                                                                      \
    }                                                                                                                    \
                                                                                                                         \
    container_t##_CPair container_t##_Iterator_CRef(const container_t##_Iterator * const self)                           \
    {                                                                                                                    \
        container_t##_CPair ret = { &self->node->key, &self->node->val };                                                \
        return ret;                                                                                                      \
    }                                                                                                                    \
                                                                                                                         \
    container_t##_Iterator container_t##_Find(container_t * const self, key_t key)                                       \
    {                                                                                                                    \
        assert(self);                                                                                                    \
        container_t##_Iterator it;                                                                                       \
        it.container = self;                                                                                             \
        it.node      = &self->nodes_table[__##container_t##_TABLE_SIZE];                                                 \
        it.index     = (size_t)__##container_t##_TABLE_SIZE;                                                             \
                                                                                                                         \
        const unsigned int hash_value = key_t##_Hash((const key_t *)&key);                                               \
        const unsigned int mask       = (unsigned int)__##container_t##_TABLE_SIZE - 1u;                                 \
        unsigned int index            = hash_value & mask;                                                               \
                                                                                                                         \
        container_t##_node * slot0 = &self->nodes_table[index];                                                          \
        if(slot0->state == __##container_t##_STATE_EMPTY)                                                                \
        {                                                                                                                \
            return it;                                                                                                   \
        }                                                                                                                \
        if(slot0->state == __##container_t##_STATE_OCCUPIED &&                                                           \
           RTLIB_UNORDERED_MAP_KEY_EQUAL(key_t, &key, &slot0->key))                                                      \
        {                                                                                                                \
            it.node  = slot0;                                                                                            \
            it.index = (size_t)index;                                                                                    \
            return it;                                                                                                   \
        }                                                                                                                \
                                                                                                                         \
        index = (index + 1u) & mask;                                                                                     \
        for(unsigned int probe = 1; probe < (unsigned int)__##container_t##_TABLE_SIZE; ++probe)                         \
        {                                                                                                                \
            container_t##_node * slot = &self->nodes_table[index];                                                       \
            if(slot->state == __##container_t##_STATE_EMPTY)                                                             \
            {                                                                                                            \
                return it;                                                                                               \
            }                                                                                                            \
            if(slot->state == __##container_t##_STATE_OCCUPIED &&                                                        \
               RTLIB_UNORDERED_MAP_KEY_EQUAL(key_t, &key, &slot->key))                                                   \
            {                                                                                                            \
                it.node  = slot;                                                                                         \
                it.index = (size_t)index;                                                                                \
                return it;                                                                                               \
            }                                                                                                            \
            index = (index + 1u) & mask;                                                                                 \
        }                                                                                                                \
        return it;                                                                                                       \
    }                                                                                                                    \
                                                                                                                         \
    void container_t##_Clear(container_t * const self)                                                                   \
    {                                                                                                                    \
        assert(self);                                                                                                    \
        memset(self->nodes_table, 0, sizeof(self->nodes_table));                                                         \
        self->size       = 0;                                                                                            \
        self->tombstones = 0;                                                                                            \
    }

#define __custom_allocator_unordered_map_methods_c(container_t, key_t, value_t, allocator_t)                           \
    enum                                                                                                               \
    {                                                                                                                  \
        __##container_t##_STATE_EMPTY     = 0u,                                                                        \
        __##container_t##_STATE_OCCUPIED  = 1u,                                                                        \
        __##container_t##_STATE_TOMBSTONE = 2u                                                                         \
    };                                                                                                                 \
                                                                                                                       \
    static bool __##container_t##_InsertKeyValueToHashTable(const key_t * key, const value_t * value,                  \
                                                            container_t##_node * table, size_t table_mask)             \
    {                                                                                                                  \
        const unsigned int hash_value        = key_t##_Hash((const key_t *)key);                                       \
        const unsigned int mask              = (unsigned int)table_mask;                                               \
        unsigned int index                   = hash_value & mask;                                                      \
        container_t##_node * first_tombstone = NULL;                                                                   \
                                                                                                                       \
        {                                                                                                              \
            container_t##_node * slot0 = &table[index];                                                                \
            if(slot0->state == __##container_t##_STATE_EMPTY)                                                          \
            {                                                                                                          \
                slot0->key   = *key;                                                                                   \
                slot0->val   = *value;                                                                                 \
                slot0->state = __##container_t##_STATE_OCCUPIED;                                                       \
                return true;                                                                                           \
            }                                                                                                          \
            if(slot0->state == __##container_t##_STATE_OCCUPIED)                                                       \
            {                                                                                                          \
                if(RTLIB_UNORDERED_MAP_KEY_EQUAL(key_t, key, &slot0->key))                                             \
                {                                                                                                      \
                    return false;                                                                                      \
                }                                                                                                      \
            }                                                                                                          \
            else if(slot0->state == __##container_t##_STATE_TOMBSTONE)                                                 \
            {                                                                                                          \
                first_tombstone = slot0;                                                                               \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        index = (index + 1u) & mask;                                                                                   \
        for(unsigned int probe = 1; probe <= mask; ++probe)                                                            \
        {                                                                                                              \
            container_t##_node * slot = &table[index];                                                                 \
            if(slot->state == __##container_t##_STATE_EMPTY)                                                           \
            {                                                                                                          \
                if(first_tombstone)                                                                                    \
                {                                                                                                      \
                    slot = first_tombstone;                                                                            \
                }                                                                                                      \
                slot->key   = *key;                                                                                    \
                slot->val   = *value;                                                                                  \
                slot->state = __##container_t##_STATE_OCCUPIED;                                                        \
                return true;                                                                                           \
            }                                                                                                          \
            if(slot->state == __##container_t##_STATE_OCCUPIED)                                                        \
            {                                                                                                          \
                if(RTLIB_UNORDERED_MAP_KEY_EQUAL(key_t, key, &slot->key))                                              \
                {                                                                                                      \
                    return false;                                                                                      \
                }                                                                                                      \
            }                                                                                                          \
            else if(slot->state == __##container_t##_STATE_TOMBSTONE && !first_tombstone)                              \
            {                                                                                                          \
                first_tombstone = slot;                                                                                \
            }                                                                                                          \
            index = (index + 1u) & mask;                                                                               \
        }                                                                                                              \
        return false;                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    static void __##container_t##_Rehash(container_t * const self, size_t new_nodes_table_size)                        \
    {                                                                                                                  \
        const size_t nodes_table_size_in_bytes = (new_nodes_table_size + 1) * sizeof(self->nodes_table[0]);            \
        container_t##_node * new_nodes_table =                                                                         \
            (container_t##_node *)allocator_t##_Allocate(&self->allocator, nodes_table_size_in_bytes);                 \
        if(!new_nodes_table)                                                                                           \
        {                                                                                                              \
            return;                                                                                                    \
        }                                                                                                              \
        memset(new_nodes_table, 0, nodes_table_size_in_bytes);                                                         \
                                                                                                                       \
        const size_t new_mask = new_nodes_table_size - 1;                                                              \
        for(size_t i = 0; i < self->nodes_table_size; ++i)                                                             \
        {                                                                                                              \
            if(self->nodes_table[i].state == __##container_t##_STATE_OCCUPIED)                                         \
            {                                                                                                          \
                (void)__##container_t##_InsertKeyValueToHashTable((const key_t *)&self->nodes_table[i].key,            \
                                                                  (const value_t *)&self->nodes_table[i].val,          \
                                                                  new_nodes_table, new_mask);                          \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        allocator_t##_Deallocate(&self->allocator, self->nodes_table);                                                 \
        self->nodes_table      = new_nodes_table;                                                                      \
        self->nodes_table_size = new_nodes_table_size;                                                                 \
        self->nodes_table_mask = new_nodes_table_size - 1;                                                             \
        self->tombstones       = 0;                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Construct(container_t * const self)                                                             \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        memset(self, 0, sizeof(*self));                                                                                \
        self->size             = 0;                                                                                    \
        self->tombstones       = 0;                                                                                    \
        self->nodes_table_size = 4;                                                                                    \
        self->nodes_table_mask = self->nodes_table_size - 1;                                                           \
        allocator_t##_Construct(&self->allocator);                                                                     \
        const size_t nodes_table_size_in_bytes = (self->nodes_table_size + 1) * sizeof(self->nodes_table[0]);          \
        self->nodes_table = (container_t##_node *)allocator_t##_Allocate(&self->allocator, nodes_table_size_in_bytes); \
        assert(self->nodes_table);                                                                                     \
        memset(self->nodes_table, 0, nodes_table_size_in_bytes);                                                       \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Destruct(container_t * const self)                                                              \
    {                                                                                                                  \
        (void)container_t##_Clear;                                                                                     \
        allocator_t##_Deallocate(&self->allocator, self->nodes_table);                                                 \
        allocator_t##_Destruct(&self->allocator);                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    size_t container_t##_Size(const container_t * const self)                                                          \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        return self->size;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    bool container_t##_Empty(const container_t * const self)                                                           \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        return self->size == 0;                                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_Insert(container_t * const self, key_t key, value_t value)                                       \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        if((self->size + self->tombstones + 1) * 2 >= self->nodes_table_size)                                          \
        {                                                                                                              \
            size_t new_nodes_table_size = self->nodes_table_size * 2;                                                  \
            __##container_t##_Rehash(self, new_nodes_table_size);                                                      \
            if(self->nodes_table_size != new_nodes_table_size)                                                         \
            {                                                                                                          \
                return ALLOCATION_ERROR;                                                                               \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        const unsigned int hash_value        = key_t##_Hash((const key_t *)&key);                                      \
        const unsigned int mask              = (unsigned int)self->nodes_table_mask;                                   \
        unsigned int index                   = hash_value & mask;                                                      \
        container_t##_node * first_tombstone = NULL;                                                                   \
                                                                                                                       \
        {                                                                                                              \
            container_t##_node * slot0 = &self->nodes_table[index];                                                    \
            if(slot0->state == __##container_t##_STATE_EMPTY)                                                          \
            {                                                                                                          \
                slot0->key   = key;                                                                                    \
                slot0->val   = value;                                                                                  \
                slot0->state = __##container_t##_STATE_OCCUPIED;                                                       \
                ++self->size;                                                                                          \
                return (int)self->size;                                                                                \
            }                                                                                                          \
            if(slot0->state == __##container_t##_STATE_OCCUPIED)                                                       \
            {                                                                                                          \
                if(RTLIB_UNORDERED_MAP_KEY_EQUAL(key_t, &key, &slot0->key))                                            \
                {                                                                                                      \
                    return ELEMENT_EXISTS;                                                                             \
                }                                                                                                      \
            }                                                                                                          \
            else if(slot0->state == __##container_t##_STATE_TOMBSTONE)                                                 \
            {                                                                                                          \
                first_tombstone = slot0;                                                                               \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        index = (index + 1u) & mask;                                                                                   \
        for(unsigned int probe = 1; probe <= mask; ++probe)                                                            \
        {                                                                                                              \
            container_t##_node * slot = &self->nodes_table[index];                                                     \
            if(slot->state == __##container_t##_STATE_EMPTY)                                                           \
            {                                                                                                          \
                if(first_tombstone)                                                                                    \
                {                                                                                                      \
                    slot = first_tombstone;                                                                            \
                    --self->tombstones;                                                                                \
                }                                                                                                      \
                slot->key   = key;                                                                                     \
                slot->val   = value;                                                                                   \
                slot->state = __##container_t##_STATE_OCCUPIED;                                                        \
                ++self->size;                                                                                          \
                return (int)self->size;                                                                                \
            }                                                                                                          \
            if(slot->state == __##container_t##_STATE_OCCUPIED)                                                        \
            {                                                                                                          \
                if(RTLIB_UNORDERED_MAP_KEY_EQUAL(key_t, &key, &slot->key))                                             \
                {                                                                                                      \
                    return ELEMENT_EXISTS;                                                                             \
                }                                                                                                      \
            }                                                                                                          \
            else if(slot->state == __##container_t##_STATE_TOMBSTONE && !first_tombstone)                              \
            {                                                                                                          \
                first_tombstone = slot;                                                                                \
            }                                                                                                          \
            index = (index + 1u) & mask;                                                                               \
        }                                                                                                              \
                                                                                                                       \
        if(first_tombstone)                                                                                            \
        {                                                                                                              \
            first_tombstone->key   = key;                                                                              \
            first_tombstone->val   = value;                                                                            \
            first_tombstone->state = __##container_t##_STATE_OCCUPIED;                                                 \
            --self->tombstones;                                                                                        \
            ++self->size;                                                                                              \
            return (int)self->size;                                                                                    \
        }                                                                                                              \
        return ALLOCATION_ERROR;                                                                                       \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator)                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        assert(iterator);                                                                                              \
        assert(iterator->node);                                                                                        \
        assert(iterator->node->state == __##container_t##_STATE_OCCUPIED);                                             \
                                                                                                                       \
        iterator->node->state = __##container_t##_STATE_TOMBSTONE;                                                     \
        --self->size;                                                                                                  \
        ++self->tombstones;                                                                                            \
        return (int)self->size;                                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_Iterator container_t##_Begin(const container_t * const self)                                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        container_t##_Iterator it       = { 0 };                                                                       \
        container_t##_node * begin_node = (container_t##_node *)&self->nodes_table[self->nodes_table_size];            \
        size_t begin_index              = self->nodes_table_size;                                                      \
        for(unsigned int i = 0; i < self->nodes_table_size; ++i)                                                       \
        {                                                                                                              \
            if(self->nodes_table[i].state == __##container_t##_STATE_OCCUPIED)                                         \
            {                                                                                                          \
                begin_node  = (container_t##_node *)&self->nodes_table[i];                                             \
                begin_index = (size_t)i;                                                                               \
                break;                                                                                                 \
            }                                                                                                          \
        }                                                                                                              \
        it.node      = begin_node;                                                                                     \
        it.container = (container_t *)self;                                                                            \
        it.index     = begin_index;                                                                                    \
        return it;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_Iterator container_t##_End(const container_t * const self)                                           \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        container_t##_Iterator it     = { 0 };                                                                         \
        container_t##_node * end_node = (container_t##_node *)&self->nodes_table[self->nodes_table_size];              \
        it.node                       = end_node;                                                                      \
        it.container                  = (container_t *)self;                                                           \
        it.index                      = self->nodes_table_size;                                                        \
        return it;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    value_t * container_t##_Ref(container_t * const self, key_t key)                                                   \
    {                                                                                                                  \
        container_t##_Iterator it  = container_t##_Find(self, key);                                                    \
        container_t##_Iterator end = container_t##_End(self);                                                          \
        if(container_t##_Iterator_Equal(&it, &end))                                                                    \
        {                                                                                                              \
            return NULL;                                                                                               \
        }                                                                                                              \
        return &it.node->val;                                                                                          \
    }                                                                                                                  \
                                                                                                                       \
    const value_t * container_t##_CRef(const container_t * const self, key_t key)                                      \
    {                                                                                                                  \
        container_t##_Iterator it  = container_t##_Find((container_t *)self, key);                                     \
        container_t##_Iterator end = container_t##_End(self);                                                          \
        if(container_t##_Iterator_Equal(&it, &end))                                                                    \
        {                                                                                                              \
            return NULL;                                                                                               \
        }                                                                                                              \
        return &it.node->val;                                                                                          \
    }                                                                                                                  \
                                                                                                                       \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                                      \
                                      const container_t##_Iterator * const second)                                     \
    {                                                                                                                  \
        return first->node == second->node;                                                                            \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self)                                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        if(self->index >= self->container->nodes_table_size)                                                           \
        {                                                                                                              \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        size_t index = self->index + 1;                                                                                \
        while(index < self->container->nodes_table_size &&                                                             \
              self->container->nodes_table[index].state != __##container_t##_STATE_OCCUPIED)                           \
        {                                                                                                              \
            ++index;                                                                                                   \
        }                                                                                                              \
        self->index = index;                                                                                           \
        self->node  = (index < self->container->nodes_table_size)                                                      \
                          ? &self->container->nodes_table[index]                                                       \
                          : &self->container->nodes_table[self->container->nodes_table_size];                          \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self)                                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        size_t current_index = self->index;                                                                            \
        size_t index =                                                                                                 \
            (current_index > self->container->nodes_table_size) ? self->container->nodes_table_size : current_index;   \
        while(index > 0)                                                                                               \
        {                                                                                                              \
            --index;                                                                                                   \
            if(self->container->nodes_table[index].state == __##container_t##_STATE_OCCUPIED)                          \
            {                                                                                                          \
                self->index = index;                                                                                   \
                self->node  = &self->container->nodes_table[index];                                                    \
                return;                                                                                                \
            }                                                                                                          \
        }                                                                                                              \
        if(current_index == self->container->nodes_table_size &&                                                       \
           self->container->nodes_table[0].state == __##container_t##_STATE_OCCUPIED)                                  \
        {                                                                                                              \
            self->index = 0;                                                                                           \
            self->node  = &self->container->nodes_table[0];                                                            \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_Pair container_t##_Iterator_Ref(const container_t##_Iterator * const self)                           \
    {                                                                                                                  \
        container_t##_Pair ret = { &self->node->key, &self->node->val };                                               \
        return ret;                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_CPair container_t##_Iterator_CRef(const container_t##_Iterator * const self)                         \
    {                                                                                                                  \
        container_t##_CPair ret = { &self->node->key, &self->node->val };                                              \
        return ret;                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_Iterator container_t##_Find(container_t * const self, key_t key)                                     \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        container_t##_Iterator it;                                                                                     \
        it.container = self;                                                                                           \
        it.node      = &self->nodes_table[self->nodes_table_size];                                                     \
        it.index     = self->nodes_table_size;                                                                         \
                                                                                                                       \
        const unsigned int hash_value = key_t##_Hash((const key_t *)&key);                                             \
        const unsigned int mask       = (unsigned int)self->nodes_table_mask;                                          \
        unsigned int index            = hash_value & mask;                                                             \
                                                                                                                       \
        container_t##_node * slot0 = &self->nodes_table[index];                                                        \
        if(slot0->state == __##container_t##_STATE_EMPTY)                                                              \
        {                                                                                                              \
            return it;                                                                                                 \
        }                                                                                                              \
        if(slot0->state == __##container_t##_STATE_OCCUPIED &&                                                         \
           RTLIB_UNORDERED_MAP_KEY_EQUAL(key_t, &key, &slot0->key))                                                    \
        {                                                                                                              \
            it.node  = slot0;                                                                                          \
            it.index = (size_t)index;                                                                                  \
            return it;                                                                                                 \
        }                                                                                                              \
                                                                                                                       \
        index = (index + 1u) & mask;                                                                                   \
        for(unsigned int probe = 1; probe <= mask; ++probe)                                                            \
        {                                                                                                              \
            container_t##_node * slot = &self->nodes_table[index];                                                     \
            if(slot->state == __##container_t##_STATE_EMPTY)                                                           \
            {                                                                                                          \
                return it;                                                                                             \
            }                                                                                                          \
            if(slot->state == __##container_t##_STATE_OCCUPIED &&                                                      \
               RTLIB_UNORDERED_MAP_KEY_EQUAL(key_t, &key, &slot->key))                                                 \
            {                                                                                                          \
                it.node  = slot;                                                                                       \
                it.index = (size_t)index;                                                                              \
                return it;                                                                                             \
            }                                                                                                          \
            index = (index + 1u) & mask;                                                                               \
        }                                                                                                              \
        return it;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Clear(container_t * const self)                                                                 \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        const size_t nodes_table_size_in_bytes = (self->nodes_table_size + 1) * sizeof(self->nodes_table[0]);          \
        memset(self->nodes_table, 0, nodes_table_size_in_bytes);                                                       \
        self->size       = 0;                                                                                          \
        self->tombstones = 0;                                                                                          \
    }

#define static_unordered_map(container_t, key_t, value_t, container_capacity)                                       \
    pair(container_t##_Pair, const key_t *, value_t *);                                                             \
    pair(container_t##_CPair, const key_t *, const value_t *);                                                      \
    typedef struct container_t container_t;                                                                         \
    typedef struct container_t##_Iterator container_t##_Iterator;                                                   \
    typedef struct container_t##_node container_t##_node;                                                           \
                                                                                                                    \
    struct container_t##_node                                                                                       \
    {                                                                                                               \
        key_t key;                                                                                                  \
        value_t val;                                                                                                \
        unsigned char state;                                                                                        \
    };                                                                                                              \
                                                                                                                    \
    struct container_t##_Iterator                                                                                   \
    {                                                                                                               \
        container_t##_node * node;                                                                                  \
        container_t * container;                                                                                    \
        size_t index;                                                                                               \
    };                                                                                                              \
                                                                                                                    \
    struct container_t                                                                                              \
    {                                                                                                               \
        container_t##_node nodes_table[RTLIB_NEXT_POW2_SIZE_T_CONST(((size_t)container_capacity) * (size_t)2) + 1]; \
        size_t size;                                                                                                \
        size_t tombstones;                                                                                          \
    };                                                                                                              \
    __unordered_map_methods_h(container_t, key_t, value_t)

#define static_unordered_map_impl(container_t, key_t, value_t, container_capacity) \
    __static_unordered_map_methods_c(container_t, key_t, value_t, container_capacity)

#define custom_allocator_unordered_map(container_t, key_t, value_t, allocator_t) \
    pair(container_t##_Pair, const key_t *, value_t *);                          \
    pair(container_t##_CPair, const key_t *, const value_t *);                   \
    typedef struct container_t container_t;                                      \
    typedef struct container_t##_Iterator container_t##_Iterator;                \
    typedef struct container_t##_node container_t##_node;                        \
                                                                                 \
    struct container_t##_node                                                    \
    {                                                                            \
        key_t key;                                                               \
        value_t val;                                                             \
        unsigned char state;                                                     \
    };                                                                           \
                                                                                 \
    struct container_t##_Iterator                                                \
    {                                                                            \
        container_t##_node * node;                                               \
        container_t * container;                                                 \
        size_t index;                                                            \
    };                                                                           \
                                                                                 \
    struct container_t                                                           \
    {                                                                            \
        container_t##_node * nodes_table;                                        \
        size_t nodes_table_size;                                                 \
        size_t nodes_table_mask;                                                 \
        size_t size;                                                             \
        size_t tombstones;                                                       \
        allocator_t allocator;                                                   \
    };                                                                           \
    __unordered_map_methods_h(container_t, key_t, value_t)

#define custom_allocator_unordered_map_impl(container_t, key_t, value_t, allocator_t) \
    __custom_allocator_unordered_map_methods_c(container_t, key_t, value_t, allocator_t)

#define dynamic_unordered_map(container_t, key_t, value_t) \
    dynamic_memory(container_t##_DynamicAllocator);        \
    custom_allocator_unordered_map(container_t, key_t, value_t, container_t##_DynamicAllocator);

#define dynamic_unordered_map_impl(container_t, key_t, value_t) \
    dynamic_memory_impl(container_t##_DynamicAllocator);        \
    custom_allocator_unordered_map_impl(container_t, key_t, value_t, container_t##_DynamicAllocator);

#ifdef __cplusplus
}
#endif
