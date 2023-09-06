#include <rtlib/memory.h>
#include <rtlib/vector.h>
#include <rtlib/list.h>
#include <rtlib/deque.h>
#include <rtlib/set.h>
#include <rtlib/unordered_set.h>

dynamic_memory(DynamicMemory);

static_vector(StaticVector, int, 10);
dynamic_vector(DynamicVector, int);
custom_allocator_vector(CustomAllocatorVector, int, DynamicMemory);
static_list(StaticList, int, 10);
dynamic_list(DynamicList, int);
custom_allocator_list(CustomAllocatorList, int, DynamicMemory);
static_deque(StaticDeque, int, 10);
static_set(StaticSet, int, 10);
custom_allocator_set(CustomAllocatorSet, int, DynamicMemory);
static_unordered_set(StaticUnorderedSet, int, 10);
custom_allocator_unordered_set(CustomAllocatorUnorderedSet, int, DynamicMemory);
