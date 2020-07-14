#ifdef __cplusplus
extern "C" {
#endif

#include "object_pool.h"
#include <stdint.h>
#include <assert.h>

typedef struct ObjectBlock ObjectBlock;

typedef struct ObjectBlock
{
    ObjectBlock* next_free_object_block;
    ObjectBlock* prev_free_object_block;
} ObjectBlock;

typedef struct Pool
{
    ObjectBlock* first_free_object_block;
    ObjectBlock* last_free_object_block;
    size_t memory_size;
    size_t object_size;
    size_t capacity;
} Pool;

Pool* const Pool_Init(void* memory_block, size_t memory_size, size_t object_size)
{
    assert(memory_block);
    assert(memory_size >= sizeof(Pool));

    Pool* pool = (Pool*) memory_block;

    pool->first_free_object_block = (ObjectBlock*)((uint8_t*)pool + sizeof(Pool));
    pool->memory_size = memory_size;
    pool->object_size = object_size;

    const size_t object_block_size = pool->object_size + sizeof(ObjectBlock);
    const size_t memory_for_objects_size = pool->memory_size - sizeof(Pool);
    pool->capacity = memory_for_objects_size / object_block_size;

    pool->last_free_object_block = (ObjectBlock*)((uint8_t*)pool + sizeof(Pool) + object_block_size * (pool->capacity - 1));

    ObjectBlock* current_object_block = NULL;
    ObjectBlock* previous_object_block = NULL;

    for(size_t index=0; index<pool->capacity; ++index)
    {
        current_object_block = (ObjectBlock*)((uint8_t*)pool->first_free_object_block + (index * object_block_size));
        assert(current_object_block);
        current_object_block->next_free_object_block = NULL;
        current_object_block->prev_free_object_block = NULL;

        if(previous_object_block)
        {
            current_object_block->prev_free_object_block = previous_object_block;
            previous_object_block->next_free_object_block = current_object_block;
        }
        previous_object_block = current_object_block;
    }
    return pool;
}

void* Pool_Alloc(Pool* const self)
{
    assert(self);

    ObjectBlock* block = self->first_free_object_block;

    if(!block)
    {
        return NULL;
    }
    
    self->first_free_object_block = block->next_free_object_block;
    if(self->first_free_object_block)
    {
        self->first_free_object_block->prev_free_object_block = NULL;
    }
    void* ptr = (uint8_t*)block + sizeof(ObjectBlock);
    return ptr;
}

void Pool_Free(Pool* const self, void* object)
{
    assert(self);

    ObjectBlock* new_last_block = (ObjectBlock*)((uint8_t*)object - sizeof(ObjectBlock));
    ObjectBlock* current_last_block = self->last_free_object_block;

    assert(new_last_block);
    assert(current_last_block);

    current_last_block->next_free_object_block = new_last_block;
    new_last_block->prev_free_object_block = current_last_block;
    new_last_block->next_free_object_block = NULL;
    self->last_free_object_block = new_last_block;
}

size_t Pool_Capacity(Pool* const self)
{
    assert(self);

    return self->capacity;
}

#ifdef __cplusplus
}
#endif