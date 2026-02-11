#pragma once

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "error_codes.h"
#include "rtlib/memory.h"
#include "rtlib/pool.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define __deque_methods_h(container_t, member_t)                                                                \
    void container_t##_Construct(container_t * const self);                                                     \
    void container_t##_Destruct(container_t * const self);                                                      \
    size_t container_t##_Size(const container_t * const self);                                                  \
    bool container_t##_Empty(const container_t * const self);                                                   \
    int container_t##_PushBack(container_t * const self, member_t data);                                        \
    int container_t##_PopBack(container_t * const self);                                                        \
    int container_t##_PushFront(container_t * const self, member_t data);                                       \
    int container_t##_PopFront(container_t * const self);                                                       \
    int container_t##_Insert(container_t * const self, container_t##_Iterator * const iterator, member_t data); \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator);                 \
    member_t container_t##_Front(const container_t * const self);                                               \
    member_t container_t##_Back(const container_t * const self);                                                \
    void container_t##_Clear(container_t * const self);                                                         \
    member_t * container_t##_Ref(container_t * const self, size_t index);                                       \
    const member_t * container_t##_CRef(const container_t * const self, size_t index);                          \
    container_t##_Iterator container_t##_Find(container_t * const self, const member_t data);                   \
                                                                                                                \
    container_t##_Iterator container_t##_Begin(const container_t * const self);                                 \
    container_t##_Iterator container_t##_End(const container_t * const self);                                   \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                               \
                                      const container_t##_Iterator * const second);                             \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self);                                 \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self);                                 \
    member_t * container_t##_Iterator_Ref(container_t##_Iterator * const self);                                 \
    const member_t * container_t##_Iterator_CRef(const container_t##_Iterator * const self);

#ifndef RTLIB_DEQUE_BLOCK_SIZE
#define RTLIB_DEQUE_BLOCK_SIZE 32
#endif

#define __deque_common_helpers(container_t, member_t)                                            \
    static size_t __##container_t##_BlockSize(void)                                              \
    {                                                                                            \
        return (size_t)RTLIB_DEQUE_BLOCK_SIZE;                                                   \
    }                                                                                            \
                                                                                                 \
    static size_t __##container_t##_NextIndex(size_t index, size_t capacity)                     \
    {                                                                                            \
        return (index + 1) % capacity;                                                           \
    }                                                                                            \
                                                                                                 \
    static size_t __##container_t##_PrevIndex(size_t index, size_t capacity)                     \
    {                                                                                            \
        return (index + capacity - 1) % capacity;                                                \
    }                                                                                            \
                                                                                                 \
    static size_t __##container_t##_BlockIndex(const container_t * const self, size_t index)     \
    {                                                                                            \
        const size_t block_offset = (self->head_offset + index) / __##container_t##_BlockSize(); \
        return (self->head_block + block_offset) % self->map_capacity;                           \
    }                                                                                            \
                                                                                                 \
    static size_t __##container_t##_BlockOffset(const container_t * const self, size_t index)    \
    {                                                                                            \
        const size_t offset = (self->head_offset + index) % __##container_t##_BlockSize();       \
        return offset;                                                                           \
    }

