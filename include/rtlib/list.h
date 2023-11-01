#pragma once

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error_codes.h"
#include "rtlib/pool.h"
#include "rtlib/memory.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define __list_methods_h(container_t, member_t)                                                                 \
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
    void container_t##_Clear(container_t * const self);                                                         \
    member_t container_t##_Front(const container_t * const self);                                               \
    member_t container_t##_Back(const container_t * const self);                                                \
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

#define __static_list_methods_c(container_t, member_t, container_capacity)                                      \
    void container_t##_Construct(container_t * const self)                                                      \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_pool_Construct(&self->pool);                                                              \
        self->end = container_t##_pool_Allocate(&self->pool);                                                   \
        assert(self->end);                                                                                      \
        self->begin       = self->end;                                                                          \
        self->begin->prev = NULL;                                                                               \
        self->begin->next = NULL;                                                                               \
        self->size        = 0;                                                                                  \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Destruct(container_t * const self)                                                       \
    {}                                                                                                          \
                                                                                                                \
    size_t container_t##_Size(const container_t * const self)                                                   \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        return self->size;                                                                                      \
    }                                                                                                           \
                                                                                                                \
    bool container_t##_Empty(const container_t * const self)                                                    \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        return self->size == 0;                                                                                 \
    }                                                                                                           \
                                                                                                                \
    int container_t##_PushBack(container_t * const self, member_t value)                                        \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_node * node = container_t##_pool_Allocate(&self->pool);                                   \
        if(node)                                                                                                \
        {                                                                                                       \
            container_t##_node * old_end_node = self->end->prev;                                                \
            if(old_end_node)                                                                                    \
            {                                                                                                   \
                old_end_node->next = node;                                                                      \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                self->begin = node;                                                                             \
            }                                                                                                   \
            node->prev      = old_end_node;                                                                     \
            node->next      = self->end;                                                                        \
            self->end->prev = node;                                                                             \
            node->value     = value;                                                                            \
                                                                                                                \
            ++self->size;                                                                                       \
                                                                                                                \
            return self->size;                                                                                  \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            return ALLOCATION_ERROR;                                                                            \
        }                                                                                                       \
    }                                                                                                           \
                                                                                                                \
    int container_t##_PopBack(container_t * const self)                                                         \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_node * old_end_node = self->end->prev;                                                    \
        container_t##_node * new_end_node = old_end_node->prev;                                                 \
                                                                                                                \
        container_t##_pool_Release(&self->pool, old_end_node);                                                  \
                                                                                                                \
        if(new_end_node == NULL)                                                                                \
        {                                                                                                       \
            self->begin       = self->end;                                                                      \
            self->begin->next = NULL;                                                                           \
            self->begin->prev = NULL;                                                                           \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            new_end_node->next = self->end;                                                                     \
            self->end->prev    = new_end_node;                                                                  \
        }                                                                                                       \
                                                                                                                \
        --self->size;                                                                                           \
                                                                                                                \
        return self->size;                                                                                      \
    }                                                                                                           \
                                                                                                                \
    int container_t##_PushFront(container_t * const self, member_t value)                                       \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_node * node = container_t##_pool_Allocate(&self->pool);                                   \
        if(node)                                                                                                \
        {                                                                                                       \
            container_t##_node * old_begin_node = self->begin;                                                  \
            old_begin_node->prev                = node;                                                         \
            node->next                          = old_begin_node;                                               \
            node->prev                          = NULL;                                                         \
            node->value                         = value;                                                        \
            self->begin                         = node;                                                         \
                                                                                                                \
            ++self->size;                                                                                       \
                                                                                                                \
            return self->size;                                                                                  \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            return ALLOCATION_ERROR;                                                                            \
        }                                                                                                       \
    }                                                                                                           \
                                                                                                                \
    int container_t##_PopFront(container_t * const self)                                                        \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_node * old_begin_node = self->begin;                                                      \
        container_t##_node * new_begin_node = old_begin_node->next;                                             \
                                                                                                                \
        container_t##_pool_Release(&self->pool, old_begin_node);                                                \
                                                                                                                \
        new_begin_node->prev = NULL;                                                                            \
        self->begin          = new_begin_node;                                                                  \
                                                                                                                \
        --self->size;                                                                                           \
                                                                                                                \
        return self->size;                                                                                      \
    }                                                                                                           \
                                                                                                                \
    int container_t##_Insert(container_t * const self, container_t##_Iterator * const iterator, member_t value) \
    {                                                                                                           \
        assert(self);                                                                                           \
        assert(iterator);                                                                                       \
                                                                                                                \
        container_t##_node * node = container_t##_pool_Allocate(&self->pool);                                   \
        if(node)                                                                                                \
        {                                                                                                       \
            container_t##_node * next_Iterator = iterator->node;                                                \
            container_t##_node * prev_Iterator = NULL;                                                          \
            if(iterator->node != self->begin)                                                                   \
            {                                                                                                   \
                prev_Iterator       = next_Iterator->prev;                                                      \
                prev_Iterator->next = node;                                                                     \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                self->begin = node;                                                                             \
            }                                                                                                   \
                                                                                                                \
            next_Iterator->prev = node;                                                                         \
            node->next          = next_Iterator;                                                                \
            node->prev          = prev_Iterator;                                                                \
            node->value         = value;                                                                        \
                                                                                                                \
            ++self->size;                                                                                       \
                                                                                                                \
            return self->size;                                                                                  \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            return ALLOCATION_ERROR;                                                                            \
        }                                                                                                       \
    }                                                                                                           \
                                                                                                                \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator)                  \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_node * to_delete_node = iterator->node;                                                   \
        container_t##_node * next_node      = to_delete_node->next;                                             \
        container_t##_node * prev_node      = to_delete_node->prev;                                             \
                                                                                                                \
        container_t##_pool_Release(&self->pool, to_delete_node);                                                \
                                                                                                                \
        if(!prev_node)                                                                                          \
        {                                                                                                       \
            self->begin = next_node;                                                                            \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            prev_node->next = next_node;                                                                        \
        }                                                                                                       \
        next_node->prev = prev_node;                                                                            \
                                                                                                                \
        --self->size;                                                                                           \
                                                                                                                \
        return self->size;                                                                                      \
    }                                                                                                           \
                                                                                                                \
    member_t container_t##_Front(const container_t * const self)                                                \
    {                                                                                                           \
        assert(self);                                                                                           \
        assert(self->size > 0);                                                                                 \
                                                                                                                \
        return self->begin->value;                                                                              \
    }                                                                                                           \
                                                                                                                \
    member_t container_t##_Back(const container_t * const self)                                                 \
    {                                                                                                           \
        assert(self);                                                                                           \
        assert(self->size > 0);                                                                                 \
                                                                                                                \
        return self->end->prev->value;                                                                          \
    }                                                                                                           \
                                                                                                                \
    member_t * container_t##_Ref(container_t * const self, size_t index)                                        \
    {                                                                                                           \
        container_t##_Iterator it = container_t##_Begin(self);                                                  \
                                                                                                                \
        while(index--)                                                                                          \
        {                                                                                                       \
            container_t##_Iterator_Increment(&it);                                                              \
        }                                                                                                       \
        member_t * value = &it.node->value;                                                                     \
        return value;                                                                                           \
    }                                                                                                           \
                                                                                                                \
    const member_t * container_t##_CRef(const container_t * const self, size_t index)                           \
    {                                                                                                           \
        container_t##_Iterator it = container_t##_Begin(self);                                                  \
                                                                                                                \
        while(index--)                                                                                          \
        {                                                                                                       \
            container_t##_Iterator_Increment(&it);                                                              \
        }                                                                                                       \
        const member_t * value = (const member_t *)&it.node->value;                                             \
        return value;                                                                                           \
    }                                                                                                           \
                                                                                                                \
    container_t##_Iterator container_t##_Begin(const container_t * const self)                                  \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_Iterator it;                                                                              \
        it.node = self->begin;                                                                                  \
        return it;                                                                                              \
    }                                                                                                           \
                                                                                                                \
    container_t##_Iterator container_t##_End(const container_t * const self)                                    \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_Iterator it;                                                                              \
        it.node = self->end;                                                                                    \
        return it;                                                                                              \
    }                                                                                                           \
                                                                                                                \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                               \
                                      const container_t##_Iterator * const second)                              \
    {                                                                                                           \
        const bool is_equal = first->node == second->node;                                                      \
        return is_equal;                                                                                        \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self)                                  \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        self->node = self->node->next;                                                                          \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self)                                  \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        self->node = self->node->prev;                                                                          \
    }                                                                                                           \
    member_t * container_t##_Iterator_Ref(container_t##_Iterator * const self)                                  \
    {                                                                                                           \
        assert(self);                                                                                           \
        return &self->node->value;                                                                              \
    }                                                                                                           \
                                                                                                                \
    const member_t * container_t##_Iterator_CRef(const container_t##_Iterator * const self)                     \
    {                                                                                                           \
        assert(self);                                                                                           \
        return &self->node->value;                                                                              \
    }                                                                                                           \
                                                                                                                \
    container_t##_Iterator container_t##_Find(container_t * const self, const member_t data)                    \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_Iterator end = container_t##_End(self);                                                   \
        container_t##_Iterator it  = container_t##_Begin(self);                                                 \
                                                                                                                \
        for(; !container_t##_Iterator_Equal(&it, &end); container_t##_Iterator_Increment(&it))                  \
        {                                                                                                       \
            const member_t it_value = *container_t##_Iterator_CRef(&it);                                        \
            if(member_t##_Compare(&data, &it_value) == 0)                                                       \
            {                                                                                                   \
                break;                                                                                          \
            }                                                                                                   \
        }                                                                                                       \
        return it;                                                                                              \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Clear(container_t * const self)                                                          \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        while(container_t##_Size(self) != 0)                                                                    \
        {                                                                                                       \
            container_t##_Iterator begin = container_t##_Begin(self);                                           \
            container_t##_Erase(self, &begin);                                                                  \
        }                                                                                                       \
    }

#define __custom_allocator_list_methods_c(container_t, member_t, allocator_t)                                   \
                                                                                                                \
    void container_t##_Construct(container_t * const self)                                                      \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        allocator_t##_Construct(&self->allocator);                                                              \
        self->end = (container_t##_node *)allocator_t##_Allocate(&self->allocator, sizeof(container_t##_node)); \
        assert(self->end);                                                                                      \
        self->begin       = self->end;                                                                          \
        self->begin->prev = NULL;                                                                               \
        self->begin->next = NULL;                                                                               \
        self->size        = 0;                                                                                  \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Destruct(container_t * const self)                                                       \
    {                                                                                                           \
        while(container_t##_Size(self) != 0)                                                                    \
        {                                                                                                       \
            container_t##_PopBack(self);                                                                        \
        }                                                                                                       \
        allocator_t##_Deallocate(&self->allocator, self->end);                                                  \
    }                                                                                                           \
                                                                                                                \
    size_t container_t##_Size(const container_t * const self)                                                   \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        return self->size;                                                                                      \
    }                                                                                                           \
                                                                                                                \
    bool container_t##_Empty(const container_t * const self)                                                    \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        return self->size == 0;                                                                                 \
    }                                                                                                           \
                                                                                                                \
    int container_t##_PushBack(container_t * const self, member_t value)                                        \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_node * node =                                                                             \
            (container_t##_node *)allocator_t##_Allocate(&self->allocator, sizeof(container_t##_node));         \
        if(node)                                                                                                \
        {                                                                                                       \
            container_t##_node * old_end_node = self->end->prev;                                                \
            if(old_end_node)                                                                                    \
            {                                                                                                   \
                old_end_node->next = node;                                                                      \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                self->begin = node;                                                                             \
            }                                                                                                   \
            node->prev      = old_end_node;                                                                     \
            node->next      = self->end;                                                                        \
            self->end->prev = node;                                                                             \
            node->value     = value;                                                                            \
                                                                                                                \
            ++self->size;                                                                                       \
                                                                                                                \
            return self->size;                                                                                  \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            return ALLOCATION_ERROR;                                                                            \
        }                                                                                                       \
    }                                                                                                           \
                                                                                                                \
    int container_t##_PopBack(container_t * const self)                                                         \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_node * old_end_node = self->end->prev;                                                    \
        container_t##_node * new_end_node = old_end_node->prev;                                                 \
                                                                                                                \
        allocator_t##_Deallocate(&self->allocator, old_end_node);                                               \
                                                                                                                \
        if(new_end_node == NULL)                                                                                \
        {                                                                                                       \
            self->begin       = self->end;                                                                      \
            self->begin->next = NULL;                                                                           \
            self->begin->prev = NULL;                                                                           \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            new_end_node->next = self->end;                                                                     \
            self->end->prev    = new_end_node;                                                                  \
        }                                                                                                       \
                                                                                                                \
        --self->size;                                                                                           \
                                                                                                                \
        return self->size;                                                                                      \
    }                                                                                                           \
                                                                                                                \
    int container_t##_PushFront(container_t * const self, member_t value)                                       \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_node * node =                                                                             \
            (container_t##_node *)allocator_t##_Allocate(&self->allocator, sizeof(container_t##_node));         \
        if(node)                                                                                                \
        {                                                                                                       \
            container_t##_node * old_begin_node = self->begin;                                                  \
            old_begin_node->prev                = node;                                                         \
            node->next                          = old_begin_node;                                               \
            node->prev                          = NULL;                                                         \
            node->value                         = value;                                                        \
            self->begin                         = node;                                                         \
                                                                                                                \
            ++self->size;                                                                                       \
                                                                                                                \
            return self->size;                                                                                  \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            return ALLOCATION_ERROR;                                                                            \
        }                                                                                                       \
    }                                                                                                           \
                                                                                                                \
    int container_t##_PopFront(container_t * const self)                                                        \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_node * old_begin_node = self->begin;                                                      \
        container_t##_node * new_begin_node = old_begin_node->next;                                             \
                                                                                                                \
        allocator_t##_Deallocate(&self->allocator, old_begin_node);                                             \
                                                                                                                \
        new_begin_node->prev = NULL;                                                                            \
        self->begin          = new_begin_node;                                                                  \
                                                                                                                \
        --self->size;                                                                                           \
                                                                                                                \
        return self->size;                                                                                      \
    }                                                                                                           \
                                                                                                                \
    int container_t##_Insert(container_t * const self, container_t##_Iterator * const iterator, member_t value) \
    {                                                                                                           \
        assert(self);                                                                                           \
        assert(iterator);                                                                                       \
                                                                                                                \
        container_t##_node * node =                                                                             \
            (container_t##_node *)allocator_t##_Allocate(&self->allocator, sizeof(container_t##_node));         \
        if(node)                                                                                                \
        {                                                                                                       \
            container_t##_node * next_Iterator = iterator->node;                                                \
            container_t##_node * prev_Iterator = NULL;                                                          \
            if(iterator->node != self->begin)                                                                   \
            {                                                                                                   \
                prev_Iterator       = next_Iterator->prev;                                                      \
                prev_Iterator->next = node;                                                                     \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                self->begin = node;                                                                             \
            }                                                                                                   \
                                                                                                                \
            next_Iterator->prev = node;                                                                         \
            node->next          = next_Iterator;                                                                \
            node->prev          = prev_Iterator;                                                                \
            node->value         = value;                                                                        \
                                                                                                                \
            ++self->size;                                                                                       \
                                                                                                                \
            return self->size;                                                                                  \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            return ALLOCATION_ERROR;                                                                            \
        }                                                                                                       \
    }                                                                                                           \
                                                                                                                \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator)                  \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_node * to_delete_node = iterator->node;                                                   \
        container_t##_node * next_node      = to_delete_node->next;                                             \
        container_t##_node * prev_node      = to_delete_node->prev;                                             \
                                                                                                                \
        allocator_t##_Deallocate(&self->allocator, to_delete_node);                                             \
                                                                                                                \
        if(!prev_node)                                                                                          \
        {                                                                                                       \
            self->begin = next_node;                                                                            \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            prev_node->next = next_node;                                                                        \
        }                                                                                                       \
        next_node->prev = prev_node;                                                                            \
                                                                                                                \
        --self->size;                                                                                           \
                                                                                                                \
        return self->size;                                                                                      \
    }                                                                                                           \
                                                                                                                \
    member_t container_t##_Front(const container_t * const self)                                                \
    {                                                                                                           \
        assert(self);                                                                                           \
        assert(self->size > 0);                                                                                 \
                                                                                                                \
        return self->begin->value;                                                                              \
    }                                                                                                           \
                                                                                                                \
    member_t container_t##_Back(const container_t * const self)                                                 \
    {                                                                                                           \
        assert(self);                                                                                           \
        assert(self->size > 0);                                                                                 \
                                                                                                                \
        return self->end->prev->value;                                                                          \
    }                                                                                                           \
                                                                                                                \
    member_t * container_t##_Ref(container_t * const self, size_t index)                                        \
    {                                                                                                           \
        container_t##_Iterator it = container_t##_Begin(self);                                                  \
                                                                                                                \
        while(index--)                                                                                          \
        {                                                                                                       \
            container_t##_Iterator_Increment(&it);                                                              \
        }                                                                                                       \
        member_t * value = &it.node->value;                                                                     \
        return value;                                                                                           \
    }                                                                                                           \
                                                                                                                \
    const member_t * container_t##_CRef(const container_t * const self, size_t index)                           \
    {                                                                                                           \
        container_t##_Iterator it = container_t##_Begin(self);                                                  \
                                                                                                                \
        while(index--)                                                                                          \
        {                                                                                                       \
            container_t##_Iterator_Increment(&it);                                                              \
        }                                                                                                       \
        const member_t * value = (const member_t *)&it.node->value;                                             \
        return value;                                                                                           \
    }                                                                                                           \
                                                                                                                \
    container_t##_Iterator container_t##_Begin(const container_t * const self)                                  \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_Iterator it;                                                                              \
        it.node = self->begin;                                                                                  \
        return it;                                                                                              \
    }                                                                                                           \
                                                                                                                \
    container_t##_Iterator container_t##_End(const container_t * const self)                                    \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_Iterator it;                                                                              \
        it.node = self->end;                                                                                    \
        return it;                                                                                              \
    }                                                                                                           \
                                                                                                                \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                               \
                                      const container_t##_Iterator * const second)                              \
    {                                                                                                           \
        const bool is_equal = first->node == second->node;                                                      \
        return is_equal;                                                                                        \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self)                                  \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        self->node = self->node->next;                                                                          \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self)                                  \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        self->node = self->node->prev;                                                                          \
    }                                                                                                           \
                                                                                                                \
    member_t * container_t##_Iterator_Ref(container_t##_Iterator * const self)                                  \
    {                                                                                                           \
        assert(self);                                                                                           \
        return &self->node->value;                                                                              \
    }                                                                                                           \
                                                                                                                \
    const member_t * container_t##_Iterator_CRef(const container_t##_Iterator * const self)                     \
    {                                                                                                           \
        assert(self);                                                                                           \
        return &self->node->value;                                                                              \
    }                                                                                                           \
                                                                                                                \
    container_t##_Iterator container_t##_Find(container_t * const self, const member_t data)                    \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_Iterator end = container_t##_End(self);                                                   \
        container_t##_Iterator it  = container_t##_Begin(self);                                                 \
                                                                                                                \
        for(; !container_t##_Iterator_Equal(&it, &end); container_t##_Iterator_Increment(&it))                  \
        {                                                                                                       \
            const member_t it_value = *container_t##_Iterator_CRef(&it);                                        \
            if(member_t##_Compare(&data, &it_value) == 0)                                                       \
            {                                                                                                   \
                break;                                                                                          \
            }                                                                                                   \
        }                                                                                                       \
        return it;                                                                                              \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Clear(container_t * const self)                                                          \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        while(container_t##_Size(self) != 0)                                                                    \
        {                                                                                                       \
            container_t##_Iterator begin = container_t##_Begin(self);                                           \
            container_t##_Erase(self, &begin);                                                                  \
        }                                                                                                       \
    }

#define static_list(container_t, member_t, container_capacity)                   \
    typedef struct container_t container_t;                                      \
    typedef struct container_t##_Iterator container_t##_Iterator;                \
    typedef struct container_t##_node container_t##_node;                        \
    typedef int (*container_t##_compare_t)(const member_t *, const member_t *);  \
    struct container_t##_node                                                    \
    {                                                                            \
        container_t##_node * prev;                                               \
        container_t##_node * next;                                               \
        member_t value;                                                          \
    };                                                                           \
                                                                                 \
    struct container_t##_Iterator                                                \
    {                                                                            \
        container_t##_node * node;                                               \
    };                                                                           \
                                                                                 \
    static_pool(container_t##_pool, container_t##_node, container_capacity + 1); \
                                                                                 \
    struct container_t                                                           \
    {                                                                            \
        container_t##_node * begin;                                              \
        container_t##_node * end;                                                \
        container_t##_pool pool;                                                 \
        size_t size;                                                             \
    };                                                                           \
    __list_methods_h(container_t, member_t);

#define static_list_impl(container_t, member_t, container_capacity)                   \
    static_pool_impl(container_t##_pool, container_t##_node, container_capacity + 1); \
    __static_list_methods_c(container_t, member_t, container_capacity)

#define custom_allocator_list(container_t, member_t, allocator_t)               \
    typedef struct container_t container_t;                                     \
    typedef struct container_t##_Iterator container_t##_Iterator;               \
    typedef struct container_t##_node container_t##_node;                       \
    typedef int (*container_t##_compare_t)(const member_t *, const member_t *); \
    struct container_t##_node                                                   \
    {                                                                           \
        container_t##_node * prev;                                              \
        container_t##_node * next;                                              \
        member_t value;                                                         \
    };                                                                          \
                                                                                \
    struct container_t##_Iterator                                               \
    {                                                                           \
        container_t##_node * node;                                              \
    };                                                                          \
                                                                                \
    struct container_t                                                          \
    {                                                                           \
        container_t##_node * begin;                                             \
        container_t##_node * end;                                               \
        allocator_t allocator;                                                  \
        size_t size;                                                            \
    };                                                                          \
    __list_methods_h(container_t, member_t);

#define custom_allocator_list_impl(container_t, member_t, allocator_t) \
    __custom_allocator_list_methods_c(container_t, member_t, allocator_t)

#define dynamic_list(container_t, member_t)         \
    dynamic_memory(container_t##_DynamicAllocator); \
    custom_allocator_list(container_t, member_t, container_t##_DynamicAllocator);

#define dynamic_list_impl(container_t, member_t)         \
    dynamic_memory_impl(container_t##_DynamicAllocator); \
    custom_allocator_list_impl(container_t, member_t, container_t##_DynamicAllocator);

#ifdef __cplusplus
}
#endif