#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "error_codes.h"
#include "rtlib/memory.h"

#define __vector_methods_h(container_t, member_t)                                                               \
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

#define __static_vector_methods_c(container_t, member_t)                                                       \
    void container_t##_Construct(container_t * const self)                                                     \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        self->size     = 0;                                                                                    \
        self->capacity = sizeof(self->data) / sizeof(member_t);                                                \
    }                                                                                                          \
                                                                                                               \
    void container_t##_Destruct(container_t * const self)                                                      \
    {}                                                                                                         \
                                                                                                               \
    size_t container_t##_Size(const container_t * const self)                                                  \
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
    int container_t##_PushBack(container_t * const self, member_t data)                                        \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        if(self->size < self->capacity)                                                                        \
        {                                                                                                      \
            self->data[self->size] = data;                                                                     \
            ++self->size;                                                                                      \
                                                                                                               \
            return self->size;                                                                                 \
        }                                                                                                      \
        else                                                                                                   \
        {                                                                                                      \
            return ALLOCATION_ERROR;                                                                           \
        }                                                                                                      \
    }                                                                                                          \
                                                                                                               \
    int container_t##_PopBack(container_t * const self)                                                        \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        --self->size;                                                                                          \
        return self->size;                                                                                     \
    }                                                                                                          \
                                                                                                               \
    int container_t##_PushFront(container_t * const self, member_t data)                                       \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        if(self->size < self->capacity)                                                                        \
        {                                                                                                      \
            memmove(&self->data[1], &self->data[0], self->size * sizeof(member_t));                            \
            self->data[0] = data;                                                                              \
            ++self->size;                                                                                      \
                                                                                                               \
            return self->size;                                                                                 \
        }                                                                                                      \
        else                                                                                                   \
        {                                                                                                      \
            return ALLOCATION_ERROR;                                                                           \
        }                                                                                                      \
    }                                                                                                          \
                                                                                                               \
    int container_t##_PopFront(container_t * const self)                                                       \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        --self->size;                                                                                          \
        memmove(&self->data[0], &self->data[1], self->size * sizeof(member_t));                                \
        return self->size;                                                                                     \
    }                                                                                                          \
                                                                                                               \
    int container_t##_Insert(container_t * const self, container_t##_Iterator * const iterator, member_t data) \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(iterator);                                                                                      \
                                                                                                               \
        if(self->size < self->capacity)                                                                        \
        {                                                                                                      \
            uint8_t * end              = (uint8_t *)&self->data[self->size];                                   \
            const size_t to_move_bytes = end - (uint8_t *)iterator->value;                                     \
            memmove(iterator->value + 1, iterator->value, to_move_bytes);                                      \
            *iterator->value = data;                                                                           \
            ++self->size;                                                                                      \
                                                                                                               \
            return self->size;                                                                                 \
        }                                                                                                      \
        else                                                                                                   \
        {                                                                                                      \
            return ALLOCATION_ERROR;                                                                           \
        }                                                                                                      \
    }                                                                                                          \
                                                                                                               \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator)                 \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(iterator);                                                                                      \
                                                                                                               \
        uint8_t * end = (uint8_t *)&self->data[self->size];                                                    \
        --self->size;                                                                                          \
        const size_t to_move_bytes = end - (uint8_t *)iterator->value;                                         \
        memmove(iterator->value, iterator->value + 1, to_move_bytes);                                          \
        return self->size;                                                                                     \
    }                                                                                                          \
                                                                                                               \
    member_t container_t##_Front(const container_t * const self)                                               \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(self->size > 0);                                                                                \
                                                                                                               \
        return self->data[0];                                                                                  \
    }                                                                                                          \
                                                                                                               \
    member_t container_t##_Back(const container_t * const self)                                                \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(self->size > 0);                                                                                \
                                                                                                               \
        return self->data[self->size - 1];                                                                     \
    }                                                                                                          \
                                                                                                               \
    member_t * container_t##_Ref(container_t * const self, size_t index)                                       \
    {                                                                                                          \
        member_t * value = &self->data[index];                                                                 \
        return value;                                                                                          \
    }                                                                                                          \
                                                                                                               \
    const member_t * container_t##_CRef(const container_t * const self, size_t index)                          \
    {                                                                                                          \
        const member_t * value = (const member_t *)&self->data[index];                                         \
        return value;                                                                                          \
    }                                                                                                          \
                                                                                                               \
    container_t##_Iterator container_t##_Begin(const container_t * const self)                                 \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        container_t##_Iterator it;                                                                             \
        it.value = (member_t *)self->data;                                                                     \
                                                                                                               \
        return it;                                                                                             \
    }                                                                                                          \
                                                                                                               \
    container_t##_Iterator container_t##_End(const container_t * const self)                                   \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        container_t##_Iterator it;                                                                             \
        it.value = (member_t *)&self->data[self->size];                                                        \
                                                                                                               \
        return it;                                                                                             \
    }                                                                                                          \
                                                                                                               \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                              \
                                      const container_t##_Iterator * const second)                             \
    {                                                                                                          \
        const bool is_equal = first->value == second->value;                                                   \
        return is_equal;                                                                                       \
    }                                                                                                          \
                                                                                                               \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self)                                 \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        self->value = self->value + 1;                                                                         \
    }                                                                                                          \
                                                                                                               \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self)                                 \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        self->value = self->value - 1;                                                                         \
    }                                                                                                          \
                                                                                                               \
    member_t * container_t##_Iterator_Ref(container_t##_Iterator * const self)                                 \
    {                                                                                                          \
        assert(self);                                                                                          \
        return self->value;                                                                                    \
    }                                                                                                          \
                                                                                                               \
    const member_t * container_t##_Iterator_CRef(const container_t##_Iterator * const self)                    \
    {                                                                                                          \
        assert(self);                                                                                          \
        return self->value;                                                                                    \
    }                                                                                                          \
                                                                                                               \
    container_t##_Iterator container_t##_Find(container_t * const self, const member_t data)                   \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
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
        while(container_t##_Size(self) != 0)                                                                   \
        {                                                                                                      \
            container_t##_Iterator begin = container_t##_Begin(self);                                          \
            container_t##_Erase(self, &begin);                                                                 \
        }                                                                                                      \
    }

