#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>

typedef struct List List;
typedef struct Node Node;

typedef struct ListIterator
{
    Node* node;
} ListIterator;

List* List_Init(void* buffer, size_t size, size_t element_size);
size_t List_Capacity(List* const self);
size_t List_Size(List* const self);

int List_Insert(List* const self, const void* const data, const ListIterator* iterator);
int List_PushBack(List* const self, const void* const data);
int List_PushFront(List* const self, const void* const data);

int List_Erase(List* const self, const ListIterator* iterator);
int List_PopBack(List* const self);
int List_PopFront(List* const self);

const void* List_Front(List* const self);
const void* List_Back(List* const self);

ListIterator List_Begin(List* const self);
ListIterator List_End(List* const self);

void ListIterator_Increment(ListIterator* const self);
void ListIterator_Decrement(ListIterator* const self);
void* ListIterator_Value(const ListIterator* const self);
bool ListIterator_Equal(const ListIterator* const self, const ListIterator* const list_iterator);

#ifdef __cplusplus
}
#endif