#define __deque_core_methods_c(container_t, member_t, allocate_block_fn, release_block_fn, grow_map_fn,        \
                               can_grow_map_fn)                                                                \
    __deque_common_helpers(container_t, member_t)                                                              \
                                                                                                               \
        size_t                                                                                                 \
        container_t##_Size(const container_t * const self)                                                     \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        return self->size;                                                                                     \
    }                                                                                                          \
                                                                                                               \
    bool container_t##_Empty(const container_t * const self)                                                   \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        return self->size == 0;                                                                                \
    }                                                                                                          \
                                                                                                               \
    static int __##container_t##_EnsureBlockForEmpty(container_t * const self)                                 \
    {                                                                                                          \
        if(self->block_count != 0)                                                                             \
        {                                                                                                      \
            return OK;                                                                                         \
        }                                                                                                      \
        if(self->block_count == self->map_capacity)                                                            \
        {                                                                                                      \
            if(can_grow_map_fn(self))                                                                          \
            {                                                                                                  \
                if(!grow_map_fn(self))                                                                         \
                {                                                                                              \
                    return ALLOCATION_ERROR;                                                                   \
                }                                                                                              \
            }                                                                                                  \
            else                                                                                               \
            {                                                                                                  \
                return ALLOCATION_ERROR;                                                                       \
            }                                                                                                  \
        }                                                                                                      \
        container_t##_block * block = allocate_block_fn(self);                                                 \
        if(!block)                                                                                             \
        {                                                                                                      \
            return ALLOCATION_ERROR;                                                                           \
        }                                                                                                      \
        self->head_block            = self->map_capacity / 2;                                                  \
        self->tail_block            = self->head_block;                                                        \
        self->head_offset           = __##container_t##_BlockSize() / 2;                                       \
        self->tail_offset           = self->head_offset;                                                       \
        self->map[self->head_block] = block;                                                                   \
        self->block_count           = 1;                                                                       \
        return OK;                                                                                             \
    }                                                                                                          \
                                                                                                               \
    static int __##container_t##_EnsureBackBlock(container_t * const self)                                     \
    {                                                                                                          \
        if(self->block_count == 0)                                                                             \
        {                                                                                                      \
            return __##container_t##_EnsureBlockForEmpty(self);                                                \
        }                                                                                                      \
        if(self->tail_offset < __##container_t##_BlockSize())                                                  \
        {                                                                                                      \
            return OK;                                                                                         \
        }                                                                                                      \
        if(self->block_count == self->map_capacity)                                                            \
        {                                                                                                      \
            if(can_grow_map_fn(self))                                                                          \
            {                                                                                                  \
                if(!grow_map_fn(self))                                                                         \
                {                                                                                              \
                    return ALLOCATION_ERROR;                                                                   \
                }                                                                                              \
            }                                                                                                  \
            else                                                                                               \
            {                                                                                                  \
                return ALLOCATION_ERROR;                                                                       \
            }                                                                                                  \
        }                                                                                                      \
        size_t new_tail             = __##container_t##_NextIndex(self->tail_block, self->map_capacity);       \
        container_t##_block * block = allocate_block_fn(self);                                                 \
        if(!block)                                                                                             \
        {                                                                                                      \
            return ALLOCATION_ERROR;                                                                           \
        }                                                                                                      \
        self->map[new_tail] = block;                                                                           \
        self->tail_block    = new_tail;                                                                        \
        self->tail_offset   = 0;                                                                               \
        self->block_count++;                                                                                   \
        return OK;                                                                                             \
    }                                                                                                          \
                                                                                                               \
    static int __##container_t##_EnsureFrontBlock(container_t * const self)                                    \
    {                                                                                                          \
        if(self->block_count == 0)                                                                             \
        {                                                                                                      \
            return __##container_t##_EnsureBlockForEmpty(self);                                                \
        }                                                                                                      \
        if(self->head_offset > 0)                                                                              \
        {                                                                                                      \
            return OK;                                                                                         \
        }                                                                                                      \
        if(self->block_count == self->map_capacity)                                                            \
        {                                                                                                      \
            if(can_grow_map_fn(self))                                                                          \
            {                                                                                                  \
                if(!grow_map_fn(self))                                                                         \
                {                                                                                              \
                    return ALLOCATION_ERROR;                                                                   \
                }                                                                                              \
            }                                                                                                  \
            else                                                                                               \
            {                                                                                                  \
                return ALLOCATION_ERROR;                                                                       \
            }                                                                                                  \
        }                                                                                                      \
        size_t new_head             = __##container_t##_PrevIndex(self->head_block, self->map_capacity);       \
        container_t##_block * block = allocate_block_fn(self);                                                 \
        if(!block)                                                                                             \
        {                                                                                                      \
            return ALLOCATION_ERROR;                                                                           \
        }                                                                                                      \
        self->map[new_head] = block;                                                                           \
        self->head_block    = new_head;                                                                        \
        self->head_offset   = __##container_t##_BlockSize();                                                   \
        self->block_count++;                                                                                   \
        return OK;                                                                                             \
    }                                                                                                          \
                                                                                                               \
    int container_t##_PushBack(container_t * const self, member_t data)                                        \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        if(self->size >= self->max_size)                                                                       \
        {                                                                                                      \
            return ALLOCATION_ERROR;                                                                           \
        }                                                                                                      \
        if(__##container_t##_EnsureBackBlock(self) != OK)                                                      \
        {                                                                                                      \
            return ALLOCATION_ERROR;                                                                           \
        }                                                                                                      \
        self->map[self->tail_block]->values[self->tail_offset] = data;                                         \
        self->tail_offset++;                                                                                   \
        self->size++;                                                                                          \
        return (int)self->size;                                                                                \
    }                                                                                                          \
                                                                                                               \
    int container_t##_PushFront(container_t * const self, member_t data)                                       \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        if(self->size >= self->max_size)                                                                       \
        {                                                                                                      \
            return ALLOCATION_ERROR;                                                                           \
        }                                                                                                      \
        if(__##container_t##_EnsureFrontBlock(self) != OK)                                                     \
        {                                                                                                      \
            return ALLOCATION_ERROR;                                                                           \
        }                                                                                                      \
        self->head_offset--;                                                                                   \
        self->map[self->head_block]->values[self->head_offset] = data;                                         \
        self->size++;                                                                                          \
        return (int)self->size;                                                                                \
    }                                                                                                          \
                                                                                                               \
    int container_t##_PopBack(container_t * const self)                                                        \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(self->size > 0);                                                                                \
                                                                                                               \
        if(self->tail_offset == 0)                                                                             \
        {                                                                                                      \
            size_t old_tail   = self->tail_block;                                                              \
            self->tail_block  = __##container_t##_PrevIndex(self->tail_block, self->map_capacity);             \
            self->tail_offset = __##container_t##_BlockSize();                                                 \
            release_block_fn(self, self->map[old_tail]);                                                       \
            self->map[old_tail] = NULL;                                                                        \
            self->block_count--;                                                                               \
        }                                                                                                      \
        self->tail_offset--;                                                                                   \
        self->size--;                                                                                          \
                                                                                                               \
        if(self->size == 0)                                                                                    \
        {                                                                                                      \
            self->head_block  = self->tail_block;                                                              \
            self->head_offset = self->tail_offset;                                                             \
        }                                                                                                      \
        return (int)self->size;                                                                                \
    }                                                                                                          \
                                                                                                               \
    int container_t##_PopFront(container_t * const self)                                                       \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(self->size > 0);                                                                                \
                                                                                                               \
        self->head_offset++;                                                                                   \
        self->size--;                                                                                          \
                                                                                                               \
        if(self->head_offset == __##container_t##_BlockSize())                                                 \
        {                                                                                                      \
            size_t old_head   = self->head_block;                                                              \
            self->head_block  = __##container_t##_NextIndex(self->head_block, self->map_capacity);             \
            self->head_offset = 0;                                                                             \
            release_block_fn(self, self->map[old_head]);                                                       \
            self->map[old_head] = NULL;                                                                        \
            self->block_count--;                                                                               \
        }                                                                                                      \
                                                                                                               \
        if(self->size == 0)                                                                                    \
        {                                                                                                      \
            self->tail_block  = self->head_block;                                                              \
            self->tail_offset = self->head_offset;                                                             \
        }                                                                                                      \
        return (int)self->size;                                                                                \
    }                                                                                                          \
                                                                                                               \
    member_t container_t##_Front(const container_t * const self)                                               \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(self->size > 0);                                                                                \
                                                                                                               \
        return self->map[self->head_block]->values[self->head_offset];                                         \
    }                                                                                                          \
                                                                                                               \
    member_t container_t##_Back(const container_t * const self)                                                \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(self->size > 0);                                                                                \
                                                                                                               \
        size_t tail_block  = self->tail_block;                                                                 \
        size_t tail_offset = self->tail_offset;                                                                \
        if(tail_offset == 0)                                                                                   \
        {                                                                                                      \
            tail_block  = __##container_t##_PrevIndex(tail_block, self->map_capacity);                         \
            tail_offset = __##container_t##_BlockSize();                                                       \
        }                                                                                                      \
        return self->map[tail_block]->values[tail_offset - 1];                                                 \
    }                                                                                                          \
                                                                                                               \
    member_t * container_t##_Ref(container_t * const self, size_t index)                                       \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(index < self->size);                                                                            \
                                                                                                               \
        const size_t block_index = __##container_t##_BlockIndex(self, index);                                  \
        const size_t offset      = __##container_t##_BlockOffset(self, index);                                 \
        return &self->map[block_index]->values[offset];                                                        \
    }                                                                                                          \
                                                                                                               \
    const member_t * container_t##_CRef(const container_t * const self, size_t index)                          \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(index < self->size);                                                                            \
                                                                                                               \
        const size_t block_index = __##container_t##_BlockIndex(self, index);                                  \
        const size_t offset      = __##container_t##_BlockOffset(self, index);                                 \
        return &self->map[block_index]->values[offset];                                                        \
    }                                                                                                          \
                                                                                                               \
    container_t##_Iterator container_t##_Begin(const container_t * const self)                                 \
    {                                                                                                          \
        container_t##_Iterator tmp = { 0, self };                                                              \
        return tmp;                                                                                            \
    }                                                                                                          \
                                                                                                               \
    container_t##_Iterator container_t##_End(const container_t * const self)                                   \
    {                                                                                                          \
        container_t##_Iterator tmp = { self->size, self };                                                     \
        return tmp;                                                                                            \
    }                                                                                                          \
                                                                                                               \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                              \
                                      const container_t##_Iterator * const second)                             \
    {                                                                                                          \
        return first->index == second->index;                                                                  \
    }                                                                                                          \
                                                                                                               \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self)                                 \
    {                                                                                                          \
        self->index++;                                                                                         \
    }                                                                                                          \
                                                                                                               \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self)                                 \
    {                                                                                                          \
        self->index--;                                                                                         \
    }                                                                                                          \
                                                                                                               \
    member_t * container_t##_Iterator_Ref(container_t##_Iterator * const self)                                 \
    {                                                                                                          \
        return container_t##_Ref((container_t *)self->owner, self->index);                                     \
    }                                                                                                          \
                                                                                                               \
    const member_t * container_t##_Iterator_CRef(const container_t##_Iterator * const self)                    \
    {                                                                                                          \
        return container_t##_CRef(self->owner, self->index);                                                   \
    }                                                                                                          \
                                                                                                               \
    container_t##_Iterator container_t##_Find(container_t * const self, const member_t data)                   \
    {                                                                                                          \
        container_t##_Iterator end = container_t##_End(self);                                                  \
        container_t##_Iterator it  = container_t##_Begin(self);                                                \
                                                                                                               \
        for(; !container_t##_Iterator_Equal(&it, &end); container_t##_Iterator_Increment(&it))                 \
        {                                                                                                      \
            const member_t it_value = *container_t##_Iterator_CRef(&it);                                       \
            if(member_t##_Compare(&data, &it_value) == 0)                                                      \
            {                                                                                                  \
                break;                                                                                         \
            }                                                                                                  \
        }                                                                                                      \
        return it;                                                                                             \
    }                                                                                                          \
                                                                                                               \
    void container_t##_Clear(container_t * const self)                                                         \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        while(self->size != 0)                                                                                 \
        {                                                                                                      \
            container_t##_PopBack(self);                                                                       \
        }                                                                                                      \
    }                                                                                                          \
                                                                                                               \
    int container_t##_Insert(container_t * const self, container_t##_Iterator * const iterator, member_t data) \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(iterator);                                                                                      \
                                                                                                               \
        if(iterator->index == 0)                                                                               \
        {                                                                                                      \
            return container_t##_PushFront(self, data);                                                        \
        }                                                                                                      \
        if(iterator->index == self->size)                                                                      \
        {                                                                                                      \
            return container_t##_PushBack(self, data);                                                         \
        }                                                                                                      \
                                                                                                               \
        if(iterator->index < self->size / 2)                                                                   \
        {                                                                                                      \
            if(container_t##_PushFront(self, container_t##_Front(self)) < 0)                                   \
            {                                                                                                  \
                return ALLOCATION_ERROR;                                                                       \
            }                                                                                                  \
            for(size_t i = 0; i < iterator->index; ++i)                                                        \
            {                                                                                                  \
                *container_t##_Ref(self, i) = *container_t##_CRef(self, i + 1);                                \
            }                                                                                                  \
            *container_t##_Ref(self, iterator->index) = data;                                                  \
        }                                                                                                      \
        else                                                                                                   \
        {                                                                                                      \
            if(container_t##_PushBack(self, container_t##_Back(self)) < 0)                                     \
            {                                                                                                  \
                return ALLOCATION_ERROR;                                                                       \
            }                                                                                                  \
            for(size_t i = self->size - 1; i > iterator->index; --i)                                           \
            {                                                                                                  \
                *container_t##_Ref(self, i) = *container_t##_CRef(self, i - 1);                                \
            }                                                                                                  \
            *container_t##_Ref(self, iterator->index) = data;                                                  \
        }                                                                                                      \
        return (int)self->size;                                                                                \
    }                                                                                                          \
                                                                                                               \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator)                 \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(iterator);                                                                                      \
                                                                                                               \
        if(iterator->index == 0)                                                                               \
        {                                                                                                      \
            return container_t##_PopFront(self);                                                               \
        }                                                                                                      \
        if(iterator->index == self->size - 1)                                                                  \
        {                                                                                                      \
            return container_t##_PopBack(self);                                                                \
        }                                                                                                      \
                                                                                                               \
        if(iterator->index < self->size / 2)                                                                   \
        {                                                                                                      \
            for(size_t i = iterator->index; i > 0; --i)                                                        \
            {                                                                                                  \
                *container_t##_Ref(self, i) = *container_t##_CRef(self, i - 1);                                \
            }                                                                                                  \
            return container_t##_PopFront(self);                                                               \
        }                                                                                                      \
        else                                                                                                   \
        {                                                                                                      \
            for(size_t i = iterator->index; i < self->size - 1; ++i)                                           \
            {                                                                                                  \
                *container_t##_Ref(self, i) = *container_t##_CRef(self, i + 1);                                \
            }                                                                                                  \
            return container_t##_PopBack(self);                                                                \
        }                                                                                                      \
    }

