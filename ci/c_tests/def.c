#include "dec.h"

static unsigned int int_Hash(const int * value)
{
    return *value;
}

static int int_Compare(const int * v1, const int * v2)
{
    if(*v1 > *v2)
    {
        return 1;
    }
    else if(*v1 < *v2)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

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

static_map_impl(StaticMap, int, int, 10);
custom_allocator_map_impl(CustomAllocatorMap, int, int, DynamicMemory);
dynamic_map_impl(DynamicMap, int, int);

static_unordered_map_impl(StaticUnorderedMap, int, int, 10);
custom_allocator_unordered_map_impl(CustomAllocatorUnorderedMap, int, int, DynamicMemory);
dynamic_unordered_map_impl(DynamicUnorderedMap, int, int);