#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef struct Heap Heap;

Heap* const Heap_Init(void* memory_block, size_t memory_size);
void* Heap_Alloc(Heap* const self, size_t size);
void Heap_Free(Heap* const self);

#ifdef __cplusplus
}
#endif