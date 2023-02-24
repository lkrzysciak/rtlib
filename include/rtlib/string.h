#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "error_codes.h"

#define string_t(container_t)                                                                                   \
    typedef struct container_t container_t;                                                                     \
    typedef struct container_t##_Iterator container_t##_Iterator;                                               \
    typedef int (*container_t##_compare_t)(const member_t *, const member_t *);                                 \
                                                                                                                \
    void container_t##_Construct(container_t * const self);                                                     \
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
    void container_t##_Clear(container_t * const self);                                                         \
    const char * container_t##_CStr(container_t * const self);                                                  \
    container_t##_Iterator container_t##_Begin(const container_t * const self);                                 \
    container_t##_Iterator container_t##_End(const container_t * const self);                                   \
    member_t container_t##_Iterator_GetValue(const container_t##_Iterator * const self);                        \
    void container_t##_Iterator_SetValue(container_t##_Iterator * const self, member_t value);                  \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                               \
                                      const container_t##_Iterator * const second);                             \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self);                                 \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self);                                 \
    container_t##_Iterator container_t##_Find(container_t * const self, const container_t * data);
