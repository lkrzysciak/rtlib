#include "vector.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

typedef struct VectorIterator
{
    void* ptr;
} VectorIterator;

typedef struct Vector
{
    size_t element_size;
    size_t capacity;
    size_t size;
} Vector;


Vector* Vector_Init(void * buffer, size_t size, size_t element_size)
{
    assert(buffer);
    assert(size >= sizeof(Vector));

    Vector* vector = (Vector*)buffer;
    vector->element_size = element_size;
    vector->size = 0;
    vector->capacity = (size - sizeof(Vector)) / vector->element_size;
    
    return vector;
}

size_t Vector_Size(Vector * const self)
{
    assert(self);

    return self->size;
}

int Vector_PushBack(Vector * const self, const void * const data)
{
    assert(self);
    assert(data);

    if(self->size < self->capacity)
    {
        void* ptr = (uint8_t*)self + sizeof(Vector) + (self->size * self->element_size);
        memcpy(ptr, data, self->element_size);
        ++self->size;

        return self->size;
    }
    else
    {
        return -1;
    }
}

int Vector_PopBack(Vector * const self)
{
    assert(self);

    if(self->size == 0)
    {
        return -1;
    }
    else
    {
        self->size--;
        return self->size;
    }
}

int Vector_PushFront(Vector * const self, const void * const data)
{
    assert(self);
    assert(data);

    if(self->size < self->capacity)
    {
        const size_t to_move_size = self->size * self->element_size;
        uint8_t* ptr = (uint8_t*)self + sizeof(Vector);
        memmove(ptr + self->element_size, ptr, to_move_size);
        memcpy(ptr, data, self->element_size);
        ++self->size;

        return self->size;
    }
    else
    {
        return -1;
    }
}

int Vector_PopFront(Vector * const self)
{
    assert(self);

    if(self->size == 0)
    {
        return -1;
    }
    else
    {
        self->size--;
        const size_t to_move_size = self->size * self->element_size;
        uint8_t* ptr = (uint8_t*)self + sizeof(Vector);
        memmove(ptr + self->element_size, ptr, to_move_size);
        return self->size;
    }
}

const void * Vector_Front(Vector * const self)
{
    assert(self);
    assert(self->size > 0);

    return (uint8_t*)self + sizeof(Vector);
}

const void * Vector_Back(Vector * const self)
{
    assert(self);
    assert(self->size > 0);

    const size_t lastElementOffset = self->element_size * (self->size - 1);

    return (uint8_t*)self + sizeof(Vector) + lastElementOffset;
}