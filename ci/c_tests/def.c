#include "dec.h"

dynamic_memory_impl(DynamicMemory);

static_vector_impl(StaticVector, int, 10);
dynamic_vector_impl(DynamicVector, int);
custom_allocator_vector_impl(CustomAllocatorVector, int, DynamicMemory);
static_list_impl(StaticList, int, 10);
dynamic_list_impl(DynamicList, int);
custom_allocator_list_impl(CustomAllocatorList, int, DynamicMemory);
static_deque_impl(StaticDeque, int, 10);
static_set_impl(StaticSet, int, 10);
custom_allocator_set_impl(CustomAllocatorSet, int, DynamicMemory);
static_unordered_set_impl(StaticUnorderedSet, int, 10);
custom_allocator_unordered_set_impl(CustomAllocatorUnorderedSet, int, DynamicMemory);