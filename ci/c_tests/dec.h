#include <rtlib/memory.h>
#include <rtlib/vector.h>
#include <rtlib/list.h>
#include <rtlib/deque.h>

dynamic_memory(DynamicMemory);

static_vector(StaticVector, int, 10);
dynamic_vector(DynamicVector, int);
custom_allocator_vector(CustomAllocatorVector, int, DynamicMemory);
static_list(StaticList, int, 10);
dynamic_list(DynamicList, int);
custom_allocator_list(CustomAllocatorList, int, DynamicMemory);
static_deque(StaticDeque, int, 10);
