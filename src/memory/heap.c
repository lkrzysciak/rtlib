#ifdef __cplusplus
extern "C" {
#endif

#include "heap.h"
#include <assert.h>
#include <stdint.h>

typedef struct MemoryBlock MemoryBlock;

typedef struct MemoryBlock
{
    MemoryBlock* next;
    size_t size;
} MemoryBlock;

typedef struct Heap
{
    size_t memory_size;
    MemoryBlock* first;
} Heap;

Heap* const Heap_Init(void* memory_block, size_t memory_size)
{
    assert(memory_block);
    assert(memory_size >= sizeof(Heap) + sizeof(MemoryBlock));
    
    Heap* heap = (Heap*)memory_block;
    heap->memory_size = memory_size;
    heap->first = (MemoryBlock*)((uint8_t*)heap + sizeof(Heap));
    heap->first->next = NULL;
    heap->first->size = memory_size - sizeof(Heap) - sizeof(MemoryBlock);

    return heap;
}

void* Heap_Alloc(Heap* const self, size_t size)
{
    assert(self);

    MemoryBlock* block = self->first;
    uint8_t* ptr = NULL;

    do
    {
        assert(block);

        if(block->size >= size)
        {
            ptr = (uint8_t*)block + sizeof(MemoryBlock);
            break;
        }
    } while (block->next != NULL);
    
    return ptr;
}

void Heap_Free(Heap* const self)
{
    assert(self);

}

#ifdef __cplusplus
}
#endif