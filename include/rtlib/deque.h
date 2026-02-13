#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "error_codes.h"
#include "rtlib/memory.h"

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

#define __deque_common_methods_c(container_t, member_t)                                                        \
    static inline size_t __##container_t##_Next(size_t index, size_t capacity)                                 \
    {                                                                                                          \
        ++index;                                                                                               \
        return index == capacity ? 0 : index;                                                                  \
    }                                                                                                          \
                                                                                                               \
    static inline size_t __##container_t##_Prev(size_t index, size_t capacity)                                 \
    {                                                                                                          \
        return index == 0 ? (capacity - 1) : (index - 1);                                                      \
    }                                                                                                          \
                                                                                                               \
    static inline size_t __##container_t##_RawIndex(const container_t * const self, size_t logical_index)      \
    {                                                                                                          \
        const size_t raw = self->head + logical_index;                                                         \
        return raw >= self->capacity ? (raw - self->capacity) : raw;                                           \
    }                                                                                                          \
                                                                                                               \
    size_t container_t##_Size(const container_t * const self)                                                  \
    {                                                                                                          \
        assert(self);                                                                                          \
        return self->size;                                                                                     \
    }                                                                                                          \
                                                                                                               \
    bool container_t##_Empty(const container_t * const self)                                                   \
    {                                                                                                          \
        assert(self);                                                                                          \
        return self->size == 0;                                                                                \
    }                                                                                                          \
                                                                                                               \
    member_t * container_t##_Ref(container_t * const self, size_t index)                                       \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(index < self->size);                                                                            \
        return &self->data[__##container_t##_RawIndex(self, index)];                                           \
    }                                                                                                          \
                                                                                                               \
    const member_t * container_t##_CRef(const container_t * const self, size_t index)                          \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(index < self->size);                                                                            \
        return &self->data[__##container_t##_RawIndex(self, index)];                                           \
    }                                                                                                          \
                                                                                                               \
    member_t container_t##_Front(const container_t * const self)                                               \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(self->size > 0);                                                                                \
        return *container_t##_CRef(self, 0);                                                                   \
    }                                                                                                          \
                                                                                                               \
    member_t container_t##_Back(const container_t * const self)                                                \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(self->size > 0);                                                                                \
        return *container_t##_CRef(self, self->size - 1);                                                      \
    }                                                                                                          \
                                                                                                               \
    container_t##_Iterator container_t##_Begin(const container_t * const self)                                 \
    {                                                                                                          \
        container_t##_Iterator it = { 0, self };                                                               \
        return it;                                                                                             \
    }                                                                                                          \
                                                                                                               \
    container_t##_Iterator container_t##_End(const container_t * const self)                                   \
    {                                                                                                          \
        container_t##_Iterator it = { self->size, self };                                                      \
        return it;                                                                                             \
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
        container_t##_Iterator it  = container_t##_Begin(self);                                                \
        container_t##_Iterator end = container_t##_End(self);                                                  \
        for(; !container_t##_Iterator_Equal(&it, &end); container_t##_Iterator_Increment(&it))                 \
        {                                                                                                      \
            const member_t value = *container_t##_Iterator_CRef(&it);                                          \
            if(member_t##_Compare(&data, &value) == 0)                                                         \
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
        self->size = 0;                                                                                        \
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
                                                                                                               \
        for(size_t i = iterator->index; i < self->size - 1; ++i)                                               \
        {                                                                                                      \
            *container_t##_Ref(self, i) = *container_t##_CRef(self, i + 1);                                    \
        }                                                                                                      \
        return container_t##_PopBack(self);                                                                    \
    }

#define __static_deque_methods_c(container_t, member_t, container_capacity)          \
    __deque_common_methods_c(container_t, member_t)                                  \
                                                                                     \
        void                                                                         \
        container_t##_Construct(container_t * const self)                            \
    {                                                                                \
        assert(self);                                                                \
        self->data     = self->buffer;                                               \
        self->capacity = container_capacity;                                         \
        self->head     = container_capacity / 2;                                     \
        self->size     = 0;                                                          \
        self->max_size = container_capacity;                                         \
    }                                                                                \
                                                                                     \
    void container_t##_Destruct(container_t * const self)                            \
    {                                                                                \
        assert(self);                                                                \
    }                                                                                \
                                                                                     \
    int container_t##_PushBack(container_t * const self, member_t data)              \
    {                                                                                \
        assert(self);                                                                \
        if(self->size >= self->max_size)                                             \
        {                                                                            \
            return ALLOCATION_ERROR;                                                 \
        }                                                                            \
        const size_t tail = __##container_t##_RawIndex(self, self->size);            \
        self->data[tail]  = data;                                                    \
        self->size++;                                                                \
        return (int)self->size;                                                      \
    }                                                                                \
                                                                                     \
    int container_t##_PushFront(container_t * const self, member_t data)             \
    {                                                                                \
        assert(self);                                                                \
        if(self->size >= self->max_size)                                             \
        {                                                                            \
            return ALLOCATION_ERROR;                                                 \
        }                                                                            \
        self->head             = __##container_t##_Prev(self->head, self->capacity); \
        self->data[self->head] = data;                                               \
        self->size++;                                                                \
        return (int)self->size;                                                      \
    }                                                                                \
                                                                                     \
    int container_t##_PopBack(container_t * const self)                              \
    {                                                                                \
        assert(self);                                                                \
        assert(self->size > 0);                                                      \
        self->size--;                                                                \
        return (int)self->size;                                                      \
    }                                                                                \
                                                                                     \
    int container_t##_PopFront(container_t * const self)                             \
    {                                                                                \
        assert(self);                                                                \
        assert(self->size > 0);                                                      \
        self->head = __##container_t##_Next(self->head, self->capacity);             \
        self->size--;                                                                \
        return (int)self->size;                                                      \
    }

#define __custom_allocator_deque_methods_c(container_t, member_t, allocator_t)                                       \
    __deque_common_methods_c(container_t, member_t)                                                                  \
                                                                                                                     \
        static bool                                                                                                  \
        __##container_t##_EnsureCapacity(container_t * const self)                                                   \
    {                                                                                                                \
        if(self->size < self->capacity)                                                                              \
        {                                                                                                            \
            return true;                                                                                             \
        }                                                                                                            \
                                                                                                                     \
        size_t new_capacity = self->capacity == 0 ? 8 : self->capacity * 2;                                          \
        if(self->max_size != (size_t)-1 && new_capacity > self->max_size)                                            \
        {                                                                                                            \
            new_capacity = self->max_size;                                                                           \
        }                                                                                                            \
        if(new_capacity <= self->capacity)                                                                           \
        {                                                                                                            \
            return false;                                                                                            \
        }                                                                                                            \
                                                                                                                     \
        member_t * new_data = (member_t *)allocator_t##_Allocate(&self->allocator, sizeof(member_t) * new_capacity); \
        if(!new_data)                                                                                                \
        {                                                                                                            \
            return false;                                                                                            \
        }                                                                                                            \
                                                                                                                     \
        if(self->data)                                                                                               \
        {                                                                                                            \
            for(size_t i = 0; i < self->size; ++i)                                                                   \
            {                                                                                                        \
                const size_t src =                                                                                   \
                    self->head + i >= self->capacity ? (self->head + i - self->capacity) : (self->head + i);         \
                new_data[i] = self->data[src];                                                                       \
            }                                                                                                        \
            allocator_t##_Deallocate(&self->allocator, self->data);                                                  \
        }                                                                                                            \
                                                                                                                     \
        self->data     = new_data;                                                                                   \
        self->capacity = new_capacity;                                                                               \
        self->head     = 0;                                                                                          \
        return true;                                                                                                 \
    }                                                                                                                \
                                                                                                                     \
    void container_t##_Construct(container_t * const self)                                                           \
    {                                                                                                                \
        assert(self);                                                                                                \
        allocator_t##_Construct(&self->allocator);                                                                   \
        self->data     = NULL;                                                                                       \
        self->capacity = 0;                                                                                          \
        self->head     = 4;                                                                                          \
        self->size     = 0;                                                                                          \
        self->max_size = (size_t)-1;                                                                                 \
    }                                                                                                                \
                                                                                                                     \
    void container_t##_Destruct(container_t * const self)                                                            \
    {                                                                                                                \
        assert(self);                                                                                                \
        allocator_t##_Deallocate(&self->allocator, self->data);                                                      \
        self->data = NULL;                                                                                           \
        allocator_t##_Destruct(&self->allocator);                                                                    \
    }                                                                                                                \
                                                                                                                     \
    int container_t##_PushBack(container_t * const self, member_t data)                                              \
    {                                                                                                                \
        assert(self);                                                                                                \
        if(self->size >= self->max_size)                                                                             \
        {                                                                                                            \
            return ALLOCATION_ERROR;                                                                                 \
        }                                                                                                            \
        if(!__##container_t##_EnsureCapacity(self))                                                                  \
        {                                                                                                            \
            return ALLOCATION_ERROR;                                                                                 \
        }                                                                                                            \
        const size_t tail = __##container_t##_RawIndex(self, self->size);                                            \
        self->data[tail]  = data;                                                                                    \
        self->size++;                                                                                                \
        return (int)self->size;                                                                                      \
    }                                                                                                                \
                                                                                                                     \
    int container_t##_PushFront(container_t * const self, member_t data)                                             \
    {                                                                                                                \
        assert(self);                                                                                                \
        if(self->size >= self->max_size)                                                                             \
        {                                                                                                            \
            return ALLOCATION_ERROR;                                                                                 \
        }                                                                                                            \
        if(!__##container_t##_EnsureCapacity(self))                                                                  \
        {                                                                                                            \
            return ALLOCATION_ERROR;                                                                                 \
        }                                                                                                            \
        self->head             = __##container_t##_Prev(self->head, self->capacity);                                 \
        self->data[self->head] = data;                                                                               \
        self->size++;                                                                                                \
        return (int)self->size;                                                                                      \
    }                                                                                                                \
                                                                                                                     \
    int container_t##_PopBack(container_t * const self)                                                              \
    {                                                                                                                \
        assert(self);                                                                                                \
        assert(self->size > 0);                                                                                      \
        self->size--;                                                                                                \
        return (int)self->size;                                                                                      \
    }                                                                                                                \
                                                                                                                     \
    int container_t##_PopFront(container_t * const self)                                                             \
    {                                                                                                                \
        assert(self);                                                                                                \
        assert(self->size > 0);                                                                                      \
        self->head = __##container_t##_Next(self->head, self->capacity);                                             \
        self->size--;                                                                                                \
        return (int)self->size;                                                                                      \
    }

#define static_deque(container_t, member_t, container_capacity)                 \
    typedef struct container_t container_t;                                     \
    typedef struct container_t##_Iterator container_t##_Iterator;               \
    typedef int (*container_t##_compare_t)(const member_t *, const member_t *); \
    struct container_t##_Iterator                                               \
    {                                                                           \
        size_t index;                                                           \
        const container_t * owner;                                              \
    };                                                                          \
    struct container_t                                                          \
    {                                                                           \
        member_t * data;                                                        \
        size_t capacity;                                                        \
        size_t head;                                                            \
        size_t size;                                                            \
        size_t max_size;                                                        \
        member_t buffer[container_capacity];                                    \
    };                                                                          \
    __deque_methods_h(container_t, member_t)

#define static_deque_impl(container_t, member_t, container_capacity) \
    __static_deque_methods_c(container_t, member_t, container_capacity)

#define custom_allocator_deque(container_t, member_t, allocator_t)              \
    typedef struct container_t container_t;                                     \
    typedef struct container_t##_Iterator container_t##_Iterator;               \
    typedef int (*container_t##_compare_t)(const member_t *, const member_t *); \
    struct container_t##_Iterator                                               \
    {                                                                           \
        size_t index;                                                           \
        const container_t * owner;                                              \
    };                                                                          \
    struct container_t                                                          \
    {                                                                           \
        member_t * data;                                                        \
        size_t capacity;                                                        \
        size_t head;                                                            \
        size_t size;                                                            \
        size_t max_size;                                                        \
        allocator_t allocator;                                                  \
    };                                                                          \
    __deque_methods_h(container_t, member_t)

#define custom_allocator_deque_impl(container_t, member_t, allocator_t) \
    __custom_allocator_deque_methods_c(container_t, member_t, allocator_t)

#define dynamic_deque(container_t, member_t)        \
    dynamic_memory(container_t##_DynamicAllocator); \
    custom_allocator_deque(container_t, member_t, container_t##_DynamicAllocator)

#define dynamic_deque_impl(container_t, member_t)        \
    dynamic_memory_impl(container_t##_DynamicAllocator); \
    custom_allocator_deque_impl(container_t, member_t, container_t##_DynamicAllocator)

#ifdef __cplusplus
}
#endif
