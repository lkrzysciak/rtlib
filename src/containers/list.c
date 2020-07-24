#ifdef __cplusplus
extern "C"
{
#endif

#include "list.h"
#include "../memory/object_pool.h"
#include <string.h>
#include <assert.h>
#include <stdint.h>

    typedef struct Node
    {
        Node * prev;
        Node * next;
    } Node;

    typedef struct List
    {
        Node * first;
        Node end;  // Aditional node for end iterator
        size_t element_size;
        Pool * pool;
    } List;

    static int List_PushMiddle(List * const self, const void * const data, const ListIterator * const iterator)
    {
        assert(self);
        assert(iterator);
        assert(Pool_Size(self->pool) >= 2);

        uint8_t * ptr = (uint8_t *)Pool_Alloc(self->pool);
        if(!ptr)
        {
            return -1;
        }

        Node * new_node = (Node *)ptr;
        Node * old_node = (Node*)iterator;
        assert(old_node);
        Node * node_before = old_node->prev;
        assert(node_before);
        new_node->prev    = node_before;
        new_node->next    = old_node;
        node_before->next = new_node;
        old_node->prev    = new_node;

        memcpy(ptr + sizeof(Node), data, self->element_size);

        return Pool_Size(self->pool);
    }

    static int List_PopMiddle(List * const self, const ListIterator * const iterator)
    {
        Node * to_delete_node = (Node*)iterator;
        Node * next_node      = to_delete_node->next;
        Node * prev_node      = to_delete_node->prev;

        next_node->prev = prev_node;
        prev_node->next = next_node;

        Pool_Free(self->pool, to_delete_node);
        return Pool_Size(self->pool);
    }

    List * List_Init(void * buffer, size_t size, size_t element_size)
    {
        List * list        = (List *)buffer;
        list->first        = &list->end;
        list->end.next     = NULL;
        list->end.prev     = NULL;
        list->element_size = element_size;
        list->pool =
            Pool_Init((uint8_t *)buffer + sizeof(List), size - sizeof(List), list->element_size + sizeof(Node));
        return list;
    }

    size_t List_Capacity(List * const self)
    {
        assert(self);

        return Pool_Capacity(self->pool);
    }

    size_t List_Size(List * const self)
    {
        assert(self);

        return Pool_Size(self->pool);
    }

    int List_Insert(List * const self, const void * const data, const ListIterator * iterator)
    {
        assert(self);

        ListIterator* begin = List_Begin(self);
        ListIterator* end   = List_End(self);

        if(ListIterator_Equal(begin, iterator))
        {
            return List_PushFront(self, data);
        }
        else if(ListIterator_Equal(end, iterator))
        {
            return List_PushBack(self, data);
        }
        else
        {
            return List_PushMiddle(self, data, iterator);
        }
    }

    int List_PushBack(List * const self, const void * const data)
    {
        assert(self);

        uint8_t * ptr = (uint8_t *)Pool_Alloc(self->pool);
        if(!ptr)
        {
            return -1;
        }

        Node * node        = (Node *)ptr;
        Node * oldLastNode = self->end.prev;

        if(self->first == &(self->end))
        {
            self->first = node;
        }
        else
        {
            assert(oldLastNode);
            oldLastNode->next = node;
        }

        assert(node);

        node->next     = &self->end;
        node->prev     = oldLastNode;
        self->end.prev = node;

        memcpy(ptr + sizeof(Node), data, self->element_size);

        return Pool_Size(self->pool);
    }

    int List_PushFront(List * const self, const void * const data)
    {
        assert(self);

        uint8_t * ptr = (uint8_t *)Pool_Alloc(self->pool);
        if(!ptr)
        {
            return -1;
        }

        Node * node = (Node *)ptr;

        node->prev        = NULL;
        node->next        = self->first;
        self->first->prev = node;
        self->first       = node;

        memcpy(ptr + sizeof(Node), data, self->element_size);

        return Pool_Size(self->pool);
    }

    int List_Erase(List * const self, const ListIterator * iterator)
    {
        assert(self);

        ListIterator* begin = List_Begin(self);
        ListIterator* end   = List_End(self);

        if(ListIterator_Equal(begin, iterator))
        {
            return List_PopFront(self);
        }
        else if(ListIterator_Equal(end, iterator))
        {
            return List_PopBack(self);
        }
        else
        {
            return List_PopMiddle(self, iterator);
        }
    }

    int List_PopBack(List * const self)
    {
        assert(self);
        Node * lastNode = self->end.prev;
        assert(lastNode);
        Node * newLastNode = lastNode->prev;

        if(newLastNode)
        {
            newLastNode->next = &self->end;
        }
        else
        {
            /* Last element will be deleted */
            self->first = &self->end;
        }

        self->end.prev = newLastNode;

        Pool_Free(self->pool, lastNode);
        return Pool_Size(self->pool);
    }

    int List_PopFront(List * const self)
    {
        assert(self);

        Node * firstNode = self->first;
        assert(firstNode);

        Node * newFirstNode = firstNode->next;
        assert(newFirstNode);

        newFirstNode->prev = NULL;
        self->first        = newFirstNode;

        Pool_Free(self->pool, firstNode);
        return Pool_Size(self->pool);
    }

    const void * List_Front(List * const self)
    {
        assert(self);

        void * ptr = (uint8_t *)self->first + sizeof(Node);
        return ptr;
    }

    const void * List_Back(List * const self)
    {
        assert(self);

        void * ptr = (uint8_t *)self->end.prev + sizeof(Node);
        return ptr;
    }

    ListIterator* List_Begin(List * const self)
    {
        assert(self);

        return self->first;
    }

    ListIterator* List_End(List * const self)
    {
        assert(self);

        return &self->end;
    }

    ListIterator * ListIterator_Increment(const ListIterator * const self)
    {
        assert(self);

        return self->next;
    }

    ListIterator * ListIterator_Decrement(const ListIterator * const self)
    {
        assert(self);

        return self->prev;
    }

    void * ListIterator_Value(const ListIterator * const self)
    {
        assert(self);

        void * ptr = (uint8_t *)self + sizeof(Node);
        return ptr;
    }

    bool ListIterator_Equal(const ListIterator * const self, const ListIterator * const list_iterator)
    {
        const bool are_equal = self == list_iterator;
        return are_equal;
    }

#ifdef __cplusplus
}
#endif