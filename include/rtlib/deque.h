#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error_codes.h"
#include "rtlib/vector.h"

#define deque_t(container_t, member_t)                                                                          \
    typedef struct container_t container_t;                                                                     \
    typedef struct container_t##_Iterator container_t##_Iterator;                                               \
    typedef int (*container_t##_compare_t)(const member_t *, const member_t *);                                 \
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
    container_t##_Iterator container_t##_Find(container_t * const self, const member_t data);                   \
    container_t##_Iterator container_t##_CustomFind(container_t * const self, const member_t data,              \
                                                    container_t##_compare_t compare_function);                  \
    void container_t##_Clear(container_t * const self);

#define static_deque_t(container_t, member_t, container_capacity)                                                     \
    vector_t(container_t##_Vector, member_t);                                                                         \
    static_vector_t(container_t##_Vector, member_t, container_capacity + 1);                                          \
                                                                                                                      \
    struct container_t##_Iterator                                                                                     \
    {                                                                                                                 \
        int index;                                                                                                    \
        const container_t * owner;                                                                                    \
    };                                                                                                                \
                                                                                                                      \
    struct container_t                                                                                                \
    {                                                                                                                 \
        container_t##_Vector vector;                                                                                  \
        int begin;                                                                                                    \
        int end;                                                                                                      \
        container_t##_compare_t compare_function;                                                                     \
    };                                                                                                                \
                                                                                                                      \
    void container_t##_Construct(container_t * const self, container_t##_compare_t compare_function)                  \
    {                                                                                                                 \
        container_t##_Vector_Construct(&self->vector, compare_function);                                              \
        self->begin            = 0;                                                                                   \
        self->end              = 0;                                                                                   \
        self->compare_function = compare_function;                                                                    \
        member_t tmp           = { 0 };                                                                               \
        for(size_t idx = 0; idx < container_capacity + 1; idx++)                                                      \
        {                                                                                                             \
            container_t##_Vector_PushBack(&self->vector, tmp);                                                        \
        }                                                                                                             \
    }                                                                                                                 \
                                                                                                                      \
    void container_t##_Destroy(container_t * const self)                                                              \
    {                                                                                                                 \
        container_t##_Vector_Destroy(&self->vector);                                                                  \
    }                                                                                                                 \
                                                                                                                      \
    size_t container_t##_Size(const container_t * const self)                                                         \
    {                                                                                                                 \
        if(self->begin <= self->end)                                                                                  \
        {                                                                                                             \
            return self->end - self->begin;                                                                           \
        }                                                                                                             \
        else                                                                                                          \
        {                                                                                                             \
            return container_capacity + 1 - (self->begin - self->end);                                                \
        }                                                                                                             \
    }                                                                                                                 \
                                                                                                                      \
    bool container_t##_Empty(const container_t * const self)                                                          \
    {                                                                                                                 \
        return self->begin == self->end;                                                                              \
    }                                                                                                                 \
                                                                                                                      \
    int container_t##_PushBack(container_t * const self, member_t data)                                               \
    {                                                                                                                 \
        size_t newEnd = (self->end + 1) % (container_capacity + 1);                                                   \
        if(newEnd == self->begin)                                                                                     \
        {                                                                                                             \
            return ALLOCATION_ERROR;                                                                                  \
        }                                                                                                             \
        container_t##_Vector_SetValue(&self->vector, self->end, data);                                                \
        self->end = newEnd;                                                                                           \
        return container_t##_Size(self);                                                                              \
    }                                                                                                                 \
                                                                                                                      \
    int container_t##_PopBack(container_t * const self)                                                               \
    {                                                                                                                 \
        size_t newEnd = (self->end - 1) % (container_capacity + 1);                                                   \
        newEnd        = newEnd < 0 ? (newEnd + container_capacity + 1) : newEnd;                                      \
        self->end     = newEnd;                                                                                       \
        return container_t##_Size(self);                                                                              \
    }                                                                                                                 \
                                                                                                                      \
    int container_t##_PushFront(container_t * const self, member_t data)                                              \
    {                                                                                                                 \
        int newBegin = (self->begin - 1) % (container_capacity + 1);                                                  \
        newBegin     = newBegin < 0 ? (newBegin + container_capacity + 1) : newBegin;                                 \
        if(newBegin == self->end)                                                                                     \
        {                                                                                                             \
            return ALLOCATION_ERROR;                                                                                  \
        }                                                                                                             \
        self->begin = newBegin;                                                                                       \
        container_t##_Vector_SetValue(&self->vector, self->begin, data);                                              \
        return container_t##_Size(self);                                                                              \
    }                                                                                                                 \
                                                                                                                      \
    int container_t##_PopFront(container_t * const self)                                                              \
    {                                                                                                                 \
        int newBegin = (self->begin + 1) % (container_capacity + 1);                                                  \
        newBegin     = newBegin < 0 ? (newBegin + container_capacity + 1) : newBegin;                                 \
        self->begin  = newBegin;                                                                                      \
        return container_t##_Size(self);                                                                              \
    }                                                                                                                 \
                                                                                                                      \
    int container_t##_Insert(container_t * const self, container_t##_Iterator * const iterator, member_t data)        \
    {                                                                                                                 \
        int ret = -1;                                                                                                 \
        if(iterator->index == 0)                                                                                      \
        {                                                                                                             \
            ret = container_t##_PushFront(self, data);                                                                \
        }                                                                                                             \
        else if(iterator->index == container_t##_Size(self))                                                          \
        {                                                                                                             \
            ret = container_t##_PushBack(self, data);                                                                 \
        }                                                                                                             \
        else                                                                                                          \
        {                                                                                                             \
            size_t newEnd = (self->end + 1) % (container_capacity + 1);                                               \
            if(newEnd == self->begin)                                                                                 \
            {                                                                                                         \
                return -1;                                                                                            \
            }                                                                                                         \
            self->end = newEnd;                                                                                       \
            int curr  = container_t##_Size(self) - 2;                                                                 \
            while(curr >= iterator->index)                                                                            \
            {                                                                                                         \
                member_t tmp = container_t##_GetValue(self, curr);                                                    \
                container_t##_SetValue(self, curr + 1, tmp);                                                          \
                curr--;                                                                                               \
            }                                                                                                         \
            container_t##_SetValue(self, iterator->index, data);                                                      \
            ret = container_t##_Size(self);                                                                           \
        }                                                                                                             \
        return ret;                                                                                                   \
    }                                                                                                                 \
                                                                                                                      \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator)                        \
    {                                                                                                                 \
        int ret = -1;                                                                                                 \
        if(iterator->index == 0)                                                                                      \
        {                                                                                                             \
            ret = container_t##_PopFront(self);                                                                       \
        }                                                                                                             \
        else if(iterator->index == container_t##_Size(self))                                                          \
        {                                                                                                             \
            ret = container_t##_PopBack(self);                                                                        \
        }                                                                                                             \
        else                                                                                                          \
        {                                                                                                             \
            size_t newEnd = (self->end - 1) % (container_capacity + 1);                                               \
            newEnd        = newEnd < 0 ? (newEnd + container_capacity + 1) : newEnd;                                  \
            if(newEnd == self->begin)                                                                                 \
            {                                                                                                         \
                return -1;                                                                                            \
            }                                                                                                         \
            self->end = newEnd;                                                                                       \
            int curr  = iterator->index;                                                                              \
            while(curr <= container_t##_Size(self))                                                                   \
            {                                                                                                         \
                member_t tmp = container_t##_GetValue(self, curr + 1);                                                \
                container_t##_SetValue(self, curr, tmp);                                                              \
                curr++;                                                                                               \
            }                                                                                                         \
            ret = container_t##_Size(self);                                                                           \
        }                                                                                                             \
        return ret;                                                                                                   \
    }                                                                                                                 \
                                                                                                                      \
    member_t container_t##_Front(const container_t * const self)                                                      \
    {                                                                                                                 \
        return container_t##_GetValue(self, 0);                                                                       \
    }                                                                                                                 \
                                                                                                                      \
    member_t container_t##_Back(const container_t * const self)                                                       \
    {                                                                                                                 \
        return container_t##_GetValue(self, container_t##_Size(self) - 1);                                            \
    }                                                                                                                 \
                                                                                                                      \
    member_t container_t##_GetValue(const container_t * const self, size_t index)                                     \
    {                                                                                                                 \
        return container_t##_Vector_GetValue(&self->vector, (self->begin + index) % (container_capacity + 1));        \
    }                                                                                                                 \
                                                                                                                      \
    void container_t##_SetValue(container_t * const self, size_t index, member_t value)                               \
    {                                                                                                                 \
        return container_t##_Vector_SetValue(&self->vector, (self->begin + index) % (container_capacity + 1), value); \
    }                                                                                                                 \
                                                                                                                      \
    container_t##_Iterator container_t##_Begin(const container_t * const self)                                        \
    {                                                                                                                 \
        container_t##_Iterator tmp = { 0, self };                                                                     \
        return tmp;                                                                                                   \
    }                                                                                                                 \
                                                                                                                      \
    container_t##_Iterator container_t##_End(const container_t * const self)                                          \
    {                                                                                                                 \
        container_t##_Iterator tmp = { (int)container_t##_Size(self), self };                                         \
        return tmp;                                                                                                   \
    }                                                                                                                 \
                                                                                                                      \
    member_t container_t##_Iterator_GetValue(const container_t##_Iterator * const self)                               \
    {                                                                                                                 \
        return container_t##_GetValue(self->owner, self->index);                                                      \
    }                                                                                                                 \
                                                                                                                      \
    void container_t##_Iterator_SetValue(container_t##_Iterator * const self, member_t value)                         \
    {                                                                                                                 \
        return container_t##_SetValue((container_t *)self->owner, self->index, value);                                \
    }                                                                                                                 \
                                                                                                                      \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                                     \
                                      const container_t##_Iterator * const second)                                    \
    {                                                                                                                 \
        return first->index == second->index;                                                                         \
    }                                                                                                                 \
                                                                                                                      \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self)                                        \
    {                                                                                                                 \
        self->index++;                                                                                                \
    }                                                                                                                 \
                                                                                                                      \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self)                                        \
    {                                                                                                                 \
        self->index--;                                                                                                \
    }                                                                                                                 \
                                                                                                                      \
    container_t##_Iterator container_t##_Find(container_t * const self, const member_t data)                          \
    {                                                                                                                 \
        container_t##_Iterator end = container_t##_End(self);                                                         \
        container_t##_Iterator it  = container_t##_Begin(self);                                                       \
                                                                                                                      \
        for(; !container_t##_Iterator_Equal(&it, &end); container_t##_Iterator_Increment(&it))                        \
        {                                                                                                             \
            const member_t it_value = container_t##_Iterator_GetValue(&it);                                           \
            if(self->compare_function(&data, &it_value) == 0)                                                         \
            {                                                                                                         \
                break;                                                                                                \
            }                                                                                                         \
        }                                                                                                             \
        return it;                                                                                                    \
    }                                                                                                                 \
                                                                                                                      \
    container_t##_Iterator container_t##_CustomFind(container_t * const self, const member_t data,                    \
                                                    container_t##_compare_t compare_function)                         \
    {                                                                                                                 \
        container_t##_Iterator end = container_t##_End(self);                                                         \
        container_t##_Iterator it  = container_t##_Begin(self);                                                       \
                                                                                                                      \
        for(; !container_t##_Iterator_Equal(&it, &end); container_t##_Iterator_Increment(&it))                        \
        {                                                                                                             \
            const member_t it_value = container_t##_Iterator_GetValue(&it);                                           \
            if(compare_function(&data, &it_value) == 0)                                                               \
            {                                                                                                         \
                break;                                                                                                \
            }                                                                                                         \
        }                                                                                                             \
        return it;                                                                                                    \
    }                                                                                                                 \
                                                                                                                      \
    void container_t##_Clear(container_t * const self)                                                                \
    {                                                                                                                 \
        self->begin = 0;                                                                                              \
        self->end   = 0;                                                                                              \
    }
