#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <cstddef>
#define RTLIB_MAX_ALIGN_T std::max_align_t
#else
#define RTLIB_MAX_ALIGN_T max_align_t
#endif

#if defined(__cplusplus)
#define RTLIB_ALIGNAS(x) alignas(x)
#else
#define RTLIB_ALIGNAS(x) _Alignas(x)
#endif

#define __heap_methods_h(container_t)                                                    \
    void container_t##_Construct(container_t * const self);                              \
    void container_t##_Destruct(container_t * const self);                               \
    void * container_t##_Allocate(container_t * const self, size_t size);                \
    void * container_t##_Callocate(container_t * const self, size_t count, size_t size); \
    void * container_t##_Reallocate(container_t * const self, void * object, size_t);    \
    void container_t##_Deallocate(container_t * const self, void * object);

#define __heap_helpers_c(container_t)                      \
    static size_t __##container_t##_AlignSize(size_t size) \
    {                                                      \
        size_t align = sizeof(RTLIB_MAX_ALIGN_T);          \
        return (size + align - 1) & ~(align - 1);          \
    }

#define __static_heap_methods_c(container_t, heap_capacity)                                                     \
    __heap_helpers_c(container_t)                                                                               \
                                                                                                                \
        void                                                                                                    \
        container_t##_Construct(container_t * const self)                                                       \
    {                                                                                                           \
        assert(self);                                                                                           \
        assert(heap_capacity > sizeof(container_t##_block));                                                    \
                                                                                                                \
        self->capacity   = heap_capacity;                                                                       \
        self->head       = (container_t##_block *)self->buffer;                                                 \
        self->head->size = heap_capacity - sizeof(container_t##_block);                                         \
        self->head->free = true;                                                                                \
        self->head->next = NULL;                                                                                \
        self->head->prev = NULL;                                                                                \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Destruct(container_t * const self)                                                       \
    {                                                                                                           \
        assert(self);                                                                                           \
    }                                                                                                           \
                                                                                                                \
    void * container_t##_Allocate(container_t * const self, size_t size)                                        \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        if(size == 0)                                                                                           \
        {                                                                                                       \
            return NULL;                                                                                        \
        }                                                                                                       \
                                                                                                                \
        size                          = __##container_t##_AlignSize(size);                                      \
        container_t##_block * current = self->head;                                                             \
        while(current)                                                                                          \
        {                                                                                                       \
            if(current->free && current->size >= size)                                                          \
            {                                                                                                   \
                size_t remaining = current->size - size;                                                        \
                if(remaining > sizeof(container_t##_block) + sizeof(RTLIB_MAX_ALIGN_T))                         \
                {                                                                                               \
                    container_t##_block * next =                                                                \
                        (container_t##_block *)((unsigned char *)current + sizeof(container_t##_block) + size); \
                    next->size = remaining - sizeof(container_t##_block);                                       \
                    next->free = true;                                                                          \
                    next->next = current->next;                                                                 \
                    next->prev = current;                                                                       \
                    if(current->next)                                                                           \
                    {                                                                                           \
                        current->next->prev = next;                                                             \
                    }                                                                                           \
                    current->next = next;                                                                       \
                    current->size = size;                                                                       \
                }                                                                                               \
                current->free = false;                                                                          \
                return (void *)(current + 1);                                                                   \
            }                                                                                                   \
            current = current->next;                                                                            \
        }                                                                                                       \
        return NULL;                                                                                            \
    }                                                                                                           \
                                                                                                                \
    void * container_t##_Callocate(container_t * const self, size_t count, size_t size)                         \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        if(size != 0 && count > (SIZE_MAX / size))                                                              \
        {                                                                                                       \
            return NULL;                                                                                        \
        }                                                                                                       \
                                                                                                                \
        size_t total = count * size;                                                                            \
        void * ptr   = container_t##_Allocate(self, total);                                                     \
        if(ptr)                                                                                                 \
        {                                                                                                       \
            memset(ptr, 0, total);                                                                              \
        }                                                                                                       \
        return ptr;                                                                                             \
    }                                                                                                           \
                                                                                                                \
    void * container_t##_Reallocate(container_t * const self, void * object, size_t new_size)                   \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        if(!object)                                                                                             \
        {                                                                                                       \
            return container_t##_Allocate(self, new_size);                                                      \
        }                                                                                                       \
        if(new_size == 0)                                                                                       \
        {                                                                                                       \
            container_t##_Deallocate(self, object);                                                             \
            return NULL;                                                                                        \
        }                                                                                                       \
                                                                                                                \
        new_size                    = __##container_t##_AlignSize(new_size);                                    \
        container_t##_block * block = ((container_t##_block *)object) - 1;                                      \
        if(block->size >= new_size)                                                                             \
        {                                                                                                       \
            size_t remaining = block->size - new_size;                                                          \
            if(remaining > sizeof(container_t##_block) + sizeof(RTLIB_MAX_ALIGN_T))                             \
            {                                                                                                   \
                container_t##_block * next =                                                                    \
                    (container_t##_block *)((unsigned char *)block + sizeof(container_t##_block) + new_size);   \
                next->size = remaining - sizeof(container_t##_block);                                           \
                next->free = true;                                                                              \
                next->next = block->next;                                                                       \
                next->prev = block;                                                                             \
                if(block->next)                                                                                 \
                {                                                                                               \
                    block->next->prev = next;                                                                   \
                }                                                                                               \
                block->next = next;                                                                             \
                block->size = new_size;                                                                         \
            }                                                                                                   \
            return object;                                                                                      \
        }                                                                                                       \
                                                                                                                \
        if(block->next && block->next->free &&                                                                  \
           block->size + sizeof(container_t##_block) + block->next->size >= new_size)                           \
        {                                                                                                       \
            container_t##_block * next = block->next;                                                           \
            block->size += sizeof(container_t##_block) + next->size;                                            \
            block->next = next->next;                                                                           \
            if(next->next)                                                                                      \
            {                                                                                                   \
                next->next->prev = block;                                                                       \
            }                                                                                                   \
            return container_t##_Reallocate(self, object, new_size);                                            \
        }                                                                                                       \
                                                                                                                \
        void * new_ptr = container_t##_Allocate(self, new_size);                                                \
        if(!new_ptr)                                                                                            \
        {                                                                                                       \
            return NULL;                                                                                        \
        }                                                                                                       \
        memcpy(new_ptr, object, block->size);                                                                   \
        container_t##_Deallocate(self, object);                                                                 \
        return new_ptr;                                                                                         \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Deallocate(container_t * const self, void * object)                                      \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        if(!object)                                                                                             \
        {                                                                                                       \
            return;                                                                                             \
        }                                                                                                       \
                                                                                                                \
        container_t##_block * block = ((container_t##_block *)object) - 1;                                      \
        block->free                 = true;                                                                     \
                                                                                                                \
        if(block->next && block->next->free)                                                                    \
        {                                                                                                       \
            container_t##_block * next = block->next;                                                           \
            block->size += sizeof(container_t##_block) + next->size;                                            \
            block->next = next->next;                                                                           \
            if(next->next)                                                                                      \
            {                                                                                                   \
                next->next->prev = block;                                                                       \
            }                                                                                                   \
        }                                                                                                       \
        if(block->prev && block->prev->free)                                                                    \
        {                                                                                                       \
            container_t##_block * prev = block->prev;                                                           \
            prev->size += sizeof(container_t##_block) + block->size;                                            \
            prev->next = block->next;                                                                           \
            if(block->next)                                                                                     \
            {                                                                                                   \
                block->next->prev = prev;                                                                       \
            }                                                                                                   \
        }                                                                                                       \
    }

#define __dynamic_heap_methods_c(container_t, heap_capacity)                                                         \
    __heap_helpers_c(container_t)                                                                                    \
                                                                                                                     \
        void                                                                                                         \
        container_t##_Construct(container_t * const self)                                                            \
    {                                                                                                                \
        assert(self);                                                                                                \
                                                                                                                     \
        self->capacity = heap_capacity;                                                                              \
        self->used     = 0;                                                                                          \
    }                                                                                                                \
                                                                                                                     \
    void container_t##_Destruct(container_t * const self)                                                            \
    {                                                                                                                \
        assert(self);                                                                                                \
    }                                                                                                                \
                                                                                                                     \
    void * container_t##_Allocate(container_t * const self, size_t size)                                             \
    {                                                                                                                \
        assert(self);                                                                                                \
                                                                                                                     \
        if(size == 0)                                                                                                \
        {                                                                                                            \
            return NULL;                                                                                             \
        }                                                                                                            \
                                                                                                                     \
        size = __##container_t##_AlignSize(size);                                                                    \
        if(self->capacity > 0 && self->used + size > self->capacity)                                                 \
        {                                                                                                            \
            return NULL;                                                                                             \
        }                                                                                                            \
                                                                                                                     \
        container_t##_dyn_block * block = (container_t##_dyn_block *)malloc(sizeof(container_t##_dyn_block) + size); \
        if(!block)                                                                                                   \
        {                                                                                                            \
            return NULL;                                                                                             \
        }                                                                                                            \
        block->size = size;                                                                                          \
        self->used += size;                                                                                          \
        return (void *)(block + 1);                                                                                  \
    }                                                                                                                \
                                                                                                                     \
    void * container_t##_Callocate(container_t * const self, size_t count, size_t size)                              \
    {                                                                                                                \
        assert(self);                                                                                                \
                                                                                                                     \
        if(size != 0 && count > (SIZE_MAX / size))                                                                   \
        {                                                                                                            \
            return NULL;                                                                                             \
        }                                                                                                            \
                                                                                                                     \
        size_t total = count * size;                                                                                 \
        void * ptr   = container_t##_Allocate(self, total);                                                          \
        if(ptr)                                                                                                      \
        {                                                                                                            \
            memset(ptr, 0, total);                                                                                   \
        }                                                                                                            \
        return ptr;                                                                                                  \
    }                                                                                                                \
                                                                                                                     \
    void * container_t##_Reallocate(container_t * const self, void * object, size_t new_size)                        \
    {                                                                                                                \
        assert(self);                                                                                                \
                                                                                                                     \
        if(!object)                                                                                                  \
        {                                                                                                            \
            return container_t##_Allocate(self, new_size);                                                           \
        }                                                                                                            \
        if(new_size == 0)                                                                                            \
        {                                                                                                            \
            container_t##_Deallocate(self, object);                                                                  \
            return NULL;                                                                                             \
        }                                                                                                            \
                                                                                                                     \
        new_size                        = __##container_t##_AlignSize(new_size);                                     \
        container_t##_dyn_block * block = ((container_t##_dyn_block *)object) - 1;                                   \
        size_t old_size                 = block->size;                                                               \
                                                                                                                     \
        if(self->capacity > 0 && self->used - old_size + new_size > self->capacity)                                  \
        {                                                                                                            \
            return NULL;                                                                                             \
        }                                                                                                            \
                                                                                                                     \
        container_t##_dyn_block * resized =                                                                          \
            (container_t##_dyn_block *)realloc(block, sizeof(container_t##_dyn_block) + new_size);                   \
        if(!resized)                                                                                                 \
        {                                                                                                            \
            return NULL;                                                                                             \
        }                                                                                                            \
        resized->size = new_size;                                                                                    \
        self->used    = self->used - old_size + new_size;                                                            \
        return (void *)(resized + 1);                                                                                \
    }                                                                                                                \
                                                                                                                     \
    void container_t##_Deallocate(container_t * const self, void * object)                                           \
    {                                                                                                                \
        assert(self);                                                                                                \
                                                                                                                     \
        if(!object)                                                                                                  \
        {                                                                                                            \
            return;                                                                                                  \
        }                                                                                                            \
                                                                                                                     \
        container_t##_dyn_block * block = ((container_t##_dyn_block *)object) - 1;                                   \
        self->used -= block->size;                                                                                   \
        free(block);                                                                                                 \
    }

#define static_heap(container_t, heap_capacity)                                                         \
    typedef struct container_t container_t;                                                             \
    typedef struct container_t##_block container_t##_block;                                             \
    struct container_t##_block                                                                          \
    {                                                                                                   \
        size_t size;                                                                                    \
        bool free;                                                                                      \
        container_t##_block * next;                                                                     \
        container_t##_block * prev;                                                                     \
    };                                                                                                  \
    enum                                                                                                \
    {                                                                                                   \
        container_t##_heap_capacity_check = 1 / ((heap_capacity > sizeof(container_t##_block)) ? 1 : 0) \
    };                                                                                                  \
    struct container_t                                                                                  \
    {                                                                                                   \
        size_t capacity;                                                                                \
        container_t##_block * head;                                                                     \
        RTLIB_ALIGNAS(RTLIB_MAX_ALIGN_T) unsigned char buffer[heap_capacity];                           \
    };                                                                                                  \
    __heap_methods_h(container_t)

#define static_heap_impl(container_t, heap_capacity) __static_heap_methods_c(container_t, heap_capacity)

#define dynamic_heap(container_t, heap_capacity)                    \
    typedef struct container_t container_t;                         \
    typedef struct container_t##_dyn_block container_t##_dyn_block; \
    struct container_t##_dyn_block                                  \
    {                                                               \
        size_t size;                                                \
    };                                                              \
    struct container_t                                              \
    {                                                               \
        size_t capacity;                                            \
        size_t used;                                                \
    };                                                              \
    __heap_methods_h(container_t)

#define dynamic_heap_impl(container_t, heap_capacity) __dynamic_heap_methods_c(container_t, heap_capacity)

#ifdef __cplusplus
}
#endif
