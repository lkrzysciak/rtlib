#pragma once

/*
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#define node_t(Name, Type) \
typedef struct Name Name; \
typedef struct Name \
{ \
    Name* prev; \
    Name* next; \
    Type data; \
} Name;

#define list_t(Name, Type, Size) \
\
node_t(Name##_Node, Type); \
\
typedef struct \
{ \
    bool memory_chunks[Size]; \
    Name##_Node nodes[Size]; \
    Name##_Node* first; \
    Name##_Node* last; \
} Name; \
\
Name##_Node* get_free_node_chunk_##Name(Name* const self) \
{ \
  for(int idx=0; idx<Size; ++idx) \
  { \
    if(self->memory_chunks[idx] == false) \
    { \
        self->memory_chunks[idx] = true; \
        return &self->nodes[idx]; \
    } \
  } \
  return NULL; \
} \
\
Name##_Node* get_node_by_index_##Name(Name* const self, int index) \
{ \
    const size_t size = size_##Name(self); \
    if(index >= size) \
    { \
        return NULL; \
    } \
    Name##_Node* node_ptr = self->first; \
    while(index--) \
    { \
        node_ptr = node_ptr->next; \
    } \
    return node_ptr; \
} \
\
void init_##Name(Name* const self) \
{ \
    assert(self); \
    self->first = NULL; \
    self->last = NULL; \
    memset(self->nodes, 0, Size * sizeof(Type)); \
    memset(self->memory_map, 0, Size % 8 + 1); \
} \
\
size_t size_##Name(Name* const self) \
{ \
    assert(self); \
    size_t size = 0; \
    Name##_Node* node_ptr = self->first; \
    while(node_ptr != self->last) \
        { \
        node_ptr = node_ptr->next; \
        ++size; \
    } \
    return size; \
} \
\
int insert_##Name(Name* const self, Type value, int index) \
{ \
    assert(self); \
    const size_t size = size_##Name(self); \
    if(index > size) { return -1; } \
    else { \
        Name##_Node* node_ptr = get_free_node_chunk_##Name(self); \
        if(mem_chunk_no == NULL) \
        { \
            assert(false); \
            return -1; \
        } \
        \
        if(index == 0) \
        {
            self->first = node_ptr;
            node_ptr->prev = NULL;
            node_ptr->next = ??;
        }
        if(index == size)
        {
            self->last = node_ptr;
            node_ptr->next = NULL;
            node_ptr->prev = ??;
        }
        if(index > 0 && index < size)
        {

        }
        return 0; \
    } \
} \
\
void push_back_##Name(Name* object, Type value) \
{ \
  \
} \
\
\
void push_front_##Name(Name* object, Type value) \
{ \
  \
} \
\
Type get_##Name(Name* object, int index, int err) \
{ \
   return object->nodes[index].data; \
}

#define list_init(Name) init_##Name
#define list_size(Name) size_##Name
#define list_insert(Name) insert_##Name

#ifdef __cplusplus
}
#endif
*/