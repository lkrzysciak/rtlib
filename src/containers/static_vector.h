#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "error_codes.h"

#define declare_static_vector_t(container_t, member_t, container_capacity)                                      \
                                                                                                                \
    typedef int (*container_t##_compare_t)(const member_t *, const member_t *);                                 \
                                                                                                                \
    typedef struct container_t                                                                                  \
    {                                                                                                           \
        size_t size;                                                                                            \
        member_t data[container_capacity];                                                                      \
        member_t * end;                                                                                         \
        container_t##_compare_t compare_function;                                                               \
    } container_t;                                                                                              \
                                                                                                                \
    typedef struct container_t##_Iterator                                                                       \
    {                                                                                                           \
        member_t * value;                                                                                       \
    } container_t##_Iterator;                                                                                   \
                                                                                                                \
    /* Deprecated - needed to keep compability with version 1 API */                                            \
    typedef container_t##_Iterator container_t##_iterator;                                                      \
                                                                                                                \
    void container_t##_Construct(container_t * const self, container_t##_compare_t compare_function);           \
    void container_t##_Destroy(container_t * const self);                                                       \
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
    member_t container_t##_GetValue(const container_t * const self, size_t index);                              \
    void container_t##_SetValue(container_t * const self, size_t index, member_t value);                        \
    container_t##_Iterator container_t##_Begin(const container_t * const self);                                 \
    container_t##_Iterator container_t##_End(const container_t * const self);                                   \
    member_t container_t##_Iterator_GetValue(const container_t##_Iterator * const self);                        \
    void container_t##_Iterator_SetValue(container_t##_Iterator * const self, member_t value);                  \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                               \
                                      const container_t##_Iterator * const second);                             \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self);                                 \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self);                                 \
    container_t##_Iterator container_t##_Find(container_t * const self, const member_t data);

#define define_static_vector_t(container_t, member_t, container_capacity)                                              \
    void container_t##_Construct(container_t * const self, container_t##_compare_t compare_function)                   \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        assert(sizeof(self->data) / sizeof(member_t) == container_capacity);                                           \
                                                                                                                       \
        self->size             = 0;                                                                                    \
        self->end              = self->data;                                                                           \
        self->compare_function = compare_function;                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Destroy(container_t * const self) {}                                                            \
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
        if(self->size < container_capacity)                                                                            \
        {                                                                                                              \
            *self->end = data;                                                                                         \
            ++self->end;                                                                                               \
            ++self->size;                                                                                              \
                                                                                                                       \
            return self->size;                                                                                         \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            return ALLOCATION_ERROR;                                                                                   \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_PopBack(container_t * const self)                                                                \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        --self->size;                                                                                                  \
        --self->end;                                                                                                   \
        return self->size;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_PushFront(container_t * const self, member_t data)                                               \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        if(self->size < container_capacity)                                                                            \
        {                                                                                                              \
            memmove(&self->data[1], &self->data[0], self->size * sizeof(member_t));                                    \
            self->data[0] = data;                                                                                      \
            ++self->end;                                                                                               \
            ++self->size;                                                                                              \
                                                                                                                       \
            return self->size;                                                                                         \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            return ALLOCATION_ERROR;                                                                                   \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_PopFront(container_t * const self)                                                               \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        --self->size;                                                                                                  \
        --self->end;                                                                                                   \
        memmove(&self->data[0], &self->data[1], self->size * sizeof(member_t));                                        \
        return self->size;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_Insert(container_t * const self, container_t##_Iterator * const iterator, member_t data)         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        assert(iterator);                                                                                              \
                                                                                                                       \
        if(self->size < container_capacity)                                                                            \
        {                                                                                                              \
            const size_t to_move_bytes = (uint8_t *)self->end - (uint8_t *)iterator->value;                            \
            memmove(iterator->value + 1, iterator->value, to_move_bytes);                                              \
            *iterator->value = data;                                                                                   \
            ++self->end;                                                                                               \
            ++self->size;                                                                                              \
                                                                                                                       \
            return self->size;                                                                                         \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            return ALLOCATION_ERROR;                                                                                   \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator)                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        assert(iterator);                                                                                              \
                                                                                                                       \
        --self->size;                                                                                                  \
        --self->end;                                                                                                   \
        const size_t to_move_bytes = (uint8_t *)self->end - (uint8_t *)iterator->value;                                \
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
        return *(self->end - 1);                                                                                       \
    }                                                                                                                  \
                                                                                                                       \
    member_t container_t##_GetValue(const container_t * const self, size_t index)                                      \
    {                                                                                                                  \
        member_t value = self->data[index];                                                                            \
        return value;                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_SetValue(container_t * const self, size_t index, member_t value) { self->data[index] = value; } \
                                                                                                                       \
    container_t##_Iterator container_t##_Begin(const container_t * const self)                                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_Iterator it;                                                                                     \
        it.value = (member_t *)self->data;                                                                             \
                                                                                                                       \
        return it;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_Iterator container_t##_End(const container_t * const self)                                           \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_Iterator it;                                                                                     \
        it.value = self->end;                                                                                          \
                                                                                                                       \
        return it;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    member_t container_t##_Iterator_GetValue(const container_t##_Iterator * const self)                                \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        return *self->value;                                                                                           \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Iterator_SetValue(container_t##_Iterator * const self, member_t value)                          \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        *self->value = value;                                                                                          \
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
    container_t##_Iterator container_t##_Find(container_t * const self, const member_t data)                           \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_Iterator end = container_t##_End(self);                                                          \
        container_t##_Iterator it  = container_t##_Begin(self);                                                        \
                                                                                                                       \
        for(; !container_t##_Iterator_Equal(&it, &end); container_t##_Iterator_Increment(&it))                         \
        {                                                                                                              \
            const member_t it_value = container_t##_Iterator_GetValue(&it);                                            \
            if(self->compare_function(&data, &it_value) == 0)                                                          \
            {                                                                                                          \
                break;                                                                                                 \
            }                                                                                                          \
        }                                                                                                              \
        return it;                                                                                                     \
    }
