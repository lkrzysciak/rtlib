#include "object_pool.h"
#include <stdint.h>
#include <assert.h>

    typedef struct ObjectBlock ObjectBlock;

    typedef struct ObjectBlock
    {
        ObjectBlock * next_free_object_block;
    } ObjectBlock;

    typedef struct Pool
    {
        ObjectBlock * first_free_object_block;
        ObjectBlock * last_free_object_block;
        size_t memory_size;
        size_t object_size;
        size_t capacity;
        size_t size;
    } Pool;

    size_t Sizeof_ObjectBlock = sizeof(ObjectBlock);
    size_t Sizeof_Pool = sizeof(Pool);

    Pool * const Pool_Init(void * memory_block, size_t memory_size, size_t object_size)
    {
        assert(memory_block);
        assert(memory_size >= sizeof(Pool));

        Pool * pool = (Pool *)memory_block;

        pool->first_free_object_block = (ObjectBlock *)((uint8_t *)pool + sizeof(Pool));
        pool->memory_size             = memory_size;
        pool->object_size             = object_size;
        pool->size                    = 0;

        const size_t object_block_size       = pool->object_size + sizeof(ObjectBlock);
        const size_t memory_for_objects_size = pool->memory_size - sizeof(Pool);
        pool->capacity                       = memory_for_objects_size / object_block_size;

        pool->last_free_object_block =
            (ObjectBlock *)((uint8_t *)pool + sizeof(Pool) + object_block_size * (pool->capacity - 1));

        ObjectBlock * current_object_block  = NULL;
        ObjectBlock * previous_object_block = NULL;

        for(size_t index = 0; index < pool->capacity; ++index)
        {
            current_object_block =
                (ObjectBlock *)((uint8_t *)pool->first_free_object_block + (index * object_block_size));
            assert(current_object_block);
            current_object_block->next_free_object_block = NULL;

            if(previous_object_block)
            {
                previous_object_block->next_free_object_block = current_object_block;
            }
            previous_object_block = current_object_block;
        }
        return pool;
    }

    void * Pool_Alloc(Pool * const self)
    {
        assert(self);

        ObjectBlock * current_first_block = self->first_free_object_block;

        if(!current_first_block)
        {
            return NULL;
        }

        self->first_free_object_block = current_first_block->next_free_object_block;
        if(!self->first_free_object_block)
        {
            self->last_free_object_block = NULL;
        }

        self->size++;

        void * ptr = (uint8_t *)current_first_block + sizeof(ObjectBlock);
        return ptr;
    }

    void Pool_Free(Pool * const self, void * object)
    {
        assert(self);

        ObjectBlock * new_last_block     = (ObjectBlock *)((uint8_t *)object - sizeof(ObjectBlock));
        ObjectBlock * current_last_block = self->last_free_object_block;

        assert(new_last_block);

        if(current_last_block)
        {
            current_last_block->next_free_object_block = new_last_block;
        }
        else
        {
            self->first_free_object_block = new_last_block;
        }
        new_last_block->next_free_object_block = NULL;
        self->last_free_object_block           = new_last_block;
        self->size--;
    }

    size_t Pool_Capacity(Pool * const self)
    {
        assert(self);

        return self->capacity;
    }

    size_t Pool_Size(Pool * const self)
    {
        assert(self);

        return self->size;
    }
