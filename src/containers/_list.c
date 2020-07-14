#include "_list.h"
#include <string.h>
#include <assert.h>

_List* _List_Init(size_t element_size, void* buffer, size_t size)
{
    if(!buffer || size < sizeof(_List))
    {
        return NULL;
    }
    _List* list = (_List*)buffer;
    list->first = NULL;
    list->last = NULL;
    list->memory_buffer = buffer;
    list->memory_buffer_size = size;
    list->element_size = element_size;
    list->capacity = (list->memory_buffer_size - _List_HeaderSize()) / list->element_size;
    list->node_size = _List_NodeHeaderSize() + list->element_size;
    return list;
}

size_t _List_Capacity(_List* const self)
{
    assert(self);

    return self->capacity;
}

size_t _List_Size(_List* const self)
{
    assert(self);
    size_t size = 0;

    const _Node* node = self->first;
    while(node != self->last)
    {
        node = node->next;
        ++size;
    }
    return size;
}

size_t _List_Insert(_List* const self, const void* const data)
{
    assert(self);


}

size_t _List_PushBack(_List* const self, const void* const data)
{

}

size_t _List_PushFront(_List* const self, const void* const data)
{
    
}

size_t _List_HeaderSize()
{
    return sizeof(_List);
}

size_t _List_NodeHeaderSize()
{
    return sizeof(_Node);
}
