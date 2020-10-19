#pragma once

#include <assert.h>
#include <stdbool.h>

#define declare_static_hash_table_t(container_t, member_t, container_capacity) \
typedef struct container_t container_t; \
typedef struct container_t##_iterator container_t##_iterator; \
typedef struct container_t##_node container_t##_node; \
\
typedef struct container_t##_node \
{ \
    bool is_busy; \
    member_t value; \
} container_t##_node; \
\
typedef struct container_t##_iterator \
{ \
    container_t##_node * node; \
    container_t* container; \
} container_t##_iterator; \
\
typedef int(*compare_t)(const member_t*, const member_t*); \
typedef unsigned int(*hash_t)(const member_t*); \
\
typedef struct container_t \
{ \
    container_t##_node data[container_capacity]; \
    compare_t compare_function; \
    hash_t hash_function; \
    size_t size; \
} container_t; \
\
void container_t##_Init(container_t* const self, compare_t compare, hash_t hash); \
size_t container_t##_Size(const container_t * const self); \
bool container_t##_Empty(const container_t * const self); \
int container_t##_Insert(container_t * const self, container_t##_iterator* const iterator, member_t data); \
int container_t##_Erase(container_t * const self, container_t##_iterator* const iterator); \
container_t##_iterator container_t##_Begin(const container_t * const self); \
container_t##_iterator container_t##_End(const container_t * const self); \
member_t container_t##_Iterator_GetValue(const container_t##_iterator* const self); \
void container_t##_Iterator_SetValue(container_t##_iterator* const self, member_t value); \
bool container_t##_Iterator_Equal(const container_t##_iterator* const first, const container_t##_iterator* const second); \
void container_t##_Iterator_Increment(container_t##_iterator* const self); \
void container_t##_Iterator_Decrement(container_t##_iterator* const self); \
container_t##_iterator container_t##_Find(container_t * const self, member_t data);


#define define_static_hash_table_t(container_t, member_t, container_capacity) \
void container_t##_Init(container_t* const self, compare_t compare, hash_t hash) \
{ \
    assert(self); \
    assert(compare); \
    assert(hash); \
    \
    self->size = 0; \
    self->compare_function = compare; \
    self->hash_function = hash; \
    memset(self->data, 0, container_capacity * sizeof(member_t)); \
} \
\
size_t container_t##_Size(const container_t * const self) \
{ \
    assert(self); \
    \
    return self->size; \
} \
\
bool container_t##_Empty(const container_t * const self) \
{ \
    assert(self); \
    \
    return self->size == 0; \
} \
\
int container_t##_Insert(container_t * const self, member_t data) \
{ \
    assert(self); \
    \
    if(self->size == container_capacity) \
    { \
        return -1; \
    } \
    const unsigned int hash_value = self->hash_function(&data); \
    unsigned int index = hash_value % container_capacity; \
    \
    while(self->data[index].is_busy) \
    { \
        ++index; \
        index %= container_capacity; \
    } \
    self->data[index].is_busy = true; \
    self->data[index].value = data; \
    ++self->size; \
    return self->size; \
} \
\
int container_t##_Erase(container_t * const self, container_t##_iterator* const iterator) \
{ \
    assert(self); \
    \
    iterator->node->is_busy = false; \
    --self->size; \
    return self->size; \
} \
\
container_t##_iterator container_t##_Begin(const container_t * const self) \
{ \
    assert(self); \
    \
    container_t##_iterator it = {0}; \
    container_t##_node* begin_node = (container_t##_node*)self->data; \
    for(;!begin_node->is_busy; ++begin_node) {} \
    it.node = begin_node; \
    it.container = (container_t*)self; \
    return it; \
} \
\
container_t##_iterator container_t##_End(const container_t * const self) \
{ \
    assert(self); \
    \
    container_t##_iterator it = {0}; \
    container_t##_node* end_node = (container_t##_node*)self->data + container_capacity; \
    it.node = end_node; \
    it.container = (container_t*)self; \
    return it; \
} \
\
member_t container_t##_Iterator_GetValue(const container_t##_iterator* const self) \
{ \
    assert(self); \
    \
    const member_t member = self->node->value; \
    return member; \
} \
\
void container_t##_Iterator_SetValue(container_t##_iterator* const self, member_t value) \
{ \
    assert(self); \
    \
    self->node->value = value; \
} \
\
bool container_t##_Iterator_Equal(const container_t##_iterator* const first, const container_t##_iterator* const second) \
{ \
    return first->node == second->node; \
} \
\
void container_t##_Iterator_Increment(container_t##_iterator* const self) \
{ \
    assert(self); \
    \
    container_t##_iterator end_it = container_t##_End(self->container); \
    container_t##_node* next_node = self->node + 1; \
    for(;!next_node->is_busy && next_node != end_it.node; ++next_node) {} \
    self->node = next_node; \
} \
\
void container_t##_Iterator_Decrement(container_t##_iterator* const self) \
{ \
    assert(self); \
    \
    container_t##_iterator begin_it = container_t##_Begin(self->container); \
    container_t##_node* prev_node = self->node - 1; \
    for(;!prev_node->is_busy && prev_node != begin_it.node; --prev_node) {} \
    self->node = prev_node; \
} \
\
container_t##_iterator container_t##_Find(container_t * const self, member_t data) \
{ \
    container_t##_iterator it = {0}; \
    container_t##_node* found_node = (container_t##_node*)self->data + container_capacity; \
    const unsigned int hash_value = self->hash_function(&data); \
    unsigned int start_index = hash_value % container_capacity; \
    for(unsigned int index=0; index<container_capacity; index++) \
    { \
        const unsigned int index_with_offset = (index + start_index) % container_capacity; \
        int compare_result = self->compare_function(&data,  &self->data[index_with_offset].value); \
        if(compare_result == 0) \
        { \
            found_node = &self->data[index_with_offset]; \
            break; \
        } \
    } \
    it.node = found_node; \
    it.container = (container_t*)self; \
    return it; \
} \

