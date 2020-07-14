#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef struct _Node _Node;
typedef struct _List _List;

typedef struct _Node
{
    _Node* prev;
    _Node* next;
} _Node;

typedef struct _List
{
    _Node* first;
    _Node* last;
    void* memory_buffer;
    size_t memory_buffer_size;
    size_t element_size;
    size_t capacity;
    size_t node_size;
} _List;

// Public
_List* _List_Init(size_t element_size, void* buffer, size_t size);
size_t _List_Capacity(_List* const self);
size_t _List_Size(_List* const self);
size_t _List_Insert(_List* const self, const void* const data);
size_t _List_PushBack(_List* const self, const void* const data);
size_t _List_PushFront(_List* const self, const void* const data);

// Static
size_t _List_HeaderSize();
size_t _List_NodeHeaderSize();

#ifdef __cplusplus
}
#endif