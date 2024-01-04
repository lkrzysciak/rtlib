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

#define __string_methods_h(container_t)                                                                     \
    void container_t##_Construct(container_t * const self);                                                 \
    void container_t##_Construct2(container_t * const self, const char * data, size_t size);                \
    void container_t##_Construct3(container_t * const self, const char * data);                             \
    void container_t##_Destruct(container_t * const self);                                                  \
    size_t container_t##_Size(const container_t * const self);                                              \
    bool container_t##_Empty(const container_t * const self);                                               \
    int container_t##_PushBack(container_t * const self, char data);                                        \
    int container_t##_PopBack(container_t * const self);                                                    \
    int container_t##_PushFront(container_t * const self, char data);                                       \
    int container_t##_PopFront(container_t * const self);                                                   \
    int container_t##_Insert(container_t * const self, container_t##_Iterator * const iterator, char data); \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator);             \
    void container_t##_Clear(container_t * const self);                                                     \
    char container_t##_Front(const container_t * const self);                                               \
    char container_t##_Back(const container_t * const self);                                                \
    char * container_t##_Ref(container_t * const self, size_t index);                                       \
    const char * container_t##_CRef(const container_t * const self, size_t index);                          \
    container_t##_Iterator container_t##_Find(container_t * const self, const char data);                   \
                                                                                                            \
    container_t##_Iterator container_t##_Begin(const container_t * const self);                             \
    container_t##_Iterator container_t##_End(const container_t * const self);                               \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                           \
                                      const container_t##_Iterator * const second);                         \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self);                             \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self);                             \
    char * container_t##_Iterator_Ref(container_t##_Iterator * const self);                                 \
    const char * container_t##_Iterator_CRef(const container_t##_Iterator * const self);                    \
                                                                                                            \
    int container_t##_Compare(const container_t * const self, const container_t * const second);            \
    unsigned int container_t##_Hash(const container_t * const self);