#define static_deque(container_t, member_t, container_capacity)                                               \
    typedef struct container_t container_t;                                                                   \
    typedef struct container_t##_Iterator container_t##_Iterator;                                             \
    typedef struct container_t##_block container_t##_block;                                                   \
    typedef int (*container_t##_compare_t)(const member_t *, const member_t *);                               \
    struct container_t##_block                                                                                \
    {                                                                                                         \
        member_t values[RTLIB_DEQUE_BLOCK_SIZE];                                                              \
    };                                                                                                        \
    struct container_t##_Iterator                                                                             \
    {                                                                                                         \
        size_t index;                                                                                         \
        const container_t * owner;                                                                            \
    };                                                                                                        \
    enum                                                                                                      \
    {                                                                                                         \
        container_t##_max_blocks = (container_capacity + RTLIB_DEQUE_BLOCK_SIZE - 1) / RTLIB_DEQUE_BLOCK_SIZE \
    };                                                                                                        \
    static_pool(container_t##_block_pool, container_t##_block, container_t##_max_blocks);                     \
    struct container_t                                                                                        \
    {                                                                                                         \
        container_t##_block * map[container_t##_max_blocks];                                                  \
        size_t map_capacity;                                                                                  \
        size_t head_block;                                                                                    \
        size_t tail_block;                                                                                    \
        size_t head_offset;                                                                                   \
        size_t tail_offset;                                                                                   \
        size_t size;                                                                                          \
        size_t block_count;                                                                                   \
        size_t max_size;                                                                                      \
        container_t##_block_pool pool;                                                                        \
    };                                                                                                        \
    __deque_methods_h(container_t, member_t)

#define static_deque_impl(container_t, member_t, container_capacity)                                               \
    static_pool_impl(container_t##_block_pool, container_t##_block, container_t##_max_blocks);                     \
    static container_t##_block * __##container_t##_AllocateBlock(container_t * const self)                         \
    {                                                                                                              \
        return container_t##_block_pool_Allocate(&self->pool);                                                     \
    }                                                                                                              \
    static void __##container_t##_ReleaseBlock(container_t * const self, container_t##_block * block)              \
    {                                                                                                              \
        if(block)                                                                                                  \
        {                                                                                                          \
            container_t##_block_pool_Release(&self->pool, block);                                                  \
        }                                                                                                          \
    }                                                                                                              \
    static bool __##container_t##_CanGrowMap(container_t * const self)                                             \
    {                                                                                                              \
        (void)self;                                                                                                \
        return false;                                                                                              \
    }                                                                                                              \
    static bool __##container_t##_GrowMap(container_t * const self)                                                \
    {                                                                                                              \
        (void)self;                                                                                                \
        return false;                                                                                              \
    }                                                                                                              \
    void container_t##_Construct(container_t * const self)                                                         \
    {                                                                                                              \
        assert(self);                                                                                              \
                                                                                                                   \
        container_t##_block_pool_Construct(&self->pool);                                                           \
        self->map_capacity = container_t##_max_blocks;                                                             \
        self->size         = 0;                                                                                    \
        self->block_count  = 0;                                                                                    \
        self->head_block   = 0;                                                                                    \
        self->tail_block   = 0;                                                                                    \
        self->head_offset  = 0;                                                                                    \
        self->tail_offset  = 0;                                                                                    \
        self->max_size     = container_capacity;                                                                   \
        for(size_t i = 0; i < self->map_capacity; ++i)                                                             \
        {                                                                                                          \
            self->map[i] = NULL;                                                                                   \
        }                                                                                                          \
    }                                                                                                              \
    void container_t##_Destruct(container_t * const self)                                                          \
    {                                                                                                              \
        assert(self);                                                                                              \
                                                                                                                   \
        for(size_t i = 0; i < self->map_capacity; ++i)                                                             \
        {                                                                                                          \
            if(self->map[i])                                                                                       \
            {                                                                                                      \
                __##container_t##_ReleaseBlock(self, self->map[i]);                                                \
                self->map[i] = NULL;                                                                               \
            }                                                                                                      \
        }                                                                                                          \
        container_t##_block_pool_Destruct(&self->pool);                                                            \
    }                                                                                                              \
    __deque_core_methods_c(container_t, member_t, __##container_t##_AllocateBlock, __##container_t##_ReleaseBlock, \
                           __##container_t##_GrowMap, __##container_t##_CanGrowMap)

#define custom_allocator_deque(container_t, member_t, allocator_t)              \
    typedef struct container_t container_t;                                     \
    typedef struct container_t##_Iterator container_t##_Iterator;               \
    typedef struct container_t##_block container_t##_block;                     \
    typedef int (*container_t##_compare_t)(const member_t *, const member_t *); \
    struct container_t##_block                                                  \
    {                                                                           \
        member_t values[RTLIB_DEQUE_BLOCK_SIZE];                                \
    };                                                                          \
    struct container_t##_Iterator                                               \
    {                                                                           \
        size_t index;                                                           \
        const container_t * owner;                                              \
    };                                                                          \
    struct container_t                                                          \
    {                                                                           \
        container_t##_block ** map;                                             \
        size_t map_capacity;                                                    \
        size_t head_block;                                                      \
        size_t tail_block;                                                      \
        size_t head_offset;                                                     \
        size_t tail_offset;                                                     \
        size_t size;                                                            \
        size_t block_count;                                                     \
        size_t max_size;                                                        \
        allocator_t allocator;                                                  \
    };                                                                          \
    __deque_methods_h(container_t, member_t)

#define custom_allocator_deque_impl(container_t, member_t, allocator_t)                                            \
    static container_t##_block * __##container_t##_AllocateBlock(container_t * const self)                         \
    {                                                                                                              \
        return (container_t##_block *)allocator_t##_Allocate(&self->allocator, sizeof(container_t##_block));       \
    }                                                                                                              \
    static void __##container_t##_ReleaseBlock(container_t * const self, container_t##_block * block)              \
    {                                                                                                              \
        if(block)                                                                                                  \
        {                                                                                                          \
            allocator_t##_Deallocate(&self->allocator, block);                                                     \
        }                                                                                                          \
    }                                                                                                              \
    static bool __##container_t##_CanGrowMap(container_t * const self)                                             \
    {                                                                                                              \
        (void)self;                                                                                                \
        return true;                                                                                               \
    }                                                                                                              \
    static bool __##container_t##_GrowMap(container_t * const self)                                                \
    {                                                                                                              \
        const size_t new_capacity      = self->map_capacity == 0 ? 8 : self->map_capacity * 2;                     \
        container_t##_block ** new_map = (container_t##_block **)allocator_t##_Allocate(                           \
            &self->allocator, new_capacity * sizeof(container_t##_block *));                                       \
        if(!new_map)                                                                                               \
        {                                                                                                          \
            return false;                                                                                          \
        }                                                                                                          \
        for(size_t i = 0; i < new_capacity; ++i)                                                                   \
        {                                                                                                          \
            new_map[i] = NULL;                                                                                     \
        }                                                                                                          \
        if(self->block_count > 0)                                                                                  \
        {                                                                                                          \
            const size_t start = (new_capacity - self->block_count) / 2;                                           \
            for(size_t i = 0; i < self->block_count; ++i)                                                          \
            {                                                                                                      \
                const size_t old_index = (self->head_block + i) % self->map_capacity;                              \
                new_map[start + i]     = self->map[old_index];                                                     \
            }                                                                                                      \
            self->head_block = start;                                                                              \
            self->tail_block = start + self->block_count - 1;                                                      \
        }                                                                                                          \
        allocator_t##_Deallocate(&self->allocator, self->map);                                                     \
        self->map          = new_map;                                                                              \
        self->map_capacity = new_capacity;                                                                         \
        return true;                                                                                               \
    }                                                                                                              \
    void container_t##_Construct(container_t * const self)                                                         \
    {                                                                                                              \
        assert(self);                                                                                              \
                                                                                                                   \
        allocator_t##_Construct(&self->allocator);                                                                 \
        self->map_capacity = 0;                                                                                    \
        self->map          = NULL;                                                                                 \
        self->size         = 0;                                                                                    \
        self->block_count  = 0;                                                                                    \
        self->head_block   = 0;                                                                                    \
        self->tail_block   = 0;                                                                                    \
        self->head_offset  = 0;                                                                                    \
        self->tail_offset  = 0;                                                                                    \
        self->max_size     = (size_t)-1;                                                                           \
        __##container_t##_GrowMap(self);                                                                           \
    }                                                                                                              \
    void container_t##_Destruct(container_t * const self)                                                          \
    {                                                                                                              \
        assert(self);                                                                                              \
                                                                                                                   \
        for(size_t i = 0; i < self->map_capacity; ++i)                                                             \
        {                                                                                                          \
            if(self->map[i])                                                                                       \
            {                                                                                                      \
                __##container_t##_ReleaseBlock(self, self->map[i]);                                                \
                self->map[i] = NULL;                                                                               \
            }                                                                                                      \
        }                                                                                                          \
        allocator_t##_Deallocate(&self->allocator, self->map);                                                     \
        allocator_t##_Destruct(&self->allocator);                                                                  \
    }                                                                                                              \
    __deque_core_methods_c(container_t, member_t, __##container_t##_AllocateBlock, __##container_t##_ReleaseBlock, \
                           __##container_t##_GrowMap, __##container_t##_CanGrowMap)

#define dynamic_deque(container_t, member_t)        \
    dynamic_memory(container_t##_DynamicAllocator); \
    custom_allocator_deque(container_t, member_t, container_t##_DynamicAllocator)

#define dynamic_deque_impl(container_t, member_t)        \
    dynamic_memory_impl(container_t##_DynamicAllocator); \
    custom_allocator_deque_impl(container_t, member_t, container_t##_DynamicAllocator)

#ifdef __cplusplus
}
#endif