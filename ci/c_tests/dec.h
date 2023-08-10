#include <rtlib/vector.h>
#include <rtlib/memory.h>

dynamic_memory(DynamicMemory);

static_vector(StaticVector, int, 10);
dynamic_vector(DynamicVector, int);
custom_allocator_vector(CustomAllocatorVector, int, DynamicMemory)