#define __static_string_methods_c(container_t)                                                             \
    void container_t##_Construct(container_t * const self)                                                 \
    {                                                                                                      \
        assert(self);                                                                                      \
                                                                                                           \
        self->size     = 0;                                                                                \
        self->capacity = sizeof(self->data) / sizeof(char) - 1;                                            \
        self->data[0]  = 0;                                                                                \
    }                                                                                                      \
                                                                                                           \
    void container_t##_Construct2(container_t * const self, const char * data, size_t size)                \
    {                                                                                                      \
        container_t##_Construct(self);                                                                     \
        for(size_t idx = 0; idx < size; idx++)                                                             \
        {                                                                                                  \
            container_t##_PushBack(self, data[idx]);                                                       \
        }                                                                                                  \
    }                                                                                                      \
                                                                                                           \
    void container_t##_Construct3(container_t * const self, const char * data)                             \
    {                                                                                                      \
        container_t##_Construct(self);                                                                     \
        size_t size = strlen(data);                                                                        \
        for(size_t idx = 0; idx < size; idx++)                                                             \
        {                                                                                                  \
            container_t##_PushBack(self, data[idx]);                                                       \
        }                                                                                                  \
    }                                                                                                      \
                                                                                                           \
    void container_t##_Destruct(container_t * const self)                                                  \
    {}                                                                                                     \
                                                                                                           \
    size_t container_t##_Size(const container_t * const self)                                              \
    {                                                                                                      \
        assert(self);                                                                                      \
                                                                                                           \
        return self->size;                                                                                 \
    }                                                                                                      \
                                                                                                           \
    bool container_t##_Empty(const container_t * const self)                                               \
    {                                                                                                      \
        assert(self);                                                                                      \
                                                                                                           \
        return self->size == 0;                                                                            \
    }                                                                                                      \
                                                                                                           \
    int container_t##_PushBack(container_t * const self, char data)                                        \
    {                                                                                                      \
        assert(self);                                                                                      \
                                                                                                           \
        if(self->size < self->capacity)                                                                    \
        {                                                                                                  \
            self->data[self->size] = data;                                                                 \
            ++self->size;                                                                                  \
            self->data[self->size] = 0;                                                                    \
                                                                                                           \
            return self->size;                                                                             \
        }                                                                                                  \
        else                                                                                               \
        {                                                                                                  \
            return ALLOCATION_ERROR;                                                                       \
        }                                                                                                  \
    }                                                                                                      \
                                                                                                           \
    int container_t##_PopBack(container_t * const self)                                                    \
    {                                                                                                      \
        assert(self);                                                                                      \
                                                                                                           \
        --self->size;                                                                                      \
        self->data[self->size] = 0;                                                                        \
        return self->size;                                                                                 \
    }                                                                                                      \
                                                                                                           \
    int container_t##_PushFront(container_t * const self, char data)                                       \
    {                                                                                                      \
        assert(self);                                                                                      \
                                                                                                           \
        if(self->size < self->capacity)                                                                    \
        {                                                                                                  \
            memmove(&self->data[1], &self->data[0], self->size * sizeof(char));                            \
            self->data[0] = data;                                                                          \
            ++self->size;                                                                                  \
            self->data[self->size] = 0;                                                                    \
                                                                                                           \
            return self->size;                                                                             \
        }                                                                                                  \
        else                                                                                               \
        {                                                                                                  \
            return ALLOCATION_ERROR;                                                                       \
        }                                                                                                  \
    }                                                                                                      \
                                                                                                           \
    int container_t##_PopFront(container_t * const self)                                                   \
    {                                                                                                      \
        assert(self);                                                                                      \
                                                                                                           \
        --self->size;                                                                                      \
        memmove(&self->data[0], &self->data[1], self->size * sizeof(char));                                \
        self->data[self->size] = 0;                                                                        \
        return self->size;                                                                                 \
    }                                                                                                      \
                                                                                                           \
    int container_t##_Insert(container_t * const self, container_t##_Iterator * const iterator, char data) \
    {                                                                                                      \
        assert(self);                                                                                      \
        assert(iterator);                                                                                  \
                                                                                                           \
        if(self->size < self->capacity)                                                                    \
        {                                                                                                  \
            uint8_t * end              = (uint8_t *)&self->data[self->size];                               \
            const size_t to_move_bytes = end - (uint8_t *)iterator->value;                                 \
            memmove(iterator->value + 1, iterator->value, to_move_bytes);                                  \
            *iterator->value = data;                                                                       \
            ++self->size;                                                                                  \
            self->data[self->size] = 0;                                                                    \
                                                                                                           \
            return self->size;                                                                             \
        }                                                                                                  \
        else                                                                                               \
        {                                                                                                  \
            return ALLOCATION_ERROR;                                                                       \
        }                                                                                                  \
    }                                                                                                      \
                                                                                                           \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator)             \
    {                                                                                                      \
        assert(self);                                                                                      \
        assert(iterator);                                                                                  \
                                                                                                           \
        uint8_t * end = (uint8_t *)&self->data[self->size];                                                \
        --self->size;                                                                                      \
        const size_t to_move_bytes = end - (uint8_t *)iterator->value;                                     \
        memmove(iterator->value, iterator->value + 1, to_move_bytes);                                      \
        self->data[self->size] = 0;                                                                        \
        return self->size;                                                                                 \
    }                                                                                                      \
                                                                                                           \
    char container_t##_Front(const container_t * const self)                                               \
    {                                                                                                      \
        assert(self);                                                                                      \
        assert(self->size > 0);                                                                            \
                                                                                                           \
        return self->data[0];                                                                              \
    }                                                                                                      \
                                                                                                           \
    char container_t##_Back(const container_t * const self)                                                \
    {                                                                                                      \
        assert(self);                                                                                      \
        assert(self->size > 0);                                                                            \
                                                                                                           \
        return self->data[self->size - 1];                                                                 \
    }                                                                                                      \
                                                                                                           \
    char * container_t##_Ref(container_t * const self, size_t index)                                       \
    {                                                                                                      \
        char * value = &self->data[index];                                                                 \
        return value;                                                                                      \
    }                                                                                                      \
                                                                                                           \
    const char * container_t##_CRef(const container_t * const self, size_t index)                          \
    {                                                                                                      \
        const char * value = (const char *)&self->data[index];                                             \
        return value;                                                                                      \
    }                                                                                                      \
                                                                                                           \
    container_t##_Iterator container_t##_Begin(const container_t * const self)                             \
    {                                                                                                      \
        assert(self);                                                                                      \
                                                                                                           \
        container_t##_Iterator it;                                                                         \
        it.value = (char *)self->data;                                                                     \
                                                                                                           \
        return it;                                                                                         \
    }                                                                                                      \
                                                                                                           \
    container_t##_Iterator container_t##_End(const container_t * const self)                               \
    {                                                                                                      \
        assert(self);                                                                                      \
                                                                                                           \
        container_t##_Iterator it;                                                                         \
        it.value = (char *)&self->data[self->size];                                                        \
                                                                                                           \
        return it;                                                                                         \
    }                                                                                                      \
                                                                                                           \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                          \
                                      const container_t##_Iterator * const second)                         \
    {                                                                                                      \
        const bool is_equal = first->value == second->value;                                               \
        return is_equal;                                                                                   \
    }                                                                                                      \
                                                                                                           \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self)                             \
    {                                                                                                      \
        assert(self);                                                                                      \
                                                                                                           \
        self->value = self->value + 1;                                                                     \
    }                                                                                                      \
                                                                                                           \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self)                             \
    {                                                                                                      \
        assert(self);                                                                                      \
                                                                                                           \
        self->value = self->value - 1;                                                                     \
    }                                                                                                      \
                                                                                                           \
    char * container_t##_Iterator_Ref(container_t##_Iterator * const self)                                 \
    {                                                                                                      \
        assert(self);                                                                                      \
        return self->value;                                                                                \
    }                                                                                                      \
                                                                                                           \
    const char * container_t##_Iterator_CRef(const container_t##_Iterator * const self)                    \
    {                                                                                                      \
        assert(self);                                                                                      \
        return self->value;                                                                                \
    }                                                                                                      \
                                                                                                           \
    container_t##_Iterator container_t##_Find(container_t * const self, const char data)                   \
    {                                                                                                      \
        assert(self);                                                                                      \
                                                                                                           \
        container_t##_Iterator end = container_t##_End(self);                                              \
        container_t##_Iterator it  = container_t##_Begin(self);                                            \
                                                                                                           \
        for(; !container_t##_Iterator_Equal(&it, &end); container_t##_Iterator_Increment(&it))             \
        {                                                                                                  \
            const char it_value = *container_t##_Iterator_CRef(&it);                                       \
            if(it_value == data)                                                                           \
            {                                                                                              \
                break;                                                                                     \
            }                                                                                              \
        }                                                                                                  \
        return it;                                                                                         \
    }                                                                                                      \
                                                                                                           \
    void container_t##_Clear(container_t * const self)                                                     \
    {                                                                                                      \
        assert(self);                                                                                      \
                                                                                                           \
        while(container_t##_Size(self) != 0)                                                               \
        {                                                                                                  \
            container_t##_Iterator begin = container_t##_Begin(self);                                      \
            container_t##_Erase(self, &begin);                                                             \
        }                                                                                                  \
    }                                                                                                      \
                                                                                                           \
    int container_t##_Compare(const container_t * const self, const container_t * const second)            \
    {                                                                                                      \
        return strcmp(self->data, second->data);                                                           \
    }                                                                                                      \
                                                                                                           \
    unsigned int container_t##_Hash(const container_t * const self)                                        \
    {                                                                                                      \
        unsigned long hash = 5381;                                                                         \
        int c;                                                                                             \
        const char * str = self->data;                                                                     \
                                                                                                           \
        while(str++)                                                                                       \
        {                                                                                                  \
            c    = *str;                                                                                   \
            hash = ((hash << 5) + hash) + c;                                                               \
        }                                                                                                  \
                                                                                                           \
        return hash;                                                                                       \
    }

#define __custom_string_methods_c(container_t, allocator_t)                                                    \
    void container_t##_Construct(container_t * const self)                                                     \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        self->capacity = 4;                                                                                    \
        allocator_t##_Construct(&self->allocator);                                                             \
        self->data = (char *)allocator_t##_Allocate(&self->allocator, self->capacity * sizeof(char));          \
        assert(self->data);                                                                                    \
        self->data[0] = 0;                                                                                     \
        self->size    = 0;                                                                                     \
    }                                                                                                          \
                                                                                                               \
    void container_t##_Construct2(container_t * const self, const char * data, size_t size)                    \
    {                                                                                                          \
        container_t##_Construct(self);                                                                         \
        for(size_t idx = 0; idx < size; idx++)                                                                 \
        {                                                                                                      \
            container_t##_PushBack(self, data[idx]);                                                           \
        }                                                                                                      \
    }                                                                                                          \
                                                                                                               \
    void container_t##_Construct3(container_t * const self, const char * data)                                 \
    {                                                                                                          \
        container_t##_Construct(self);                                                                         \
        size_t size = strlen(data);                                                                            \
        for(size_t idx = 0; idx < size; idx++)                                                                 \
        {                                                                                                      \
            container_t##_PushBack(self, data[idx]);                                                           \
        }                                                                                                      \
    }                                                                                                          \
                                                                                                               \
    void container_t##_Destruct(container_t * const self)                                                      \
    {                                                                                                          \
        allocator_t##_Deallocate(&self->allocator, self->data);                                                \
        allocator_t##_Destruct(&self->allocator);                                                              \
    }                                                                                                          \
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
    int container_t##_PushBack(container_t * const self, char data)                                            \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        if(self->size < self->capacity - 1)                                                                    \
        {                                                                                                      \
            self->data[self->size] = data;                                                                     \
            ++self->size;                                                                                      \
            self->data[self->size] = 0;                                                                        \
                                                                                                               \
            return self->size;                                                                                 \
        }                                                                                                      \
        else                                                                                                   \
        {                                                                                                      \
            self->capacity *= 2;                                                                               \
            char * new_data =                                                                                  \
                (char *)allocator_t##_Reallocate(&self->allocator, self->data, self->capacity * sizeof(char)); \
            if(new_data)                                                                                       \
            {                                                                                                  \
                self->data             = new_data;                                                             \
                self->data[self->size] = data;                                                                 \
                ++self->size;                                                                                  \
                                                                                                               \
                return self->size;                                                                             \
            }                                                                                                  \
            else                                                                                               \
            {                                                                                                  \
                return ALLOCATION_ERROR;                                                                       \
            }                                                                                                  \
        }                                                                                                      \
    }                                                                                                          \
                                                                                                               \
    int container_t##_PopBack(container_t * const self)                                                        \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        --self->size;                                                                                          \
        self->data[self->size] = 0;                                                                            \
        return self->size;                                                                                     \
    }                                                                                                          \
                                                                                                               \
    int container_t##_PushFront(container_t * const self, char data)                                           \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        if(self->size < self->capacity - 1)                                                                    \
        {                                                                                                      \
            memmove(&self->data[1], &self->data[0], self->size * sizeof(char));                                \
            self->data[0] = data;                                                                              \
            ++self->size;                                                                                      \
            self->data[self->size] = 0;                                                                        \
                                                                                                               \
            return self->size;                                                                                 \
        }                                                                                                      \
        else                                                                                                   \
        {                                                                                                      \
            self->capacity *= 2;                                                                               \
            char * new_data =                                                                                  \
                (char *)allocator_t##_Reallocate(&self->allocator, self->data, self->capacity * sizeof(char)); \
            if(new_data)                                                                                       \
            {                                                                                                  \
                self->data = new_data;                                                                         \
                memmove(&self->data[1], &self->data[0], self->size * sizeof(char));                            \
                self->data[0] = data;                                                                          \
                ++self->size;                                                                                  \
                                                                                                               \
                return self->size;                                                                             \
            }                                                                                                  \
            else                                                                                               \
            {                                                                                                  \
                return ALLOCATION_ERROR;                                                                       \
            }                                                                                                  \
        }                                                                                                      \
    }                                                                                                          \
                                                                                                               \
    int container_t##_PopFront(container_t * const self)                                                       \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        --self->size;                                                                                          \
        memmove(&self->data[0], &self->data[1], self->size * sizeof(char));                                    \
        self->data[self->size] = 0;                                                                            \
        return self->size;                                                                                     \
    }                                                                                                          \
                                                                                                               \
    int container_t##_Insert(container_t * const self, container_t##_Iterator * const iterator, char data)     \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(iterator);                                                                                      \
                                                                                                               \
        if(self->size < self->capacity - 1)                                                                    \
        {                                                                                                      \
            const size_t to_move_bytes = (uint8_t *)&self->data[self->size] - (uint8_t *)iterator->value;      \
            memmove(iterator->value + 1, iterator->value, to_move_bytes);                                      \
            *iterator->value = data;                                                                           \
            ++self->size;                                                                                      \
            self->data[self->size] = 0;                                                                        \
                                                                                                               \
            return self->size;                                                                                 \
        }                                                                                                      \
        else                                                                                                   \
        {                                                                                                      \
            const unsigned int iteratorIndex = (iterator->value - self->data) / sizeof(char);                  \
            self->capacity *= 2;                                                                               \
            char * new_data =                                                                                  \
                (char *)allocator_t##_Reallocate(&self->allocator, self->data, self->capacity * sizeof(char)); \
            if(new_data)                                                                                       \
            {                                                                                                  \
                self->data                 = new_data;                                                         \
                const size_t to_move_bytes = (self->size - iteratorIndex) * sizeof(char);                      \
                memmove(&self->data[iteratorIndex + 1], &self->data[iteratorIndex], to_move_bytes);            \
                self->data[iteratorIndex] = data;                                                              \
                ++self->size;                                                                                  \
                self->data[self->size] = 0;                                                                    \
                                                                                                               \
                return self->size;                                                                             \
            }                                                                                                  \
            else                                                                                               \
            {                                                                                                  \
                return ALLOCATION_ERROR;                                                                       \
            }                                                                                                  \
        }                                                                                                      \
    }                                                                                                          \
                                                                                                               \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator)                 \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(iterator);                                                                                      \
                                                                                                               \
        --self->size;                                                                                          \
        const size_t to_move_bytes = (uint8_t *)&self->data[self->size] - (uint8_t *)iterator->value;          \
        memmove(iterator->value, iterator->value + 1, to_move_bytes);                                          \
        self->data[self->size] = 0;                                                                            \
        return self->size;                                                                                     \
    }                                                                                                          \
                                                                                                               \
    char container_t##_Front(const container_t * const self)                                                   \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(self->size > 0);                                                                                \
                                                                                                               \
        return self->data[0];                                                                                  \
    }                                                                                                          \
                                                                                                               \
    char container_t##_Back(const container_t * const self)                                                    \
    {                                                                                                          \
        assert(self);                                                                                          \
        assert(self->size > 0);                                                                                \
                                                                                                               \
        return *(&self->data[self->size] - 1);                                                                 \
    }                                                                                                          \
                                                                                                               \
    char * container_t##_Ref(container_t * const self, size_t index)                                           \
    {                                                                                                          \
        char * value = &self->data[index];                                                                     \
        return value;                                                                                          \
    }                                                                                                          \
                                                                                                               \
    const char * container_t##_CRef(const container_t * const self, size_t index)                              \
    {                                                                                                          \
        const char * value = (const char *)&self->data[index];                                                 \
        return value;                                                                                          \
    }                                                                                                          \
                                                                                                               \
    container_t##_Iterator container_t##_Begin(const container_t * const self)                                 \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        container_t##_Iterator it;                                                                             \
        it.value = self->data;                                                                                 \
                                                                                                               \
        return it;                                                                                             \
    }                                                                                                          \
                                                                                                               \
    container_t##_Iterator container_t##_End(const container_t * const self)                                   \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        container_t##_Iterator it;                                                                             \
        it.value = &self->data[self->size];                                                                    \
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
    char * container_t##_Iterator_Ref(container_t##_Iterator * const self)                                     \
    {                                                                                                          \
        assert(self);                                                                                          \
        return self->value;                                                                                    \
    }                                                                                                          \
                                                                                                               \
    const char * container_t##_Iterator_CRef(const container_t##_Iterator * const self)                        \
    {                                                                                                          \
        assert(self);                                                                                          \
        return self->value;                                                                                    \
    }                                                                                                          \
                                                                                                               \
    container_t##_Iterator container_t##_Find(container_t * const self, const char data)                       \
    {                                                                                                          \
        assert(self);                                                                                          \
                                                                                                               \
        container_t##_Iterator end = container_t##_End(self);                                                  \
        container_t##_Iterator it  = container_t##_Begin(self);                                                \
                                                                                                               \
        for(; !container_t##_Iterator_Equal(&it, &end); container_t##_Iterator_Increment(&it))                 \
        {                                                                                                      \
            const char it_value = *container_t##_Iterator_CRef(&it);                                           \
            if(it_value == data)                                                                               \
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
    }                                                                                                          \
                                                                                                               \
    int container_t##_Compare(const container_t * const self, const container_t * const second)                \
    {                                                                                                          \
        return strcmp(self->data, second->data);                                                               \
    }                                                                                                          \
                                                                                                               \
    unsigned int container_t##_Hash(const container_t * const self)                                            \
    {                                                                                                          \
        unsigned long hash = 5381;                                                                             \
        int c;                                                                                                 \
        const char * str = self->data;                                                                         \
                                                                                                               \
        while(str++)                                                                                           \
        {                                                                                                      \
            c    = *str;                                                                                       \
            hash = ((hash << 5) + hash) + c;                                                                   \
        }                                                                                                      \
                                                                                                               \
        return hash;                                                                                           \
    }

#define static_string(container_t, container_capacity)                  \
    typedef int (*container_t##_compare_t)(const char *, const char *); \
    typedef struct container_t container_t;                             \
    typedef struct container_t##_Iterator container_t##_Iterator;       \
    struct container_t                                                  \
    {                                                                   \
        size_t size;                                                    \
        size_t capacity;                                                \
        char data[container_capacity + 1];                              \
        char * end;                                                     \
    };                                                                  \
    struct container_t##_Iterator                                       \
    {                                                                   \
        char * value;                                                   \
    };                                                                  \
    __string_methods_h(container_t)

#define static_string_impl(container_t, container_capacity) __static_string_methods_c(container_t)

#define custom_allocator_string(container_t, allocator_t)               \
    typedef int (*container_t##_compare_t)(const char *, const char *); \
    typedef struct container_t container_t;                             \
    typedef struct container_t##_Iterator container_t##_Iterator;       \
    struct container_t                                                  \
    {                                                                   \
        size_t size;                                                    \
        size_t capacity;                                                \
        char * data;                                                    \
        allocator_t allocator;                                          \
    };                                                                  \
    struct container_t##_Iterator                                       \
    {                                                                   \
        char * value;                                                   \
    };                                                                  \
    __string_methods_h(container_t)

#define custom_allocator_string_impl(container_t, allocator_t) __custom_string_methods_c(container_t, allocator_t)

#define dynamic_string(container_t)                 \
    dynamic_memory(container_t##_DynamicAllocator); \
    custom_allocator_string(container_t, container_t##_DynamicAllocator);

#define dynamic_string_impl(container_t)                 \
    dynamic_memory_impl(container_t##_DynamicAllocator); \
    custom_allocator_string_impl(container_t, container_t##_DynamicAllocator);

#ifdef __cplusplus
}
#endif