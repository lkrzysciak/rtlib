#include "dec.h"

dynamic_memory_impl(DynamicMemory);

static_vector_impl(StaticVector, int, 10);
dynamic_vector_impl(DynamicVector, int);
custom_allocator_vector_impl(CustomAllocatorVector, int, DynamicMemory)