#define __custom_vector_methods_c(container_t, member_t, allocator_t)                                                  \
    void container_t##_Construct(container_t * const self)                                                             \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        self->capacity = 4;                                                                                            \
        allocator_t##_Construct(&self->allocator);                                                                     \
        self->data = (member_t *)allocator_t##_Allocate(&self->allocator, self->capacity * sizeof(member_t));          \
        assert(self->data);                                                                                            \
        self->size = 0;                                                                                                \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Destruct(container_t * const self)                                                              \
    {                                                                                                                  \
        allocator_t##_Deallocate(&self->allocator, self->data);                                                        \
        allocator_t##_Destruct(&self->allocator);                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    size_t container_t##_Size(const container_t * const self)                                                          \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        return self->size;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    bool container_t##_Empty(const container_t * const self)                                                           \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        return self->size == 0;                                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_PushBack(container_t * const self, member_t data)                                                \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        if(self->size < self->capacity)                                                                                \
        {                                                                                                              \
            self->data[self->size] = data;                                                                             \
            ++self->size;                                                                                              \
                                                                                                                       \
            return self->size;                                                                                         \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            self->capacity *= 2;                                                                                       \
            member_t * new_data =                                                                                      \
                (member_t *)allocator_t##_Reallocate(&self->allocator, self->data, self->capacity * sizeof(member_t)); \
            if(new_data)                                                                                               \
            {                                                                                                          \
                self->data             = new_data;                                                                     \
                self->data[self->size] = data;                                                                         \
                ++self->size;                                                                                          \
                                                                                                                       \
                return self->size;                                                                                     \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                return ALLOCATION_ERROR;                                                                               \
            }                                                                                                          \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_PopBack(container_t * const self)                                                                \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        --self->size;                                                                                                  \
        return self->size;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_PushFront(container_t * const self, member_t data)                                               \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        if(self->size < self->capacity)                                                                                \
        {                                                                                                              \
            memmove(&self->data[1], &self->data[0], self->size * sizeof(member_t));                                    \
            self->data[0] = data;                                                                                      \
            ++self->size;                                                                                              \
                                                                                                                       \
            return self->size;                                                                                         \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            self->capacity *= 2;                                                                                       \
            member_t * new_data =                                                                                      \
                (member_t *)allocator_t##_Reallocate(&self->allocator, self->data, self->capacity * sizeof(member_t)); \
            if(new_data)                                                                                               \
            {                                                                                                          \
                self->data = new_data;                                                                                 \
                memmove(&self->data[1], &self->data[0], self->size * sizeof(member_t));                                \
                self->data[0] = data;                                                                                  \
                ++self->size;                                                                                          \
                                                                                                                       \
                return self->size;                                                                                     \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                return ALLOCATION_ERROR;                                                                               \
            }                                                                                                          \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_PopFront(container_t * const self)                                                               \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        --self->size;                                                                                                  \
        memmove(&self->data[0], &self->data[1], self->size * sizeof(member_t));                                        \
        return self->size;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_Insert(container_t * const self, container_t##_Iterator * const iterator, member_t data)         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        assert(iterator);                                                                                              \
                                                                                                                       \
        if(self->size < self->capacity)                                                                                \
        {                                                                                                              \
            const size_t to_move_bytes = (uint8_t *)&self->data[self->size] - (uint8_t *)iterator->value;              \
            memmove(iterator->value + 1, iterator->value, to_move_bytes);                                              \
            *iterator->value = data;                                                                                   \
            ++self->size;                                                                                              \
                                                                                                                       \
            return self->size;                                                                                         \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            const unsigned int iteratorIndex = (iterator->value - self->data) / sizeof(member_t);                      \
            self->capacity *= 2;                                                                                       \
            member_t * new_data =                                                                                      \
                (member_t *)allocator_t##_Reallocate(&self->allocator, self->data, self->capacity * sizeof(member_t)); \
            if(new_data)                                                                                               \
            {                                                                                                          \
                self->data                 = new_data;                                                                 \
                const size_t to_move_bytes = (self->size - iteratorIndex) * sizeof(member_t);                          \
                memmove(&self->data[iteratorIndex + 1], &self->data[iteratorIndex], to_move_bytes);                    \
                self->data[iteratorIndex] = data;                                                                      \
                ++self->size;                                                                                          \
                                                                                                                       \
                return self->size;                                                                                     \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                return ALLOCATION_ERROR;                                                                               \
            }                                                                                                          \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator)                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        assert(iterator);                                                                                              \
                                                                                                                       \
        --self->size;                                                                                                  \
        const size_t to_move_bytes = (uint8_t *)&self->data[self->size] - (uint8_t *)iterator->value;                  \
        memmove(iterator->value, iterator->value + 1, to_move_bytes);                                                  \
        return self->size;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    member_t container_t##_Front(const container_t * const self)                                                       \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        assert(self->size > 0);                                                                                        \
                                                                                                                       \
        return self->data[0];                                                                                          \
    }                                                                                                                  \
                                                                                                                       \
    member_t container_t##_Back(const container_t * const self)                                                        \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        assert(self->size > 0);                                                                                        \
                                                                                                                       \
        return *(&self->data[self->size] - 1);                                                                         \
    }                                                                                                                  \
                                                                                                                       \
    member_t * container_t##_Ref(container_t * const self, size_t index)                                               \
    {                                                                                                                  \
        member_t * value = &self->data[index];                                                                         \
        return value;                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    const member_t * container_t##_CRef(const container_t * const self, size_t index)                                  \
    {                                                                                                                  \
        const member_t * value = (const member_t *)&self->data[index];                                                 \
        return value;                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_Iterator container_t##_Begin(const container_t * const self)                                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_Iterator it;                                                                                     \
        it.value = self->data;                                                                                         \
                                                                                                                       \
        return it;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_Iterator container_t##_End(const container_t * const self)                                           \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_Iterator it;                                                                                     \
        it.value = &self->data[self->size];                                                                            \
                                                                                                                       \
        return it;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                                      \
                                      const container_t##_Iterator * const second)                                     \
    {                                                                                                                  \
        const bool is_equal = first->value == second->value;                                                           \
        return is_equal;                                                                                               \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self)                                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        self->value = self->value + 1;                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self)                                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        self->value = self->value - 1;                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    member_t * container_t##_Iterator_Ref(container_t##_Iterator * const self)                                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        return self->value;                                                                                            \
    }                                                                                                                  \
                                                                                                                       \
    const member_t * container_t##_Iterator_CRef(const container_t##_Iterator * const self)                            \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        return self->value;                                                                                            \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_Iterator container_t##_Find(container_t * const self, const member_t data)                           \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_Iterator end = container_t##_End(self);                                                          \
        container_t##_Iterator it  = container_t##_Begin(self);                                                        \
                                                                                                                       \
        for(; !container_t##_Iterator_Equal(&it, &end); container_t##_Iterator_Increment(&it))                         \
        {                                                                                                              \
            const member_t it_value = *container_t##_Iterator_CRef(&it);                                               \
            if(member_t##_Compare(&data, &it_value) == 0)                                                              \
            {                                                                                                          \
                break;                                                                                                 \
            }                                                                                                          \
        }                                                                                                              \
        return it;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Clear(container_t * const self)                                                                 \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        while(container_t##_Size(self) != 0)                                                                           \
        {                                                                                                              \
            container_t##_Iterator begin = container_t##_Begin(self);                                                  \
            container_t##_Erase(self, &begin);                                                                         \
        }                                                                                                              \
    }

#define static_vector(container_t, member_t, container_capacity)                \
    typedef int (*container_t##_compare_t)(const member_t *, const member_t *); \
    typedef struct container_t container_t;                                     \
    typedef struct container_t##_Iterator container_t##_Iterator;               \
    struct container_t                                                          \
    {                                                                           \
        size_t size;                                                            \
        size_t capacity;                                                        \
        member_t data[container_capacity];                                      \
        member_t * end;                                                         \
    };                                                                          \
    struct container_t##_Iterator                                               \
    {                                                                           \
        member_t * value;                                                       \
    };                                                                          \
    __vector_methods_h(container_t, member_t)

#define static_vector_impl(container_t, member_t, container_capacity) __static_vector_methods_c(container_t, member_t)

#define custom_allocator_vector(container_t, member_t, allocator_t)             \
    typedef int (*container_t##_compare_t)(const member_t *, const member_t *); \
    typedef struct container_t container_t;                                     \
    typedef struct container_t##_Iterator container_t##_Iterator;               \
    struct container_t                                                          \
    {                                                                           \
        size_t size;                                                            \
        size_t capacity;                                                        \
        member_t * data;                                                        \
        allocator_t allocator;                                                  \
    };                                                                          \
    struct container_t##_Iterator                                               \
    {                                                                           \
        member_t * value;                                                       \
    };                                                                          \
    __vector_methods_h(container_t, member_t)

#define custom_allocator_vector_impl(container_t, member_t, allocator_t) \
    __custom_vector_methods_c(container_t, member_t, allocator_t)

#define dynamic_vector(container_t, member_t)       \
    dynamic_memory(container_t##_DynamicAllocator); \
    custom_allocator_vector(container_t, member_t, container_t##_DynamicAllocator);

#define dynamic_vector_impl(container_t, member_t)       \
    dynamic_memory_impl(container_t##_DynamicAllocator); \
    custom_allocator_vector_impl(container_t, member_t, container_t##_DynamicAllocator);

#ifdef __cplusplus
}
#endif